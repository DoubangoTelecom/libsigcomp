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
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>

using namespace std;

/*
SigComp Compartment constructor. An application-specific grouping of messages that relate to a peer
      endpoint.
*/
SigCompCompartment::SigCompCompartment(t_uint64 id, t_uint16 sigCompParameters)
:SafeObject(), identifier(id)
{
	this->lpReqFeedback = NULL;
	this->lpRetFeedback = NULL;

	/*+---+---+---+---+---+---+---+---+
      |  cpb  |    dms    |    sms    |
      +---+---+---+---+---+---+---+---+
      |        SigComp_version        |
      +---+---+---+---+---+---+---+---+
	*/

	// Remote parameters
	this->remote_parameters.setCpbCode( (sigCompParameters>>14) );
	this->remote_parameters.setDmsCode( ((sigCompParameters>>11) & 0x07) );
	this->remote_parameters.setSmsCode( ((sigCompParameters>>8) & 0x07) );
	this->remote_parameters.setSigCompVersion( (sigCompParameters & 0x00ff) );	

	// Local parameters
	::memmove(&this->local_parameters, &this->remote_parameters, sizeof(struct_sigcomp_parameters));	

	// Total size
	this->total_memory_size = this->total_memory_left = this->local_parameters.getSmsValue();
}

/**
SigComp compartment destructor
*/
SigCompCompartment::~SigCompCompartment()
{
	// Delete all states
	list<SigCompState* >::iterator it_states;
	SAFE_CLEAR_LIST(this->states, it_states);

	// Delete feedbacks
	SAFE_DELETE_PTR(this->lpReqFeedback);
	SAFE_DELETE_PTR(this->lpRetFeedback);

	// Delete Nacks
	list<SigCompBuffer* >::iterator it_nacks;
	SAFE_CLEAR_LIST(this->nacks, it_nacks);
}

/**
*/
inline t_uint64 SigCompCompartment::getIdentifier()
{
	return this->identifier;
}

/**
*/
t_uint16 SigCompCompartment::getTotalMemorySize()
{
	return this->total_memory_size;
}

/**
*/
t_uint16 SigCompCompartment::getTotalMemoryLeft()
{
	return this->total_memory_left;
}

/**
*/
void SigCompCompartment::setRemoteParams(lpstruct_sigcomp_parameters lpParams)
{
	// cpb+dms+sms [1-byte]
	if(lpParams->hasCpbDmsSms()){
		this->remote_parameters.setCpbCode( lpParams->getCpbCode() );
		this->remote_parameters.setDmsCode( lpParams->getDmsCode() );
		this->remote_parameters.setSmsCode( lpParams->getSmsCode() );
	}
	// SigComp version
	if(lpParams->getSigCompVersion()){
		this->remote_parameters.setSigCompVersion( lpParams->getSigCompVersion() );
	}
	// returned states
	// FIXME: check states about quota
	// FIXME: not tic tac
	// FIXME: what about returned feedback?
	if(lpParams->getReturnedStates()->size()){
		list<SigCompBuffer* >::iterator it;
		SAFE_CLEAR_LIST((*this->remote_parameters.getReturnedStates()), it);
		this->remote_parameters.getReturnedStates()->swap(*lpParams->getReturnedStates());
	}
}

/**
Remove all states and free associated memory space
*/
void SigCompCompartment::clearStates()
{
	this->lock();

	list<SigCompState* >::iterator it;
	SAFE_CLEAR_LIST(this->states, it);

	this->total_memory_left = this->total_memory_size;

	this->unlock();
}

/**
Free one state with the lowest priority
*/
void SigCompCompartment::freeStateByPriority()
{
	this->lock();

	// FIXME: lock
	SigCompState* lpState = NULL;

	/*The order in which the existing state items are freed is
	determined by the state_retention_priority, which is set when the
	state items are created.  The state_retention_priority of 65535 is
	reserved for locally available states; these states must always be
	freed first.  Apart from this special case, states with the lowest
	state_retention_priority are always freed first.  In the event of
	a tie, then the state item created first in the compartment is
	also the first to be freed.*/
	list<SigCompState* >::iterator it_states;
	for ( it_states=this->states.begin(); it_states!=this->states.end(); it_states++ )
	{
		// First --> always ok
		if(it_states==this->states.begin()){
			lpState = *it_states;
			continue;
		}
		// local state?
		if((*it_states)->getStateRetentionPriority() == 65535){
			lpState = *it_states;
			break;
		}
		// lower priority?
		if((*it_states)->getStateRetentionPriority() < lpState->getStateRetentionPriority()){
			lpState = *it_states;
			continue;
		}
	}

	if(lpState){
		this->total_memory_left+= GET_STATE_SIZE(lpState);
		this->states.remove(lpState);
		SAFE_DELETE_PTR(lpState);
	}

	this->unlock();
}

/**
*/
void SigCompCompartment::freeState(SigCompState* &lpState)
{
	this->lock();

	this->total_memory_left+= GET_STATE_SIZE(lpState);
	this->states.remove(lpState);
	SAFE_DELETE_PTR(lpState);

	this->unlock();
}

/**
*/
void SigCompCompartment::freeStates(lptempStateToFreeDesc *tempStates, t_uint8 size)
{
	if(!tempStates || !size)
	{
		return;
	}

	this->lock();
	
	SigCompState* lpState = NULL;
	list<SigCompState* >::iterator it_states;
	for ( t_uint8 i=0; i<size; i++ )
	{
		for ( it_states=this->states.begin(); it_states!=this->states.end(); it_states++ )
		{
			// Compare current state with provided partial state
			if((*it_states)->getStateIdentifier()->startsWith(&tempStates[i]->identifier))
			{
				/*If more than one state item in the compartment matches the partial state identifier, 
				then the state free request is ignored*/
				if(lpState){
					lpState = NULL;
					break;
				}else{
					lpState = *it_states;
				}
			}
		}
		if(lpState){
			this->freeState(lpState);
		}
	}

	this->unlock();
}

/*
findState

@param partial_identifier
@param lpState

@returns
*/
t_uint16 SigCompCompartment::findState(const SigCompBuffer* partial_identifier, SigCompState** lpState)
{
	t_uint16 count = 0;
	
	this->lock();

	list<SigCompState* >::iterator it_states;
	for ( it_states=this->states.begin(); it_states!=this->states.end(); it_states++ ){
		if((*it_states)->getStateIdentifier()->startsWith(partial_identifier)){
			*lpState = *it_states; // override
			count++;
		}
	}

	this->unlock();

	return count;
}

/**
*/
void SigCompCompartment::addState(SigCompState* &lpState)
{
	this->lock();

	lpState->makeValid();
	this->states.push_back(lpState);
	this->total_memory_left-= GET_STATE_SIZE(lpState);
	
	lpState = NULL;

	this->unlock();
}

/**
*/
void SigCompCompartment::setReqFeedback(SigCompBuffer* feedback)
{
	this->lock();

	// Delete old
	SAFE_DELETE_PTR(this->lpReqFeedback);

	this->lpReqFeedback = new SigCompBuffer();

	this->lpReqFeedback->allocBuff(feedback->getSize());
	memmove(this->lpReqFeedback->getBuffer(), feedback->getBuffer(), feedback->getSize());

	this->unlock();
}

/**
*/
void SigCompCompartment::setRetFeedback(SigCompBuffer* feedback)
{
	this->lock();

	// Delete old
	SAFE_DELETE_PTR(this->lpRetFeedback);

	this->lpRetFeedback = new SigCompBuffer();

	this->lpRetFeedback->allocBuff(feedback->getSize());
	memmove(this->lpRetFeedback->getBuffer(), feedback->getBuffer(), feedback->getSize());

	this->unlock();
}

void SigCompCompartment::addNack(const t_uint8 nackId[SHA1HashSize])
{
	this->lock();

	// FIXME: very bad
	if(this->nacks.size() >= NACK_MAX_HISTORY_SIZE)
	{
		std::list<SigCompBuffer*>::iterator it = this->nacks.end(); it--;
		SigCompBuffer* lpBuffer = *it;
		this->nacks.erase(it);
		SAFE_DELETE_PTR(lpBuffer);
	}
	SigCompBuffer* id = new SigCompBuffer();
	id->appendBuff(nackId, SHA1HashSize);
	this->nacks.push_front(id);

	this->unlock();
}

bool SigCompCompartment::hasNack(const SigCompBuffer* nackId)
{
	bool ret = false;

	this->lock();

	list<SigCompBuffer* >::iterator it_nacks;
	for ( it_nacks=this->nacks.begin(); it_nacks!=this->nacks.end(); it_nacks++ ){
		if(**it_nacks == *nackId){
			ret = true;
			break;
		}
	}

	this->unlock();
	return ret;
}