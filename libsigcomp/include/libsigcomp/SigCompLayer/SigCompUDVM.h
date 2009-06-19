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

#if !defined(LIBSIGCOMP_LAYER_UDVM_H)
#define LIBSIGCOMP_LAYER_UDVM_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <libsigcomp/SigCompMessage.h>
#include <libsigcomp/structures/struct_decompResult.h>
#include <libsigcomp/SIGCOMP_HEADERS_INDEX.h>
#include <libsigcomp/NACK_CODES.h>
#include <libsigcomp/SigCompLayer/SigCompStateHandler.h>

#define UDVM_SET_2BYTES_VAL(position, value) BINARY_SET_2BYTES(this->memory.getBuffer(position), value)
#define UDVM_GET_2BYTES_VAL(position) BINARY_GET_2BYTES(this->memory.getBuffer(position))

class SigCompDecompressorDisp;

class SigCompUDVM
{
	friend SigCompDecompressorDisp;

public:
	SigCompUDVM(const SigCompMessage* _sigCompMessage, const SigCompStateHandler* stateHandler,
		lpDecompressionResult lpResult);
	~SigCompUDVM();

private:
	bool runByteCode();
	bool decompress();

private:
	bool isOK;
	SigCompMessage* sigCompMessage;
	SigCompStateHandler* stateHandler;
	lpDecompressionResult lpResult;

	t_uint32 maximum_UDVM_cycles; // RFC3320-Section_8.6
	t_uint64 consumed_cycles;
	
	SigCompBuffer memory;
	//header_udvm_memory memoryHeader; // RFC3320-Section_7.2 - // Default constructor will initial values (sip default)
	
	t_uint16 executionPointer;
	t_uint16 last_memory_address_of_instruction;

	//
	//	Operands
	//
	t_uint16 opget_literal_param();
	t_uint16 opget_reference_param();
	t_uint16 opget_multitype_param();
	t_uint16 opget_address_param(t_uint16 memory_address_of_instruction);

	//
	// ByteCopy
	//
	bool bytecopy_self(t_uint16 &destination, t_uint16 source, t_uint16 size_tocopy);
	bool bytecopy_to(t_uint16 destination, const t_uint8* source, t_uint16 size_tocopy);
	bool bytecopy_from(t_uint8* destination, t_uint16 source, t_uint16 size_tocopy);

	//
	// State Management
	//
	bool byteCopy_TempStates();
	bool createTempState(t_uint16 state_length, t_uint16 state_address, t_uint16 state_instruction, 
		t_uint16 minimum_access_length, t_uint16 state_retention_priority, bool end_msg);

	//
	// Nack creation
	//
	void createNackInfo(t_uint8 reasonCode, SigCompBuffer* lpDetails=NULL);

	//
	// Instructions
	//
	bool EXEC_INST__DECOMPRESSION_FAILURE();
	bool EXEC_INST__AND(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__OR(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__NOT(t_uint16 operand_1);
	bool EXEC_INST__LSHIFT(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__RSHIFT(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__ADD(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__SUBTRACT(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__MULTIPLY(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__DIVIDE(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__REMAINDER(t_uint16 operand_1, t_uint16 operand_2);
	bool EXEC_INST__SORT_ASCENDING(t_uint16 start, t_uint16 n, t_uint16 k);
	bool EXEC_INST__SORT_DESCENDING(t_uint16 start, t_uint16 n, t_uint16 k);
	bool EXEC_INST__SHA_1(t_uint16 position, t_uint16 length, t_uint16 destination);
	bool EXEC_INST__LOAD(t_uint16 address, t_uint16 value);
	bool EXEC_INST__MULTILOAD(t_uint16 address, t_uint16 n);
	bool EXEC_INST__PUSH(t_int16 value = -1);
	bool EXEC_INST__POP(t_uint16* value = NULL);
	bool EXEC_INST__COPY(t_uint16 position, t_uint16 length, t_uint16 destination);
	bool EXEC_INST__COPY_LITERAL(t_uint16 position, t_uint16 length, t_uint16 destination);
	bool EXEC_INST__COPY_OFFSET(t_uint16 offset, t_uint16 length, t_uint16 destination);
	bool EXEC_INST__MEMSET(t_uint16 address, t_uint16 length, t_uint16 start_value, t_uint16 offset);
	bool EXEC_INST__JUMP(t_int16 address=-1);
	bool EXEC_INST__COMPARE(t_uint16 value_1, t_uint16 value_2, t_uint16 address_1, t_uint16 address_2, t_uint16 address_3);
	bool EXEC_INST__CALL(t_uint16 address);
	bool EXEC_INST__RETURN();
	bool EXEC_INST__SWITCH(t_uint16 n, t_uint16 j);
	bool EXEC_INST__CRC(t_uint16 value, t_uint16 position, t_uint16 length, t_uint16 address);
	bool EXEC_INST__INPUT_BYTES(t_uint16 length, t_uint16 destination, t_uint16 address);
	bool EXEC_INST__INPUT_BITS(t_uint16 length, t_uint16 destination, t_uint16 address);
	bool EXEC_INST__INPUT_HUFFMAN(t_uint16 destination, t_uint16 address, t_uint16 n);
	bool EXEC_INST__STATE_ACCESS(t_uint16 partial_identifier_start, t_uint16 partial_identifier_length, t_uint16 state_begin, t_uint16 state_length, t_uint16 state_address, t_uint16 state_instruction);
	bool EXEC_INST__STATE_CREATE(t_uint16 state_length, t_uint16 state_address, t_uint16 state_instruction, t_uint16 minimum_access_length, t_uint16 state_retention_priority);
	bool EXEC_INST__STATE_FREE(t_uint16 partial_identifier_start, t_uint16 partial_identifier_length);
	bool EXEC_INST__OUTPUT(t_uint16 output_start, t_uint16 output_length);
	bool EXEC_INST__END_MESSAGE(t_uint16 requested_feedback_location, t_uint16 returned_parameters_location, t_uint16 state_length, t_uint16 state_address, t_uint16 state_instruction, t_uint16 minimum_access_length, t_uint16 state_retention_priority);
};


#endif // LIBSIGCOMP_LAYER_UDVM_H