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

#define UDVM_MEMORY_REGISTERS_PTR this->memory.getBuffer(UDVM_REGISTERS_START)

/* RFC3320-Setction_8.4.  Byte copying
From UDVM to UDVM
@param destination
@param source
@param size_tocopy
@returns
*/
bool SigCompUDVM::bytecopy_self(t_uint16 &destination, t_uint16 source, t_uint16 size_tocopy)
{
	if( ((destination + size_tocopy) > this->memory.getSize()) || ((source + size_tocopy) > this->memory.getSize()) )
	{
		// SEGFAULT
		assert(0);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/	
	t_uint16 byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	t_uint16 byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);
	
	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*this->memory.getBuffer(destination++) = *this->memory.getBuffer(source++);

		destination = (destination == byte_copy_right)? byte_copy_left : destination;
		source = (source == byte_copy_right)? byte_copy_left : source;
	}

	return true;
}

/* RFC3320-Setction_8.4.  Byte copying
From EXTERNAL to UDVM
@param destination
@param source
@param size_tocopy
@returns
*/
bool SigCompUDVM::bytecopy_to(t_uint16 destination, const t_uint8* source, t_uint16 size_tocopy)
{
	if( ((destination + size_tocopy) > this->memory.getSize()) )
	{
		// SEGFAULT
		assert(0);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/
	t_uint16 byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	t_uint16 byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);

	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*this->memory.getBuffer(destination++) = *(source++);

		destination = (destination == byte_copy_right)? byte_copy_left : destination;
	}

	return true;
}

/* RFC3320-Setction_8.4.  Byte copying
From UDVM to EXTERNAL
@param destination
@param source
@param size_tocopy
@returns
*/
bool SigCompUDVM::bytecopy_from(t_uint8* destination, t_uint16 source, t_uint16 size_tocopy)
{
	if( ((source + size_tocopy) > this->memory.getSize()) )
	{
		// SEGFAULT
		assert(0);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/
	t_uint16 byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	t_uint16 byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);


	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*(destination++) = *this->memory.getBuffer(source++);

		source = (source == byte_copy_right)? byte_copy_left : source;
	}

	return true;
}