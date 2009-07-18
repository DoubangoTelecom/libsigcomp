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

#include "global_config.h"
#include "DummyCompressor.h"

__NS_DECLARATION_BEGIN__

// RFC 4896 [11.  Uncompressed Bytecode]
#define UNCOMPRESSED_BYTECODE_LENGTH			13
#define UNCOMPRESSED_BYTECODE_DESTINATION_CODE	0x01 // 128
const char* DummyCompressor::uncompressed_bytecode =
{
	"\xf8\x00\xa1\x1c\x01\x86\x09\x22\x86\x01\x16\xf9\x23"
};

/**
	This compressor will let the message uncompressed (see RFC 4896 para 11)
*/
DummyCompressor::DummyCompressor()
{
}

/**
*/
DummyCompressor::~DummyCompressor()
{
}

/**
*/
bool DummyCompressor::compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream)
{
	SigCompBuffer output_buffer;
	output_buffer.referenceBuff((uint8_t*)output_ptr, output_size);

	size_t pointer =0;

	uint8_t* header = output_buffer.getBuffer(pointer++);

	/* SigComp Header */
	if(lpCompartment->getReqFeedback() && lpCompartment->getReqFeedback()->getSize()){
		// Return the requested feedback
		*header = 0xfc; // T=1
		::memmove(output_buffer.getBuffer(pointer), lpCompartment->getReqFeedback()->getBuffer(), lpCompartment->getReqFeedback()->getSize());
		pointer+=lpCompartment->getReqFeedback()->getSize();
	}
	else{
		*header = 0xf8;
	}
	uint16_t codeLen = UNCOMPRESSED_BYTECODE_LENGTH;
	// first byte for codelen
	*output_buffer.getBuffer(pointer++) = ((codeLen>>4)& 0x00ff);
	// last 4 bits for codelen
	*output_buffer.getBuffer(pointer) = ((codeLen & 0x000f)<<4); 
	// first and last 4 bits for destination
	*output_buffer.getBuffer(pointer++) |= UNCOMPRESSED_BYTECODE_DESTINATION_CODE;

	//////////////////////////////////////////////////
	//	Upload UDVM bytecode
	//
	::memmove(output_buffer.getBuffer(pointer), (uint8_t*)DummyCompressor::uncompressed_bytecode, codeLen);
	pointer+= codeLen;

	//***********************************************
	//	Copy data uncompressed
	//
	::memmove(output_buffer.getBuffer(pointer), input_ptr, input_size);
	pointer += input_size;
	output_size = (pointer);

	return true;
}

__NS_DECLARATION_END__
