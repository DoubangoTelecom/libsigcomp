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

#if !defined(LIBSIGCOMP_LAYER_UDVM_H)
#define LIBSIGCOMP_LAYER_UDVM_H

#include "libsigcomp_config.h"

#include "MYTYPES.h"
#include "SigCompMessage.h"
#include "struct_decompResult.h"
#include "SIGCOMP_HEADERS_INDEX.h"
#include "NACK_CODES.h"
#include "SigCompStateHandler.h"

#define UDVM_SET_2BYTES_VAL(position, value) BINARY_SET_2BYTES(this->memory.getBuffer(position), value)
#define UDVM_GET_2BYTES_VAL(position) BINARY_GET_2BYTES(this->memory.getBuffer(position))

__NS_DECLARATION_BEGIN__

class SigCompDecompressorDisp;

class SigCompUDVM
{
	friend class SigCompDecompressorDisp;

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

	uint32_t maximum_UDVM_cycles; // RFC3320-Section_8.6
	uint64_t consumed_cycles;
	
	SigCompBuffer memory;
	//header_udvm_memory memoryHeader; // RFC3320-Section_7.2 - // Default constructor will initial values (sip default)
	
	uint16_t executionPointer;
	uint16_t last_memory_address_of_instruction;

	//
	//	Operands
	//
	uint16_t opget_literal_param();
	uint16_t opget_reference_param();
	uint16_t opget_multitype_param();
	uint16_t opget_address_param(uint16_t memory_address_of_instruction);

	//
	// ByteCopy
	//
	bool bytecopy_self(uint16_t &destination, uint16_t source, uint16_t size_tocopy);
	bool bytecopy_to(uint16_t destination, const uint8_t* source, uint16_t size_tocopy);
	bool bytecopy_from(uint8_t* destination, uint16_t source, uint16_t size_tocopy);

	//
	// State Management
	//
	bool byteCopy_TempStates();
	bool createTempState(uint16_t state_length, uint16_t state_address, uint16_t state_instruction, 
		uint16_t minimum_access_length, uint16_t state_retention_priority, bool end_msg);

	//
	// Nack creation
	//
	void createNackInfo(uint8_t reasonCode, SigCompBuffer* lpDetails=NULL, int16_t memory_address_of_instruction = -1);

	//
	// Instructions
	//
	bool EXEC_INST__DECOMPRESSION_FAILURE();
	bool EXEC_INST__AND(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__OR(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__NOT(uint16_t operand_1);
	bool EXEC_INST__LSHIFT(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__RSHIFT(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__ADD(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__SUBTRACT(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__MULTIPLY(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__DIVIDE(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__REMAINDER(uint16_t operand_1, uint16_t operand_2);
	bool EXEC_INST__SORT_ASCENDING(uint16_t start, uint16_t n, uint16_t k);
	bool EXEC_INST__SORT_DESCENDING(uint16_t start, uint16_t n, uint16_t k);
	bool EXEC_INST__SHA_1(uint16_t position, uint16_t length, uint16_t destination);
	bool EXEC_INST__LOAD(uint16_t address, uint16_t value);
	bool EXEC_INST__MULTILOAD(uint16_t address, uint16_t n);
	bool EXEC_INST__PUSH(int16_t value = -1);
	bool EXEC_INST__POP(uint16_t* value = NULL);
	bool EXEC_INST__COPY(uint16_t position, uint16_t length, uint16_t destination);
	bool EXEC_INST__COPY_LITERAL(uint16_t position, uint16_t length, uint16_t destination);
	bool EXEC_INST__COPY_OFFSET(uint16_t offset, uint16_t length, uint16_t destination);
	bool EXEC_INST__MEMSET(uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset);
	bool EXEC_INST__JUMP(int16_t address=-1);
	bool EXEC_INST__COMPARE(uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3);
	bool EXEC_INST__CALL(uint16_t address);
	bool EXEC_INST__RETURN();
	bool EXEC_INST__SWITCH(uint16_t n, uint16_t j);
	bool EXEC_INST__CRC(uint16_t value, uint16_t position, uint16_t length, uint16_t address);
	bool EXEC_INST__INPUT_BYTES(uint16_t length, uint16_t destination, uint16_t address);
	bool EXEC_INST__INPUT_BITS(uint16_t length, uint16_t destination, uint16_t address);
	bool EXEC_INST__INPUT_HUFFMAN(uint16_t destination, uint16_t address, uint16_t n);
	bool EXEC_INST__STATE_ACCESS(uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction);
	bool EXEC_INST__STATE_CREATE(uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority);
	bool EXEC_INST__STATE_FREE(uint16_t partial_identifier_start, uint16_t partial_identifier_length);
	bool EXEC_INST__OUTPUT(uint16_t output_start, uint16_t output_length);
	bool EXEC_INST__END_MESSAGE(uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority);
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_LAYER_UDVM_H
