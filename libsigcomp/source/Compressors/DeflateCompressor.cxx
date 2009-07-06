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

__NS_DECLARATION_BEGIN__

// see this file for more info: 'libsigcomp/asm/deflate.asm'
const char* DeflateCompressor::deflate_bytecode =
{
#if USE_DICTS_FOR_COMPRESSION
	/*"\x0f\x86\x7a\xa2\xbd\x8d\x05\xa2\xbd\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00
	"\x09\x00\x0a\x01\x0b\x01\x0d\x01\x0f\x01\x11\x02\x13\x02\x17\x02\x1b\x02\x1f\x03\x23\x03"
	"\x2b\x03\x33\x03\x3b\x04\xa0\x43\x04\xa0\x53\x04\xa0\x63\x04\xa0\x73\x05\xa0\x83\x05\xa0"
	"\xa3\x05\xa0\xc3\x05\xa0\xe3\x00\xa1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07"
	"\x02\x09\x02\x0d\x03\x11\x03\x19\x04\x21\x04\x31\x05\xa0\x41\x05\xa0\x61\x06\xa0\x81\x06"
	"\xa0\xc1\x07\xa1\x01\x07\xa1\x81\x08\xa2\x01\x08\xa3\x01\x09\xa4\x01\x09\xa6\x01\x0a\xa8"
	"\x01\x0a\xac\x01\x0b\xb0\x01\x0b\xb8\x01\x0c\x80\x20\x01\x0c\x80\x30\x01\x0d\x80\x40\x01"
	"\x0d\x80\x60\x01\x1c\x08\xa1\x34\xa0\xde\x0e\xa0\x42\xc1\x36\x06\x21\x86\x1a\x04\xc1\x3a"
	"\xa0\x0c\xa0\x1e\xa0\x30\xa0\x47\x0f\xa1\x3a\x04\xa6\xfb\x80\xe5\x07\x80\xdf\xe5\x80\xe6"
	"\x00\x16\xa0\x2c\x0f\xa1\x3a\x04\xa6\xd9\x80\x42\x29\x80\x7d\x0b\x80\xb3\x00\x16\xa0\x1a"
	"\x0f\xa1\x3a\x07\xa6\xfb\x80\xe5\x07\x80\xdf\xe5\x80\xe6\x06\x80\xd9\x42\x80\x29\x7d\xab"
	"\xb3\x1d\x03\x22\xa0\x89\x1e\x20\xa0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xa0\xbf\x00"
	"\x00\xa0\xc0\xa0\xc7\x80\x40\x29\x01\xa1\x90\xa1\xff\xa0\x90\x17\x50\x80\x40\x11\xa0\x0b"
	"\xa0\x49\xa0\x15\x22\x21\x01\x13\x21\x01\x23\x16\x9f\xcf\x08\x10\x04\x12\x50\x04\x22\x1d"
	"\x51\x22\xa0\x49\x06\x12\x51\x1e\x20\xa0\x41\x01\x05\x00\x1f\x2f\x08\x10\x04\x12\x50\x04"
	"\x26\x1d\x53\x26\xa0\x31\x06\x14\x53\x0e\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9f\x9b"
	"\x0e\x2a\xa4\x86\x0f\x38\x04\xc1\x36\x86\xa1\xec\x06\x0d\x38\xc1\x34\x2c\x23\x2a\xa1\x38"
	"\xc1\x36\x86\xa1\xec\x06\x00"*/
#else
	"\x0f\x86\x7a\xa2\x76\x8d\x05\xa2\x76\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00"
	"\x09\x00\x0a\x01\x0b\x01\x0d\x01\x0f\x01\x11\x02\x13\x02\x17\x02\x1b\x02\x1f\x03\x23\x03"
	"\x2b\x03\x33\x03\x3b\x04\xa0\x43\x04\xa0\x53\x04\xa0\x63\x04\xa0\x73\x05\xa0\x83\x05\xa0"
	"\xa3\x05\xa0\xc3\x05\xa0\xe3\x00\xa1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07"
	"\x02\x09\x02\x0d\x03\x11\x03\x19\x04\x21\x04\x31\x05\xa0\x41\x05\xa0\x61\x06\xa0\x81\x06"
	"\xa0\xc1\x07\xa1\x01\x07\xa1\x81\x08\xa2\x01\x08\xa3\x01\x09\xa4\x01\x09\xa6\x01\x0a\xa8"
	"\x01\x0a\xac\x01\x0b\xb0\x01\x0b\xb8\x01\x0c\x80\x20\x01\x0c\x80\x30\x01\x0d\x80\x40\x01"
	"\x0d\x80\x60\x01\x1c\x06\xa1\x34\xa0\x97\x0e\xa0\x42\xc1\x36\x06\x21\x86\x1d\x03\x22\xa0"
	"\x89\x1e\x20\xa0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xa0\xbf\x00\x00\xa0\xc0\xa0\xc7"
	"\x80\x40\x29\x01\xa1\x90\xa1\xff\xa0\x90\x17\x50\x80\x40\x11\xa0\x0b\xa0\x49\xa0\x15\x22"
	"\x21\x01\x13\x21\x01\x23\x16\x9f\xcf\x08\x10\x04\x12\x50\x04\x22\x1d\x51\x22\xa0\x49\x06"
	"\x12\x51\x1e\x20\xa0\x41\x01\x05\x00\x1f\x2f\x08\x10\x04\x12\x50\x04\x26\x1d\x53\x26\xa0"
	"\x31\x06\x14\x53\x0e\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9f\x9b\x0e\x2a\xa4\x86\x0f"
	"\x38\x04\xc1\x36\x86\xa1\xec\x06\x0d\x38\xc1\x34\x2c\x23\x2a\xa1\x38\xc1\x36\x86\xa1\xec"
	"\x06\x00"
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
	//	Init zLIB
	//
	int windowBits = ( smsCode - (stream?2:1) ) + 10;
	windowBits = (windowBits < 8) ? 8 : ( (windowBits > 15 ? 15 : windowBits) ); // Because of zlib limitation (windowsize MUST be between 8 and 15)
	if(windowBits != this->zWindowBits)
	{
		// Window size changed
		lpCompartment->freeGhostState();
		this->zWindowBits = windowBits;
		if( !(result = this->zReset()) ) goto bail;
	}
	else if(!lpCompartment->getGhostState())
	{
		// No ghost --> reset zlib
		lpCompartment->getGhostCopyOffset() = 0;
		if( !(result = this->zReset()) ) goto bail;
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
