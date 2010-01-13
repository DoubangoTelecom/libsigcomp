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

#if !defined(LIBSIGCOMP_SIGCOMP_MESSAGE_H)
#define LIBSIGCOMP_SIGCOMP_MESSAGE_H

#include "libsigcomp_config.h"

#include "MYTYPES.h"

#include "binary_utils.h"
#include "SigCompBuffer.h"
#include "struct_nack_info.h"

__NS_DECLARATION_BEGIN__

class SigCompUDVM;

class SigCompMessage
{
	friend class SigCompUDVM;

public:
	SigCompMessage(LPCVOID input_ptr, size_t input_size, bool stream);
	~SigCompMessage();

	INLINE bool getIsOK() { return this->isOK; }
	INLINE bool getIsNack() { return this->isNack; }
	INLINE const lpstruct_nack_info getNackInfo() { return &this->nack_info; }

private:
	void initFeedbackItem(uint8_t* &start_ptr);
	void initStateId(uint8_t* &start_ptr, uint8_t state_len);
	void initStateful(uint8_t* &start_ptr, uint8_t* end_ptr);
	void initStateless(uint8_t* &start_ptr, uint8_t* end_ptr);
	void initNack(uint8_t* &start_ptr, uint8_t* end_ptr);

private:
	uint8_t headerSigComp;

	bool isOK;
	bool isNack;
	bool stream_based;
	const uint8_t* startPtr;
	size_t totalSize;
	size_t header_size;

	SigCompBuffer stateId;

	uint16_t bytecodes_destination;
	SigCompBuffer remaining_sigcomp_buffer;
	SigCompBuffer uploaded_UDVM_buffer;
	SigCompBuffer ret_feedback_buffer;

	struct_nack_info nack_info;
};

__NS_DECLARATION_END__

#endif //LIBSIGCOMP_SIGCOMP_MESSAGE_H
