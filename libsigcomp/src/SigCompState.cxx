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
#include "SigCompState.h"
#include "rfc3174_us-sha1.h"

//	
// 3.3.3.  Locally Available State Items

__NS_DECLARATION_BEGIN__

/**
*/
SigCompState::SigCompState(uint16_t _state_length, uint16_t _state_address, uint16_t _state_instruction,
							uint16_t _minimum_access_length, uint16_t _state_retention_priority)
:SafeObject()
{
	this->state_length = _state_length;
	this->state_address = _state_address;
	this->state_instruction = _state_instruction;
	this->minimum_access_length = _minimum_access_length;
	this->state_retention_priority = _state_retention_priority;
}

/**
*/
SigCompState::~SigCompState()
{
	this->state_identifier.freeBuff();
}

/**
*/
inline bool SigCompState::operator == (const SigCompState &state) const
{
	return (getStateIdentifier() == state.getStateIdentifier());
}

/**
*/
void SigCompState::makeValid()
{
	this->lock();

	if(/*!this->state_identifier.getSize()*/1)
	{
		SHA1Context sha;

		this->state_identifier.allocBuff(SHA1HashSize);

		/* Calculates a 20-byte SHA-1 hash [RFC-3174] over the
		   byte string formed by concatenating the state_length, state_address,
		   state_instruction, minimum_access_length and state_value (in the
		   order given).  This is the state_identifier.*/
		uint16_t values[4] = { this->state_length, this->state_address,
		   this->state_instruction, this->minimum_access_length };
		
		//
		//	Reset
		//
		int32_t err = ::SHA1Reset(&sha);

		//
		//	[state_length]+[state_address]+[state_instruction]+[minimum_access_length]
		//
		uint8_t firstPart[8];
		for(uint8_t i=0; i<4; i++)
		{
#if 0 /*BIG_ENDIAN*/// Do not change this (it's for my own tests)
			firstPart[i] = (values[i] & 0xff);
			firstPart[i+1] = (values[i] >> 8);
#else
			firstPart[2*i] = (values[i] >> 8);
			firstPart[2*i+1] = (values[i]& 0xff);
#endif
		}
		::SHA1Input(&sha, firstPart, 8);
		
		//
		//	state_value
		//
		::SHA1Input(&sha, this->state_value.getBuffer(), this->state_value.getSize());

		//
		// Compute Result
		//
		err = ::SHA1Result(&sha, this->state_identifier.getBuffer());
		//this->state_identifier.print();
	}
	else
	{
		// FIXME: do not assert
		assert(0);
	}
	this->unlock();
}

/**
*/
inline SigCompBuffer* SigCompState::getStateValue()
{
	return &this->state_value;
}

/**

*/
inline const SigCompBuffer* SigCompState::getStateIdentifier()const
{
	return &this->state_identifier;
}

__NS_DECLARATION_END__
