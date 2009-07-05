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
#include <libsigcomp/Compressors/DeflateCompressor.h>
//#include <math.h>
#define LIBSIGCOMP_MIN(a, b) (a<b?a:b)

// FIXME: do not compile these two files when using carbide.c++
#ifdef __SYMBIAN32__
//#	include <../source/Compressors/DeflateCompressor.ghost.cxx>
//#	include <../source/Compressors/DeflateCompressor.ghost.cxx>
#endif

__NS_DECLARATION_BEGIN__

// see this file for more info: 'libsigcomp/asm/deflate.asm'
const char* DeflateCompressor::deflate_bytecode =
{
#if USE_DICTS_FOR_COMPRESSION
	"\x0F\x86\x7A\xA2\xBD\x8D\x05\xA2\xBD\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x01\x0B\x01\x0D\x01\x0F\x01\x11\x02\x13\x02\x17\x02\x1B\x02\x1F\x03\x23\x03\x2B\x03\x33\x03\x3B\x04\xA0\x43\x04\xA0\x53\x04\xA0\x63\x04\xA0\x73\x05\xA0\x83\x05\xA0\xA3\x05\xA0\xC3\x05\xA0\xE3\x00\xA1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07\x02\x09\x02\x0D\x03\x11\x03\x19\x04\x21\x04\x31\x05\xA0\x41\x05\xA0\x61\x06\xA0\x81\x06\xA0\xC1\x07\xA1\x01\x07\xA1\x81\x08\xA2\x01\x08\xA3\x01\x09\xA4\x01\x09\xA6\x01\x0A\xA8\x01\x0A\xAC\x01\x0B\xB0\x01\x0B\xB8\x01\x0C\x80\x20\x01\x0C\x80\x30\x01\x0D\x80\x40\x01\x0D\x80\x60\x01\x1C\x08\xA1\x34\xA0\xDE\x0E\xA0\x42\xC1\x36\x06\x21\x86\x1A\x04\xC1\x3A\xA0\x0C\xA0\x1E\xA0\x30\xA0\x47\x0F\xA1\x3A\x04\xA6\xFB\x80\xE5\x07\x80\xDF\xE5\x80\xE6\x00\x16\xA0\x2C\x0F\xA1\x3A\x04\xA6\xD9\x80\x42\x29\x80\x7D\x0B\x80\xB3\x00\x16\xA0\x1A\x0F\xA1\x3A\x07\xA6\xFB\x80\xE5\x07\x80\xDF\xE5\x80\xE6\x06\x80\xD9\x42\x80\x29\x7D\xAB\xB3\x1D\x03\x22\xA0\x89\x1E\x20\xA0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xA0\xBF\x00\x00\xA0\xC0\xA0\xC7\x80\x40\x29\x01\xA1\x90\xA1\xFF\xA0\x90\x17\x50\x80\x40\x11\xA0\x0B\xA0\x49\xA0\x15\x22\x21\x01\x13\x21\x01\x23\x16\x9F\xCF\x08\x10\x04\x12\x50\x04\x22\x1D\x51\x22\xA0\x49\x06\x12\x51\x1E\x20\xA0\x41\x01\x05\x00\x1F\x2F\x08\x10\x04\x12\x50\x04\x26\x1D\x53\x26\xA0\x31\x06\x14\x53\x0E\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9F\x9B\x0E\x2A\xA4\x86\x0F\x38\x04\xC1\x36\x86\xA1\xEC\x06\x0D\x38\xC1\x34\x2C\x23\x2A\xA1\x38\xC1\x36\x86\xA1\xEC\x06\x00"
#else
	"\x0F\x86\x7A\xA2\x76\x8D\x05\xA2\x76\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x01\x0B\x01\x0D\x01\x0F\x01\x11\x02\x13\x02\x17\x02\x1B\x02\x1F\x03\x23\x03\x2B\x03\x33\x03\x3B\x04\xA0\x43\x04\xA0\x53\x04\xA0\x63\x04\xA0\x73\x05\xA0\x83\x05\xA0\xA3\x05\xA0\xC3\x05\xA0\xE3\x00\xA1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07\x02\x09\x02\x0D\x03\x11\x03\x19\x04\x21\x04\x31\x05\xA0\x41\x05\xA0\x61\x06\xA0\x81\x06\xA0\xC1\x07\xA1\x01\x07\xA1\x81\x08\xA2\x01\x08\xA3\x01\x09\xA4\x01\x09\xA6\x01\x0A\xA8\x01\x0A\xAC\x01\x0B\xB0\x01\x0B\xB8\x01\x0C\x80\x20\x01\x0C\x80\x30\x01\x0D\x80\x40\x01\x0D\x80\x60\x01\x1C\x06\xA1\x34\xA0\x97\x0E\xA0\x42\xC1\x36\x06\x21\x86\x1D\x03\x22\xA0\x89\x1E\x20\xA0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xA0\xBF\x00\x00\xA0\xC0\xA0\xC7\x80\x40\x29\x01\xA1\x90\xA1\xFF\xA0\x90\x17\x50\x80\x40\x11\xA0\x0B\xA0\x49\xA0\x15\x22\x21\x01\x13\x21\x01\x23\x16\x9F\xCF\x08\x10\x04\x12\x50\x04\x22\x1D\x51\x22\xA0\x49\x06\x12\x51\x1E\x20\xA0\x41\x01\x05\x00\x1F\x2F\x08\x10\x04\x12\x50\x04\x26\x1D\x53\x26\xA0\x31\x06\x14\x53\x0E\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9F\x9B\x0E\x2A\xA4\x86\x0F\x38\x04\xC1\x36\x86\xA1\xEC\x06\x0D\x38\xC1\x34\x2C\x23\x2A\xA1\x38\xC1\x36\x86\xA1\xEC\x06\x00"
#endif
};

/**
*/
DeflateCompressor::DeflateCompressor(int z_level/*=Z_BEST_COMPRESSION*/, int z_windowBits /*= Z_DEFAULT_WINDOW_BITS*/)
{
	this->zLevel = z_level;
	this->zWindowBits = z_windowBits;

	this->initialized = false;
}

/**
*/
DeflateCompressor::~DeflateCompressor()
{
	this->zUnInit();
}

/**
*/
bool DeflateCompressor::compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream)
{
	this->lock();

	bool result = true;
	
	SigCompBuffer output_buffer;
	output_buffer.referenceBuff((uint8_t*)output_ptr, output_size);

	size_t pointer =0, state_len_index =0;

	// State memory size code
	uint8_t smsCode = LIBSIGCOMP_MIN(lpCompartment->getRemoteParameters()->getSmsCode(), lpCompartment->getRemoteParameters()->getDmsCode());
	
	//
	//	Init zLIB --> only if remote sms has changed
	//
	int windowBits = (smsCode-1) + 10;
	windowBits = (windowBits < 8) ? 8 : ( (windowBits > 15 ? 15 : windowBits) ); // Because of zlib limitation (windowsize MUST be between 8 and 15)
	if(windowBits != this->zWindowBits)
	{
		this->zWindowBits = windowBits;
		//result = this->zInit();
		result = this->zReset();
		lpCompartment->freeGhostState();
		if(!result) goto bail;
	}

	//***********************************************
	//	SigComp headers
	//
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
	/* Stateless or stateful? */
#if 1
	if(lpCompartment->getGhostState())
	{
		::memmove(output_buffer.getBuffer(pointer), const_cast<SigCompBuffer*>(lpCompartment->getGhostState()->getStateIdentifier())->getBuffer(), 
			PARTIAL_ID_LEN_VALUE);
		pointer+=PARTIAL_ID_LEN_VALUE; *header |= PARTIAL_ID_LEN_CODE;
	}
#else
	if(lpCompartment->getRetFeedback() && lpCompartment->getRetFeedback()->getSize())
	{
		// FIXME: what about priorities
		SigCompBuffer* id = lpCompartment->getRetFeedback();
		uint8_t size = (*id->getBuffer(0)&0x7f);
		switch(size)
		{
		case 6:
			*header |= 0x01;
			break;
		case 9:
			*header |= 0x02;
			break;
		case 16:
			*header |= 0x03;
			break;
		default:
			assert(0);//FIXME
			break;
		}
		::memmove(output_buffer.getBuffer(pointer), id->getBuffer(1), size);
		pointer+=size;
		//id->print();
	}
#endif
	else
	{
		uint16_t codeLen = DEFLATE_BYTECODE_LEN;
		// first byte for codelen
		*output_buffer.getBuffer(pointer++) = ((codeLen>>4)& 0x00ff);
		// last 4 bits for codelen
		*output_buffer.getBuffer(pointer) = ((codeLen & 0x000f)<<4); 
		// first and last 4 bits for destination
		*output_buffer.getBuffer(pointer++) |= DEFLATE_BYTECODE_DESTINATION_CODE;

		//////////////////////////////////////////////////
		//	Upload UDVM bytecode
		//
		::memmove(output_buffer.getBuffer(pointer), (uint8_t*)DeflateCompressor::deflate_bytecode, codeLen);
		pointer+= codeLen;

		//////////////////////////////////////////////////
		// FIXME: check for overflow and >=320
		//
		// [DMS]+[Req. Fed. Loc.]+[state_len]+[cpb||dms||sms]+[Sigcomp_version]+[states]
		//*output_buffer.getBuffer(pointer++) = 0x04; //reserved=0, Q=1, S=0, I=0
		//*output_buffer.getBuffer(pointer++) = (this->req_feedback_item++); //requested feedback item

		state_len_index  = pointer;
		*output_buffer.getBuffer(pointer+=4) = 0x00; // [hash_len]+[state_len]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()>>8); // [cpb||dms||sms]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()&0x00ff); // [Sigcomp_version]
#if USE_DICTS_FOR_COMPRESSION
		*output_buffer.getBuffer(pointer++) = 0x00; // First dict byte	// FIXME
		*output_buffer.getBuffer(pointer++) = DEFLATE_FIXME_DICT; // FIXME: also change ghost
#endif
	}

	//***********************************************
	//	Compress data using ZLIB
	//
	size_t compressedDataLen = (output_size-pointer);
	bool zret = this->zCompress(input_ptr, input_size, output_buffer.getBuffer(pointer), &compressedDataLen);
	if(!zret){
		result = false;
		goto bail;
	}
	pointer+=compressedDataLen;
	output_size = (pointer);

	//
	// Update state length(FIXME - what about tcp/udp?)
	//
	if(state_len_index)
	{		
		uint16_t state_len = ( (1<<(this->zWindowBits)) + DEFLATE_UDVM_CIRCULAR_START_INDEX - 64 );
		uint32_t hash_len = (state_len+8);
		
		// FIXME: 131072  could not go in 2-bytes
		*output_buffer.getBuffer(state_len_index) = (hash_len >> 8);
		*output_buffer.getBuffer(state_len_index+1) = (hash_len & 0x00ff);
		*output_buffer.getBuffer(state_len_index+2) = (state_len >> 8);
		*output_buffer.getBuffer(state_len_index+3) = (state_len & 0x00ff);

		//	First time or synchronize failure (NACK reason=STATE_NOT_FOUND)
		this->createGhost(lpCompartment, state_len, lpCompartment->getLocalParameters());
	}

	this->updateGhost(lpCompartment, (const uint8_t*)input_ptr, input_size);

	this->unlock();

bail:
	return result;
}

__NS_DECLARATION_END__
