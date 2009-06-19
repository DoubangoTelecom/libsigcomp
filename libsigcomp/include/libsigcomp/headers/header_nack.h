///**
//	Copyright (C) 2009 Mamadou Diop <diopmamadou@yahoo.fr>
//
//	This file is part of libSigComp project.
//
//    libSigComp is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//	
//    libSigComp is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//	
//    You should have received a copy of the GNU General Public License
//    along with libSigComp.  If not, see <http://www.gnu.org/licenses/>.
//
//	For Commercial Use or non-GPL Licensing please contact me at <diopmamadou@yahoo.fr>
//*/
//
//#if !defined(LIBSIGCOMP_NACK_H)
//#define LIBSIGCOMP_NACK_H
//
//#include <libsigcomp/mytypes.h>
//#include <string.h>
//
//// RFC_4077-Section_3.2 [Reason Codes]
//static const std::string nack_reason_codes[]
//= {
//	"SUCCESS"					   ,//0
//	"STATE_NOT_FOUND"              ,//1  State ID (6 - 20 bytes)
//	"CYCLES_EXHAUSTED"             ,//2  Cycles Per Bit (1 byte)
//	"USER_REQUESTED"               ,//3
//	"SEGFAULT"                     ,//4
//	"TOO_MANY_STATE_REQUESTS"      ,//5
//	"INVALID_STATE_ID_LENGTH"      ,//6
//	"INVALID_STATE_PRIORITY"       ,//7
//	"OUTPUT_OVERFLOW"              ,//8
//	"STACK_UNDERFLOW"              ,//9
//	"BAD_INPUT_BITORDER"           ,//10
//	"DIV_BY_ZERO"                  ,//11
//	"SWITCH_VALUE_TOO_HIGH"        ,//12
//	"TOO_MANY_BITS_REQUESTED"      ,//13
//	"INVALID_OPERAND"              ,//14
//	"HUFFMAN_NO_MATCH"             ,//15
//	"MESSAGE_TOO_SHORT"            ,//16
//	"INVALID_CODE_LOCATION"        ,//17
//	"BYTECODES_TOO_LARGE"          ,//18  Memory size (2 bytes)
//	"INVALID_OPCODE"               ,//19
//	"INVALID_STATE_PROBE"          ,//20
//	"ID_NOT_UNIQUE"                ,//21  State ID (6 - 20 bytes)
//	"MULTILOAD_OVERWRITTEN"        ,//22
//	"STATE_TOO_SHORT"              ,//23  State ID (6 - 20 bytes)
//	"INTERNAL_ERROR"               ,//24
//	"FRAMING_ERROR"                //25
//};
//
//#endif // LIBSIGCOMP_NACK_H