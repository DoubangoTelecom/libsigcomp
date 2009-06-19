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

#if !defined(LIBSIGCOMP_STRUCTNACKINFO_H)
#define LIBSIGCOMP_STRUCTNACKINFO_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/MYTYPES.h>

/*
+---+---+---+---+---+---+---+---+
|         code_len = 0          |
+---+---+---+---+---+---+---+---+
| code_len = 0  |  version = 1  |
+---+---+---+---+---+---+---+---+
|          Reason Code          |
+---+---+---+---+---+---+---+---+
|  OPCODE of failed instruction |
+---+---+---+---+---+---+---+---+
|   PC of failed instruction    |
|                               |
+---+---+---+---+---+---+---+---+
|                               |
: SHA-1 Hash of failed message  :
|                               |
+---+---+---+---+---+---+---+---+
|                               |
:         Error Details         :
|                               |
+---+---+---+---+---+---+---+---+
*/
typedef struct rfc4077_struct_nack_info
{
	rfc4077_struct_nack_info()
	{
		version = NACK_VERSION;
		reasonCode = opcode = pc = 0;
	}

	/*gives the version of the NACK mechanism being employed.
      This document defines version 1*/
	t_uint8 version;
	/*The Reason Code is a one-byte value that indicates the nature of the
      decompression failure.*/
	t_uint8 reasonCode;
	/*The OPCODE of failed instruction" is a one-byte field that includes
      the opcode to which the PC was pointing when the failure occurred*/
	t_uint8 opcode;
	/*"PC of failed instruction" is a two-byte field containing the
      value of the program counter when failure occurred (i.e., the
      memory address of the failed UDVM instruction)*/
	t_uint16 pc;
	/*"SHA-1 Hash of failed message" contains the full 20-byte SHA-1
      hash of the SigComp message that could not be decompressed*/
	t_uint8 sha1[20];
	/*"Error Details" provides additional information that might be
      useful in correcting the problem that caused decompression failure.*/
	SigCompBuffer details;
}
struct_nack_info, *lpstruct_nack_info;

#endif // LIBSIGCOMP_STRUCTNACKINFO_H