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

#include <global_config.h>
#include <libsigcomp/Dictionaries/PresenceDictionaryState.h>
#include <libsigcomp/Dictionaries/rfc5112_dictionary_presence.h>

__NS_DECLARATION_BEGIN__

PresenceDictionaryState::PresenceDictionaryState()
:SigCompState(RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH, RFC5112_DICTIONARY_PRESENCE_STATE_ADDRESS,
			  RFC5112_DICTIONARY_PRESENCE_STATE_INSTRUCTION, RFC5112_DICTIONARY_PRESENCE_MINIMUM_ACCESS_LENGTH, 65535)
{
	this->state_value.allocBuff(RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH);
	::memmove(this->state_value.getBuffer(), RFC5112_DICTIONARY_PRESENCE_VALUE, RFC5112_DICTIONARY_PRESENCE_VALUE_LENGTH);
	this->makeValid();
}

__NS_DECLARATION_END__