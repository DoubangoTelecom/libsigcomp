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

#if !defined(LIBSIGCOMP_LAYER_STATE_H)
#define LIBSIGCOMP_LAYER_STATE_H

#include "libsigcomp_config.h"

#include "MYTYPES.h"

#include "SigCompBuffer.h"
#include "rfc3174_us-sha1.h"
#include "SafeObject.h"

// FIXME:
#define PARTIAL_ID_LEN_CODE		0x01
#define PARTIAL_ID_LEN_VALUE	0x06

/*For the purpose of calculation, each state item
   is considered to cost (state_length + 64) bytes.*/
#define GET_STATE_SIZE(lpState) ( lpState->getStateLength() + 64 )

__NS_DECLARATION_BEGIN__

class LIBSIGCOMP_API SigCompState : public SafeObject
{
public:
	SigCompState(uint16_t state_length, uint16_t state_address, uint16_t state_instruction,
							uint16_t minimum_access_length, uint16_t state_retention_priority);
	~SigCompState();

	INLINE bool operator == (const SigCompState &state) const;

	void makeValid();
	INLINE SigCompBuffer* getStateValue();
	INLINE const SigCompBuffer* getStateIdentifier()const;

	INLINE const uint16_t getStateLength()const { return this->state_length; }
	INLINE const void setStateLength(uint16_t length) { this->state_length = length; }
	INLINE const uint16_t getStateAddress()const { return this->state_address; }
	INLINE const uint16_t getStateInstruction()const { return this->state_instruction; }
	INLINE const uint16_t getMinimumAccessLength()const { return this->minimum_access_length; }
	INLINE const uint16_t getStateRetentionPriority()const { return this->state_retention_priority; }

protected:
	SigCompBuffer state_value;
	SigCompBuffer state_identifier;

	uint16_t state_length; 
	uint16_t state_address; 
	uint16_t state_instruction;
	uint16_t minimum_access_length;
	uint16_t state_retention_priority;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_LAYER_STATE_H
