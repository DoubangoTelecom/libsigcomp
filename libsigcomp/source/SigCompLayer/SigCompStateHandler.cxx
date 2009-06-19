/**
	Copyright (C) 2009 Mamadou Diop <diopmamadou@yahoo.fr>

	This file is part of libSigComp project.

    libSigComp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    libSigComp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with libSigComp.  If not, see <http://www.gnu.org/licenses/>.

	For Commercial Use or non-GPL Licensing please contact me at <diopmamadou@yahoo.fr>
*/

#include <global_config.h>
#include <libsigcomp/SigCompLayer/SigCompStateHandler.h>
#include <libsigcomp/NACK_CODES.h>

using namespace std;

/**
State handler constructor. Entity responsible for accessing and storing state information
      once permission is granted by the application.
*/
SigCompStateHandler::SigCompStateHandler()
:SafeObject()
{
	this->hasSipSdpDictionary = false;

	// RFC 3320 - 3.3.  SigComp Parameters
	this->sigcomp_parameters.setDmsValue( SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE );
	this->sigcomp_parameters.setSmsValue( SIP_RFC5049_STATE_MEMORY_SIZE );
	this->sigcomp_parameters.setCpbValue( SIP_RFC5049_CYCLES_PER_BIT );
	this->sigcomp_parameters.setSigCompVersion( SIP_RFC5049_SIGCOMP_VERSION );
}

/**
State handler destructor
*/
SigCompStateHandler::~SigCompStateHandler()
{
	// Delete all compartments
	map<t_uint64, SigCompCompartment*>::iterator it;
	SAFE_CLEAR_MAP(this->compartments, it);

	// Delete all dictionaries
	map<SigCompBuffer*, SigCompState*>::iterator it2;
	SAFE_CLEAR_MAP(this->dictionaries, it2);
}

/**
Get compatement by identifier. If the compartment does not exist a new one is
created

@param id the compartement identifier

@returns the compartment
*/
SigCompCompartment* SigCompStateHandler::getCompartment(t_uint64 id)
{
	this->lock();

	bool exit = (this->compartments.find(id)!=this->compartments.end());

	SigCompCompartment* result = NULL;

	if(exit){
		result = this->compartments[id];
	}else{
		result = new SigCompCompartment(id, this->getSigCompParameters()->getParameters());
		this->compartments[id] = result;
	}

	this->unlock();

	return result;
}

/**
Delete a compartement with the given identifier

@param id the identifier of the compartment to delete

@returns void
*/
void SigCompStateHandler::deleteCompartment(t_uint64 id)
{
	this->lock();
	this->compartments.erase(this->compartments.find(id));
	this->unlock();
}

/**
Check for the existence of a compartment

@param id the id of the compartment to check

@returns true if the the compartment exist, otherwise return false
*/
bool SigCompStateHandler::compartmentExist(t_uint64 id)
{
	this->lock();
	return (this->compartments.find(id)!=this->compartments.end());
	this->unlock();
}

/*
Find a state by partial identifier

@param partial_identifier the partial identifier of the state to find
@param lpState the last matching state

@returns the number of states that match
*/
t_uint16 SigCompStateHandler::findState(const SigCompBuffer* partial_identifier, SigCompState** lpState)
{
	t_uint16 count = 0;

	map<t_uint64, SigCompCompartment*>::iterator it1;
	map<SigCompBuffer*, SigCompState*>::iterator it2;
	
	this->lock();

	//
	// Compartments
	//
	for ( it1=this->compartments.begin() ; it1 != this->compartments.end(); it1++ ){
		count+=(*it1).second->findState(partial_identifier, lpState);
	}

	if(count) goto end;

	//
	// Dictionaries
	//
	for ( it2=this->dictionaries.begin() ; it2 != this->dictionaries.end(); it2++ ){
		if((*it2).first->startsWith(partial_identifier)){
			*lpState = (*it2).second;
			count++;
		}
	}
end:
	this->unlock();

	return count;
}

/**
Handle the decompression result. This method will create or free temporary states, set feedbacks, ...

@return void
*/
void SigCompStateHandler::handleResult(lpDecompressionResult &lpResult)
{
	// FIXME: don't use interanl methods

	//
	//	The compressor does not wish (or no longer wishes) to save state information?
	//
	if(lpResult->getReqFeedBack()->getS())
	{
		if(this->compartmentExist(lpResult->getCompartmentId()))
		{
			this->deleteCompartment(lpResult->getCompartmentId());
		}
		return;
	}

	//
	// Find corresponding compartment (only if !S)
	//
	SigCompCompartment* lpCompartment = this->getCompartment(lpResult->getCompartmentId());
	t_uint16 compartment_total_size = lpCompartment->getTotalMemorySize();

compartment_create_states:
	//
	// Request state creation now we have the corresponding compartement
	//
	if( lpResult->getTempStatesToCreateSize() )
	{

		/* Check compartment allocated size*/
		if(!lpCompartment->getTotalMemorySize()){
			goto compartment_free_states;
		}

		// FIXME: lock
		for ( t_uint8 i=0; i<lpResult->getTempStatesToCreateSize(); i++ )
		{
			SigCompState* &lpState = lpResult->getTempStatesToCreate()[i];
			if(!lpState) continue;

			/*If the state creation request needs more state memory than the
			total state_memory_size for the compartment, the state handler
			deletes all but the first (state_memory_size - 64) bytes from the
			state_value.*/
			if( GET_STATE_SIZE(lpState) > compartment_total_size )
			{
				lpCompartment->clearStates();
				t_uint16 oldSize = lpState->getStateValue()->getSize();
				t_uint16 newSize = (compartment_total_size-64);
				lpState->getStateValue()->removeBuff( newSize, (oldSize-newSize) );
				lpState->setStateLength(newSize);

				lpCompartment->addState(lpState);
			}
			/*If the state creation request exceeds the state memory allocated
			to the compartment, sufficient items of state created by the same
			compartment are freed until enough memory is available to
			accommodate the new state.*/
			else
			{
				// FIXME: could infinite loop happen?
				while( lpCompartment->getTotalMemoryLeft() < GET_STATE_SIZE(lpState) )
				{
					lpCompartment->freeStateByPriority();
				}
				lpCompartment->addState(lpState);
			}
		}
	}

compartment_free_states:
	//
	// Request state free now we have the correponding comprtement
	//
	if( lpResult->getTempStatesToFreeSize() )
	{
		lpCompartment->freeStates(lpResult->getTempStatesToFree(), lpResult->getTempStatesToFreeSize());
	}

compartment_remote_params:
	//
	//	Set remote -compressor- parameters
	//
	lpCompartment->setRemoteParams(lpResult->getRemoteParams());

feedbacks:
	//
	//	Set both Returned and Requested feedbacks
	//
	if(lpResult->getReqFeedBack()->getReqFeedItem()->getSize()){
		lpCompartment->setReqFeedback(lpResult->getReqFeedBack()->getReqFeedItem());
	}
	if(lpResult->getRetFeedback()->getSize()){
		lpCompartment->setRetFeedback(lpResult->getRetFeedback());
	}
}

/**
*/
bool SigCompStateHandler::handleNack(const lpstruct_nack_info nack_info)
{
	SigCompBuffer sha_id;
	sha_id.referenceBuff(nack_info->sha1, SHA1HashSize);
	std::map<t_uint64, SigCompCompartment*>::iterator it;
	for ( it = this->compartments.begin() ; it != this->compartments.end(); it++ )
	{
		SigCompCompartment* lpCompartement = (*it).second;
		if(lpCompartement->hasNack(&sha_id))
		{
			// this compartment is responsible for this nack
			switch(nack_info->reasonCode)
			{
			case STATE_NOT_FOUND:
				{
					SigCompState* lpState = NULL;
					lpCompartement->findState(&nack_info->details, &lpState);
					if(lpState)
					{
						lpCompartement->freeState(lpState);
					}
				}
				break;

			default:
				{
					lpCompartement->clearStates();
				}
				break;
			}
		}
	}
	return false;
}

/**
Add Sip/Sdp static dictionary (as defined by RFC 3485)

@returns void
*/
void SigCompStateHandler::addSipSdpDictionary()
{
	this->lock();

	if(!this->hasSipSdpDictionary)
	{
		SigCompBuffer sipdict_id;
		sipdict_id.referenceBuff((t_uint8*)RFC3485_DICTIONARY_SIP_IDENTIFIER, RFC3485_DICTIONARY_SIP_IDENTIFIER_LENGTH);
		if( this->dictionaries.find(&sipdict_id)==this->dictionaries.end() )
		{
			SigCompState* lpSipState = new SigCompSipDictionary();
			this->dictionaries[const_cast<SigCompBuffer*>(lpSipState->getStateIdentifier())] = lpSipState;
		}

		this->hasSipSdpDictionary = true;
	}

	this->unlock();
}

/**
Remove the sip/sdp dictionary (as defined in rfc 3485)

@returns void
*/
void SigCompStateHandler::removeSipSdpDictionary()
{
	this->lock();

	if(this->hasSipSdpDictionary)
	{
		SigCompState* lpSipState = NULL;
		SigCompBuffer sipdict_id;
		sipdict_id.referenceBuff((t_uint8*)RFC3485_DICTIONARY_SIP_IDENTIFIER, RFC3485_DICTIONARY_SIP_IDENTIFIER_LENGTH);
		map<SigCompBuffer*, SigCompState*>::iterator it;
		for ( it=this->dictionaries.begin() ; it != this->dictionaries.end(); it++ ){
			if( *((*it).first) == sipdict_id){
				lpSipState = it->second;	
				break;
			}
		}
		if(lpSipState)
		{
			this->dictionaries.erase(const_cast<SigCompBuffer*>(lpSipState->getStateIdentifier()));
			SAFE_DELETE_PTR(lpSipState);
		}

		this->hasSipSdpDictionary = false;
	}

	this->unlock();
}