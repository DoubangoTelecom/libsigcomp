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

#if !defined(LIBSIGCOMP_SIGCOMP_MESSAGE_H)
#define LIBSIGCOMP_SIGCOMP_MESSAGE_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/binary_utils.h>
#include <libsigcomp/SigCompBuffer.h>
#include <libsigcomp/structures/struct_nack_info.h>

class SigCompUDVM;

class SigCompMessage
{
	friend class SigCompUDVM;

public:
	SigCompMessage(LPCVOID input_ptr, size_t input_size, bool stream);
	~SigCompMessage();

	inline bool getIsOK() { return this->isOK; }
	inline bool getIsNack() { return this->isNack; }
	inline const lpstruct_nack_info getNackInfo() { return &this->nack_info; }

private:
	void initFeedbackItem(t_uint8* &start_ptr);
	void initStateId(t_uint8* &start_ptr, t_uint8 state_len);
	void initStateful(t_uint8* &start_ptr, t_uint8* end_ptr);
	void initStateless(t_uint8* &start_ptr, t_uint8* end_ptr);
	void initNack(t_uint8* &start_ptr, t_uint8* end_ptr);

private:
	t_uint8 headerSigComp;

	bool isOK;
	bool isNack;
	bool stream_based;
	const t_uint8* startPtr;
	size_t totalSize;
	size_t header_size;

	SigCompBuffer stateId;

	t_uint16 bytecodes_destination;
	SigCompBuffer remaining_sigcomp_buffer;
	SigCompBuffer uploaded_UDVM_buffer;
	SigCompBuffer ret_feedback_buffer;

	struct_nack_info nack_info;
};

#endif //LIBSIGCOMP_SIGCOMP_MESSAGE_H