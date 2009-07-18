/**
	Copyright (C) 2009 Mamadou Diop <diopmamadou@yahoo.fr>

	This file is part of libSigComp project.

    libSigComp is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    libSigComp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU Lesser General Public License
    along with libSigComp.  

	
*/

#include "global_config.h"

#include "SigCompUDVM.h"

__NS_DECLARATION_BEGIN__

/*
@returns
*/
bool SigCompUDVM::byteCopy_TempStates()
{
	bool ok = true;
	//
	// State Create
	//
	for(uint8_t i=0; i<this->lpResult->getTempStatesToCreateSize(); i++)
	{
		/*The UDVM byte copies a string of state_length bytes from the UDVM
		memory beginning at state_address (obeying the rules of Section 8.4).
		This is the state_value.*/
		SigCompState* lpState = this->lpResult->getTempStatesToCreate()[i];
		lpState->getStateValue()->allocBuff(lpState->getStateLength());
		ok &=this->bytecopy_from(lpState->getStateValue()->getBuffer(0), lpState->getStateAddress(), lpState->getStateLength());
	}
	
	//
	// State Free
	//
	for(uint8_t i=0; i<this->lpResult->getTempStatesToFreeSize(); i++)
	{
		lptempStateToFreeDesc lpDesc = this->lpResult->getTempStatesToFree()[i];
		lpDesc->identifier.allocBuff(lpDesc->partial_identifier_length);
		ok &=this->bytecopy_from(lpDesc->identifier.getBuffer(), lpDesc->partial_identifier_start, lpDesc->partial_identifier_length);
	}
	return ok;
}

bool SigCompUDVM::createTempState(uint16_t state_length, uint16_t state_address, uint16_t state_instruction, 
								  uint16_t minimum_access_length, uint16_t state_retention_priority, bool end_msg)
{
	/*If the specified minimum_access_length does not lie between 6 and 20 inclusive, or if
	the state_retention_priority is 65535 then the END-MESSAGE
	instruction fails to make a state creation request of its own
	(however decompression failure does not occur and the state creation
	requests made by the STATE-CREATE instruction are still valid).*/
	bool is_ok = ( (6<=minimum_access_length && minimum_access_length<=20) && state_retention_priority!=65535 );

	// if not ok and not END_MESSAGE --> decompression failure MUST occurs
	if(!is_ok)
	{
		if(end_msg) return true;
		if(state_retention_priority==65535){
			this->createNackInfo(INVALID_STATE_PRIORITY);
		}else{
			this->createNackInfo(INVALID_STATE_ID_LENGTH);
		}
		return false;
	}

	// Already exist?
	/*if(this->lpResult->exitTempState(state_length, state_address, state_instruction, 
								  minimum_access_length, state_retention_priority)){
		return true;
	}*/

	/*decompression failure occurs if the END-MESSAGE instruction makes a state creation request and four
	instances of the STATE-CREATE instruction have already been encountered.*/
	if(this->lpResult->getTempStatesToCreateSize() >=4)
	{
		this->createNackInfo(TOO_MANY_STATE_REQUESTS);
		return false;
	}

	//
	//	Is there a state to create?
	//
	if(is_ok)
	{
		// no byte copy ()
		SigCompState* lpState = new SigCompState(state_length, state_address, state_instruction, minimum_access_length,
			state_retention_priority);
		this->lpResult->addTempStateToCreate(lpState);
	}

	return true;
}

__NS_DECLARATION_END__
