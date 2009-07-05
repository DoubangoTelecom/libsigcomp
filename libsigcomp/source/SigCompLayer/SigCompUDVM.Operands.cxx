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

#include <libsigcomp/SigCompLayer/SigCompUDVM.h>

#include <math.h>

__NS_DECLARATION_BEGIN__
///////////////////////////////////////////////////
//
//
//
//	RFC3320-Section-8.5.  Instruction operands and UDVM bytecode
//


/***literal (#)
Bytecode:                       Operand value:      Range:
0nnnnnnn                        N                   0 - 127
10nnnnnn nnnnnnnn               N                   0 - 16383
11000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
*/
uint16_t SigCompUDVM::opget_literal_param()
{
	const uint8_t* memory_ptr = this->memory.getBuffer(this->executionPointer);
	uint16_t result = 0;

	switch( *memory_ptr & 0xc0) // 2 first bits
	{
	case 0x00: // 0nnnnnnn                        N                   0 - 127
	case 0x40: // 0nnnnnnn                        N                   0 - 127
		{
			result = *(memory_ptr);
			this->executionPointer++;
		}
		break;

	case 0x80: // 10nnnnnn nnnnnnnn               N                   0 - 16383
		{
			result = BINARY_GET_2BYTES(memory_ptr)&0x3fff; // All except 2 first bits
			this->executionPointer+=2;
		}
		break;
	
	case 0xc0: // 11000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
		{
			result = BINARY_GET_2BYTES((memory_ptr+1));
			this->executionPointer+=3;
		}
		break;

	default:
		{
			// NEVER
		}
		break;
	}
	return result;
}

/***reference ($)
Bytecode:                       Operand value:      Range:

0nnnnnnn                        memory[2 * N]       0 - 65535
10nnnnnn nnnnnnnn               memory[2 * N]       0 - 65535
11000000 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
*/
uint16_t SigCompUDVM::opget_reference_param()
{
	const uint8_t* memory_ptr = this->memory.getBuffer(this->executionPointer);
	uint16_t result = 0;
	
	switch( *memory_ptr & 0xc0) // 2 first bits
	{
	case 0x00: // 0nnnnnnn                        memory[2 * N]       0 - 65535
	case 0x40: // 0nnnnnnn                        memory[2 * N]       0 - 65535
		{
			uint8_t N = (*(memory_ptr) & 0x7f); // no effect first bit is already nil
			result = 2*N;
			this->executionPointer++;
		}
		break;

	case 0x80: // 10nnnnnn nnnnnnnn               memory[2 * N]       0 - 65535
		{
			uint8_t N = (BINARY_GET_2BYTES(memory_ptr) & 0x3fff);
			result = 2*N;
			this->executionPointer+=2;
		}
		break;
	
	case 0xc0: // 11000000 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
		{
			uint16_t N = BINARY_GET_2BYTES(memory_ptr+1);
			result = N;
			this->executionPointer+=3;
		}
		break;

	default:
		{
			this->createNackInfo(INVALID_OPERAND);
		}
		break;
	}

	return result;
}

/*** multitype(%)
Bytecode:                       Operand value:      Range:

00nnnnnn                        N                   0 - 63
01nnnnnn                        memory[2 * N]       0 - 65535
1000011n                        2 ^ (N + 6)        64 , 128
10001nnn                        2 ^ (N + 8)    256 , ... , 32768
111nnnnn                        N + 65504       65504 - 65535
1001nnnn nnnnnnnn               N + 61440       61440 - 65535
101nnnnn nnnnnnnn               N                   0 - 8191
110nnnnn nnnnnnnn               memory[N]           0 - 65535
10000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
10000001 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
*/
uint16_t SigCompUDVM::opget_multitype_param()
{
	const uint8_t* memory_ptr = this->memory.getBuffer(this->executionPointer);
	int8_t index = operand_multitype_indexes[*memory_ptr];
	uint16_t result = 0;

	switch(index)
	{
	case 1: // 00nnnnnn                        N                   0 - 63
		{
			result = *(memory_ptr);
			this->executionPointer++;
		}
		break;

	case 2: // 01nnnnnn                        memory[2 * N]       0 - 65535
		{
			uint8_t N = (*(memory_ptr) & 0x3f);
			result = BINARY_GET_2BYTES( this->memory.getBuffer(2*N) );
			this->executionPointer++;
		}
		break;

	case 3: // 1000011n                        2 ^ (N + 6)        64 , 128
		{
			uint8_t N = (*(memory_ptr) & 0x01);
			result = pow( (double)2, (N + 6) );
			this->executionPointer++;
		}
		break;

	case 4: // 10001nnn                        2 ^ (N + 8)    256 , ... , 32768
		{
			uint8_t N = (*(memory_ptr) & 0x07);
			result = pow( (double)2, (N + 8) );
			this->executionPointer++;
		}
		break;

	case 5: // 111nnnnn                        N + 65504       65504 - 65535
		{
			result = ((*(memory_ptr) & 0x1f) + 65504 );
			this->executionPointer++;
		}
		break;

	case 6: // 1001nnnn nnnnnnnn               N + 61440       61440 - 65535
		{
			result = (BINARY_GET_2BYTES(memory_ptr) & 0x0fff) + 61440;
			this->executionPointer+=2;
		}
		break;

	case 7: // 101nnnnn nnnnnnnn               N                   0 - 8191
		{
			result = (BINARY_GET_2BYTES(memory_ptr) & 0x1fff);
			this->executionPointer+=2;
		}
		break;

	case 8: // 110nnnnn nnnnnnnn               memory[N]           0 - 65535
		{
			uint16_t N = BINARY_GET_2BYTES(memory_ptr) & 0x1fff;
			result = BINARY_GET_2BYTES( this->memory.getBuffer(N) );
			this->executionPointer+=2;
		}
		break;

	case 9: // 10000000 nnnnnnnn nnnnnnnn      N                   0 - 65535
		{
			result = BINARY_GET_2BYTES(memory_ptr+1);
			this->executionPointer+=3;
		}
		break;

	case 10: // 10000001 nnnnnnnn nnnnnnnn      memory[N]           0 - 65535
		{
			uint16_t N = BINARY_GET_2BYTES(memory_ptr+1);
			result = BINARY_GET_2BYTES( this->memory.getBuffer(N) );
			this->executionPointer+=3;
		}
		break;

		default: // -1
		{
			// FIXME: SEGFAULT/INVALID_OPERAND
			assert(0);
		}
		break;
	}

	return result;
}

/*** address(@)
This operand is decoded as a multitype operand followed by a further step: the memory address
of the UDVM instruction containing the address operand is added to
obtain the correct operand value.  So if the operand value from
Figure 10 is D then the actual operand value of an address is
calculated as follows:

   operand_value = (memory_address_of_instruction + D) modulo 2^16
*/
uint16_t SigCompUDVM::opget_address_param(uint16_t memory_address_of_instruction)
{
	uint16_t D = this->opget_multitype_param(); 
	// (2^16) => 65536;
	return ( (memory_address_of_instruction + D)%65536 );
}


__NS_DECLARATION_END__