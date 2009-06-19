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

#include <libsigcomp/SigCompLayer/SigCompUDVM.h>
#include <libsigcomp/UDVM_INSTRUCTIONS.h>

#define UDVM_HEADER_RESERVED_SIZE 22

/**
SigCompUDVM
RFC3320: [8.  Overview of the UDVM]
*/
SigCompUDVM::SigCompUDVM(const SigCompMessage* _sigCompMessage, const SigCompStateHandler* _stateHandler, lpDecompressionResult _lpResult)
{
	this->isOK = true;
	this->maximum_UDVM_cycles = 0; // RFC3320-Section_8.6
	this->consumed_cycles = 0;
	this->lpResult = _lpResult;
	this->stateHandler = const_cast<SigCompStateHandler*>(_stateHandler);
	this->sigCompMessage = const_cast<SigCompMessage*>(_sigCompMessage);
	
	// RFC 3320 - 7.  SigComp Message Format

	// Alloc UDVM memory
	if(this->sigCompMessage->stream_based){
		this->memory.allocBuff( this->stateHandler->getSigCompParameters()->getDmsValue()/2 );
	}
	else{
		this->memory.allocBuff( (this->stateHandler->getSigCompParameters()->getDmsValue()-this->sigCompMessage->totalSize) );
	}

	//
	// Has feedback with my state id?
	//
	if(this->sigCompMessage->ret_feedback_buffer.getSize())
	{
		t_uint64 size = this->sigCompMessage->ret_feedback_buffer.getSize();
		lpResult->getRetFeedback()->allocBuff(size);
		::memmove(lpResult->getRetFeedback()->getBuffer(), this->sigCompMessage->ret_feedback_buffer.getBuffer(), size);
	}

	//
	//	Has state?
	//
	if(this->sigCompMessage->stateId.getSize())
	{
		// Find the provided state
		SigCompState* lpState = NULL;
		t_uint16 match_count = this->stateHandler->findState(&this->sigCompMessage->stateId, &lpState);
		if( (!match_count || match_count>1 || !lpState)
			|| (lpState->getMinimumAccessLength()>this->sigCompMessage->stateId.getSize())
			|| ((lpState->getStateAddress()+lpState->getStateLength())>this->memory.getSize()) )
		{
			this->createNackInfo(STATE_NOT_FOUND, &this->sigCompMessage->stateId);
			this->isOK = false;
			return;
		}
		//this->sigCompMessage->stateId.print();//FIXME
		//
		// Copy bytecodes to UDVM memory
		//
		if( (lpState->getStateAddress()+lpState->getStateLength())>=this->memory.getSize() )
		{
			this->createNackInfo(SEGFAULT);
			this->isOK = false;
			return;
		}
		::memmove( this->memory.getBuffer(lpState->getStateAddress()), 
			lpState->getStateValue()->getBuffer(), 
			lpState->getStateValue()->getSize());
		
		//RFC 3320 - 7.2.  Accessing Stored State (Useful values)
		UDVM_SET_2BYTES_VAL(UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX, this->sigCompMessage->stateId.getSize());
		UDVM_SET_2BYTES_VAL(UDVM_HEADER_STATE_LENGTH_INDEX, lpState->getStateValue()->getSize());
		
		this->executionPointer = lpState->getStateInstruction();
	}
	else // DON'T HAVE STATE
	{
		//
		// Copy bytecodes to UDVM memory
		//
		size_t bytecodes_destination = this->sigCompMessage->bytecodes_destination;
		if( (bytecodes_destination+this->sigCompMessage->uploaded_UDVM_buffer.getSize())>=this->memory.getSize() )
		{
			this->createNackInfo(BYTECODES_TOO_LARGE);
			this->isOK = false;
			return;
		}
		::memmove( this->memory.getBuffer(bytecodes_destination), 
			this->sigCompMessage->uploaded_UDVM_buffer.getBuffer(), 
			this->sigCompMessage->uploaded_UDVM_buffer.getSize());

		// Set pointer indicating execution index
		this->executionPointer = bytecodes_destination;
	}

	/* RFC 3320-Section_8.6.  UDVM Cycles
	//To ensure that a SigComp message cannot consume excessive processing
	resources, SigComp limits the number of "UDVM cycles" allocated to
	each message.  The number of available UDVM cycles is initialized to
	1000 plus the number of bits in the SigComp header (as described in
	Section 7); this sum is then multiplied by cycles_per_bit.*/
	/*maximum_UDVM_cycles = (8 * n + 1000) * cycles_per_bit*///FIXME:header_size
	this->maximum_UDVM_cycles = ( (8 * this->sigCompMessage->header_size + 1000)* this->stateHandler->getSigCompParameters()->getCpbValue() );
	
	//
	//	RFC 3320 - 7.2.  Useful values
	//
	UDVM_SET_2BYTES_VAL(UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX, this->memory.getSize());
	UDVM_SET_2BYTES_VAL(UDVM_HEADER_CYCLES_PER_BIT_INDEX, this->stateHandler->getSigCompParameters()->getCpbValue());
	UDVM_SET_2BYTES_VAL(UDVM_HEADER_SIGCOMP_VERSION_INDEX, this->stateHandler->getSigCompParameters()->getSigCompVersion());
	memset(this->memory.getBuffer(UDVM_HEADER_RESERVED_INDEX), NULL, UDVM_HEADER_RESERVED_SIZE);
}

/**
~SigCompUDVM
RFC3320: [8.  Overview of the UDVM]
*/
SigCompUDVM::~SigCompUDVM()
{
	//this->memory.freeBuff();
}

/**
runByteCode

@returns
*/
bool SigCompUDVM::runByteCode()
{
	if(!this->isOK) return false;

	t_uint16 operand_1, operand_2, operand_3, operand_4, operand_5, operand_6, operand_7;

	bool excution_failed = false;
	bool end_message = false;

	// LOOP - EXCUTE all bytecode
	while( !excution_failed && !end_message )
	{
		t_uint8 udvm_instruction = * (this->memory.getBuffer(this->executionPointer));
		this->last_memory_address_of_instruction = this->executionPointer;
		this->executionPointer++; // Skip the 1-byte [INSTRUCTION]

		switch(udvm_instruction)
		{
		case UDVM_INST__DECOMPRESSION_FAILURE:
			{
				this->EXEC_INST__DECOMPRESSION_FAILURE();
				excution_failed = true;
				break;
			}

		case UDVM_INST__AND:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__AND(operand_1, operand_2);
				break;
			}

		case UDVM_INST__OR:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__OR(operand_1, operand_2);
				break;
			}

		case UDVM_INST__NOT:
			{
				operand_1 = this->opget_reference_param();
				excution_failed = !this->EXEC_INST__NOT(operand_1);
				break;
			}

		case UDVM_INST__LSHIFT:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__LSHIFT(operand_1, operand_2);
				break;
			}

		case UDVM_INST__RSHIFT:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__RSHIFT(operand_1, operand_2);
				break;
			}

		case UDVM_INST__ADD:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__ADD(operand_1, operand_2);
				break;
			}

		case UDVM_INST__SUBTRACT:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__SUBTRACT(operand_1, operand_2);
				break;
			}

		case UDVM_INST__MULTIPLY:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__MULTIPLY(operand_1, operand_2);
				break;
			}

		case UDVM_INST__DIVIDE:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__DIVIDE(operand_1, operand_2);
				break;
			}

		case UDVM_INST__REMAINDER:
			{
				operand_1 = this->opget_reference_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__REMAINDER(operand_1, operand_2);
				break;
			}

		case UDVM_INST__SORT_ASCENDING:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__SORT_ASCENDING(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__SORT_DESCENDING:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__SORT_DESCENDING(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__SHA_1:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__SHA_1(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__LOAD:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__LOAD(operand_1, operand_2);
				break;
			}

		case UDVM_INST__MULTILOAD:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_literal_param();
				excution_failed = !this->EXEC_INST__MULTILOAD(operand_1, operand_2);
				break;
			}

		case UDVM_INST__PUSH:
			{
				excution_failed = !this->EXEC_INST__PUSH();
				break;
			}

		case UDVM_INST__POP:
			{
				excution_failed = !this->EXEC_INST__POP();
				break;
			}

		case UDVM_INST__COPY:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__COPY(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__COPY_LITERAL:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_reference_param();
				excution_failed = !this->EXEC_INST__COPY_LITERAL(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__COPY_OFFSET:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_reference_param();
				excution_failed = !this->EXEC_INST__COPY_OFFSET(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__MEMSET:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				operand_4 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__MEMSET(operand_1, operand_2, operand_3, operand_4);
				break;
			}

		case UDVM_INST__JUMP:
			{
				excution_failed = !this->EXEC_INST__JUMP();
				break;
			}

		case UDVM_INST__COMPARE:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_address_param(this->last_memory_address_of_instruction);
				operand_4 = this->opget_address_param(this->last_memory_address_of_instruction);
				operand_5 = this->opget_address_param(this->last_memory_address_of_instruction);
				excution_failed = !this->EXEC_INST__COMPARE(operand_1, operand_2, operand_3, operand_4, operand_5);
				break;
			}

		case UDVM_INST__CALL:
			{
				operand_1 = this->opget_address_param(this->last_memory_address_of_instruction);
				excution_failed = !this->EXEC_INST__CALL(operand_1);
				break;
			}

		case UDVM_INST__RETURN:
			{
				excution_failed = !this->EXEC_INST__RETURN();
				break;
			}

		case UDVM_INST__SWITCH:
			{
				operand_1 = this->opget_literal_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__SWITCH(operand_1, operand_2);
				break;
			}

		case UDVM_INST__CRC:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				operand_4 = this->opget_reference_param();
				excution_failed = !this->EXEC_INST__CRC(operand_1, operand_2, operand_3, operand_4);
				break;
			}

		case UDVM_INST__INPUT_BYTES:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_address_param(this->last_memory_address_of_instruction);
				excution_failed = !this->EXEC_INST__INPUT_BYTES(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__INPUT_BITS:
			{	
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_address_param(this->last_memory_address_of_instruction);
				excution_failed = !this->EXEC_INST__INPUT_BITS(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__INPUT_HUFFMAN:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_address_param(this->last_memory_address_of_instruction);
				operand_3 = this->opget_literal_param();
				excution_failed = !this->EXEC_INST__INPUT_HUFFMAN(operand_1, operand_2, operand_3);
				break;
			}

		case UDVM_INST__STATE_ACCESS:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				operand_4 = this->opget_multitype_param();
				operand_5 = this->opget_multitype_param();
				operand_6 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__STATE_ACCESS(operand_1, operand_2, operand_3, operand_4, operand_5, operand_6);
				break;
			}

		case UDVM_INST__STATE_CREATE:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				operand_4 = this->opget_multitype_param();
				operand_5 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__STATE_CREATE(operand_1, operand_2, operand_3, operand_4, operand_5);
				break;
			}

		case UDVM_INST__STATE_FREE:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__STATE_FREE(operand_1, operand_2);
				break;
			}

		case UDVM_INST__OUTPUT:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__OUTPUT(operand_1, operand_2);
				break;
			}

		case UDVM_INST__END_MESSAGE:
			{
				operand_1 = this->opget_multitype_param();
				operand_2 = this->opget_multitype_param();
				operand_3 = this->opget_multitype_param();
				operand_4 = this->opget_multitype_param();
				operand_5 = this->opget_multitype_param();
				operand_6 = this->opget_multitype_param();
				operand_7 = this->opget_multitype_param();
				excution_failed = !this->EXEC_INST__END_MESSAGE(operand_1, operand_2, operand_3, operand_4, operand_5, operand_6, operand_7);
				end_message = true;
				break;
			}

		default:
			this->createNackInfo(INVALID_OPCODE);
			goto bail;
		}
	}

bail:

	// if excution_failed --> GetLastError()
	return (!excution_failed);
}


/**
decompress

@returns
*/
bool SigCompUDVM::decompress()
{
	return this->runByteCode();
}
