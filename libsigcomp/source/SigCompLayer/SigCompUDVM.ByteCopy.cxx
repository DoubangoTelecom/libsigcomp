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

#define UDVM_MEMORY_REGISTERS_PTR this->memory.getBuffer(UDVM_REGISTERS_START)

__NS_DECLARATION_BEGIN__

/* RFC3320-Setction_8.4.  Byte copying
From UDVM to UDVM
@param destination
@param source
@param size_tocopy
@returns
*/
bool SigCompUDVM::bytecopy_self(uint16_t &destination, uint16_t source, uint16_t size_tocopy)
{
	//if( ((destination + size_tocopy) > this->memory.getSize()) || ((source + size_tocopy) > this->memory.getSize()) )
	if (destination == this->memory.getSize() || source == this->memory.getSize())
	{
		// SEGFAULT
		this->createNackInfo(SEGFAULT);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/	
	uint16_t byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	uint16_t byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);
	
	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*this->memory.getBuffer(destination++) = *this->memory.getBuffer(source++);
		//assert(destination != byte_copy_right);
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
bool SigCompUDVM::bytecopy_to(uint16_t destination, const uint8_t* source, uint16_t size_tocopy)
{
	//if( ((destination + size_tocopy) > this->memory.getSize()) )
	if(destination == this->memory.getSize())
	{
		// SEGFAULT
		this->createNackInfo(SEGFAULT);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/
	uint16_t byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	uint16_t byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);

	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*this->memory.getBuffer(destination++) = *(source++);

		//assert(destination != byte_copy_right);
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
bool SigCompUDVM::bytecopy_from(uint8_t* destination, uint16_t source, uint16_t size_tocopy)
{
	//if( ((source + size_tocopy) > this->memory.getSize()) )
	if(source == this->memory.getSize())
	{
		// SEGFAULT
		this->createNackInfo(SEGFAULT);
		return false;
	}

	/*The string of bytes is copied in ascending order of memory address,
	respecting the bounds set by byte_copy_left and byte_copy_right.*/
	uint16_t byte_copy_left = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	uint16_t byte_copy_right = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);


	// string of bytes is copied one byte at a time
	while((size_tocopy--))
	{
		*(destination++) = *this->memory.getBuffer(source++);

		source = (source == byte_copy_right)? byte_copy_left : source;
	}

	return true;
}

__NS_DECLARATION_END__