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

#if !defined(LIBSIGCOMP_LAYER_STATE_H)
#define LIBSIGCOMP_LAYER_STATE_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/MYTYPES.h>

#include <libsigcomp/SigCompBuffer.h>
#include <libsigcomp/rfc3174_us-sha1.h>
#include <libsigcomp/rfc3485_dictionary_sip.h>
#include <libsigcomp/SafeObject.h>

/*For the purpose of calculation, each state item
   is considered to cost (state_length + 64) bytes.*/
#define GET_STATE_SIZE(lpState) (lpState->getStateLength()+64)

class LIBSIGCOMP_API SigCompState : public SafeObject
{
public:
	SigCompState(t_uint16 state_length, t_uint16 state_address, t_uint16 state_instruction,
							t_uint16 minimum_access_length, t_uint16 state_retention_priority);
	~SigCompState();

	inline bool operator == (const SigCompState &state) const;

	void makeValid();
	inline SigCompBuffer* getStateValue();
	inline const SigCompBuffer* getStateIdentifier()const;

	inline const t_uint16 getStateLength()const { return this->state_length; }
	inline const void setStateLength(t_uint16 length) { this->state_length = length; }
	inline const t_uint16 getStateAddress()const { return this->state_address; }
	inline const t_uint16 getStateInstruction()const { return this->state_instruction; }
	inline const t_uint16 getMinimumAccessLength()const { return this->minimum_access_length; }
	inline const t_uint16 getStateRetentionPriority()const { return this->state_retention_priority; }

protected:
	SigCompBuffer state_value;
	SigCompBuffer state_identifier;

	t_uint16 state_length; 
	t_uint16 state_address; 
	t_uint16 state_instruction;
	t_uint16 minimum_access_length;
	t_uint16 state_retention_priority;
};


//
//
//
class LIBSIGCOMP_API SigCompSipDictionary : public SigCompState
{
public:
	SigCompSipDictionary();
};

#endif // LIBSIGCOMP_LAYER_STATE_H