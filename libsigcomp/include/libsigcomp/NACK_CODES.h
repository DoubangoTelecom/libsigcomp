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

#if !defined(LIBSIGCOMP_NACK_CODES_H)
#define LIBSIGCOMP_NACK_CODES_H

//
//	RFC 4077 Section 3.2
//
#define STATE_NOT_FOUND					1  // State ID (6 - 20 bytes)
#define CYCLES_EXHAUSTED			    2  // Cycles Per Bit (1 byte)
#define USER_REQUESTED					3
#define SEGFAULT						4
#define TOO_MANY_STATE_REQUESTS			5
#define INVALID_STATE_ID_LENGTH			6
#define INVALID_STATE_PRIORITY			7
#define OUTPUT_OVERFLOW					8
#define STACK_UNDERFLOW					9
#define BAD_INPUT_BITORDER				10
#define DIV_BY_ZERO						11
#define SWITCH_VALUE_TOO_HIGH			12
#define TOO_MANY_BITS_REQUESTED			13
#define INVALID_OPERAND					14
#define HUFFMAN_NO_MATCH				15
#define MESSAGE_TOO_SHORT				16
#define INVALID_CODE_LOCATION			17
#define BYTECODES_TOO_LARGE				18  // Memory size (2 bytes)
#define INVALID_OPCODE					19
#define INVALID_STATE_PROBE				20
#define ID_NOT_UNIQUE					21  // State ID (6 - 20 bytes)
#define MULTILOAD_OVERWRITTEN			22
#define STATE_TOO_SHORT					23  // State ID (6 - 20 bytes)
#define INTERNAL_ERROR					24
#define FRAMING_ERROR					25

#endif // LIBSIGCOMP_NACK_CODES_H