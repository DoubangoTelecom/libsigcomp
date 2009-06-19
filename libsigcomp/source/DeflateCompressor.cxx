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
#include <libsigcomp/DeflateCompressor.h>

//
#define UDVM_CIRCULAR_BUFFER_SIZE 698

// 
#define SIP_DICT_ONLY	0x00
#define PRES_DICT_ONLY	0x01
#define SIP_PRES_DICTS	0x02
#define NO_DICT			0x03

// 
#define CODE_DESTINATION_START	320
#define CODE_DESTINATION_CODE	0x04
#define CODE_LEN				378

// see this file for more info: 'libsigcomp/asm/deflate.asm'
static const char* deflate_bytecode=
{
	"\x0F\x86\x7A\xA2\xBA\x00\x05\xA2\xBA\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00\x0A\x01\x0B\x01\x0D\x01\x0F\x01\x11\x02\x13\x02\x17\x02\x1B\x02\x1F\x03\x23\x03\x2B\x03\x33\x03\x3B\x04\xA0\x43\x04\xA0\x53\x04\xA0\x63\x04\xA0\x73\x05\xA0\x83\x05\xA0\xA3\x05\xA0\xC3\x05\xA0\xE3\x00\xA1\x02\x00\x01\x00\x02\x00\x03\x00\x04\x01\x05\x01\x07\x02\x09\x02\x0D\x03\x11\x03\x19\x04\x21\x04\x31\x05\xA0\x41\x05\xA0\x61\x06\xA0\x81\x06\xA0\xC1\x07\xA1\x01\x07\xA1\x81\x08\xA2\x01\x08\xA3\x01\x09\xA4\x01\x09\xA6\x01\x0A\xA8\x01\x0A\xAC\x01\x0B\xB0\x01\x0B\xB8\x01\x0C\x80\x20\x01\x0C\x80\x30\x01\x0D\x80\x40\x01\x0D\x80\x60\x01\x1C\x08\xA1\x34\xA0\xBF\x0E\xA0\x42\xC1\x36\x1A\x04\xC1\x3A\xA0\x0C\xA0\x1E\xA0\x30\xA0\x47\x0F\xA1\x3A\x04\xA6\xFB\x80\xE5\x07\x80\xDF\xE5\x80\xE6\x00\x16\xA0\x2C\x0F\xA1\x3A\x04\xA6\xD9\x80\x42\x29\x80\x7D\x0B\x80\xB3\x00\x16\xA0\x1A\x0F\xA1\x3A\x07\xA6\xFB\x80\xE5\x07\x80\xDF\xE5\x80\xE6\x06\x80\xD9\x42\x80\x29\x7D\xAB\xB3\x1D\x03\x22\xA0\x89\x1E\x20\xA0\x68\x04\x07\x00\x17\x80\x40\x11\x01\x30\xA0\xBF\x00\x00\xA0\xC0\xA0\xC7\x80\x40\x29\x01\xA1\x90\xA1\xFF\xA0\x90\x17\x50\x80\x40\x11\xA0\x0B\xA0\x49\xA0\x15\x22\x21\x01\x13\x21\x01\x23\x16\x9F\xCF\x08\x10\x04\x12\x50\x04\x22\x1D\x51\x22\xA0\x49\x06\x12\x51\x1E\x20\xA0\x41\x01\x05\x00\x1F\x2F\x08\x10\x04\x12\x50\x04\x26\x1D\x53\x26\xA0\x31\x06\x14\x53\x0E\x20\x63\x14\x54\x52\x23\x22\x50\x52\x16\x9F\x9B\x0E\x2A\xA4\x86\x0F\x38\x04\xC1\x36\x86\xA2\x30\x06\x0D\x38\xC1\x34\x2C\x23\x2A\xA1\x38\xC1\x36\x86\xA2\x30\x06\x00"
};

/**
*/
DeflateCompressor::DeflateCompressor(int z_level/*=Z_DEFAULT_COMPRESSION*/, int z_windowBits /*= Z_DEFAULT_WINDOW_BITS*/)
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
	//FIXME: lock

	SigCompBuffer input_buffer;
	SigCompBuffer output_buffer;

	input_buffer.referenceBuff((t_uint8*)input_ptr, input_size);
	output_buffer.referenceBuff((t_uint8*)output_ptr, output_size);

	t_uint16 pointer = 0;
	t_uint16 state_len_index = 0;

	// State memory size code
	t_uint32 smsCode = min(lpCompartment->getRemoteParameters()->getSmsCode(), lpCompartment->getRemoteParameters()->getDmsCode());


	//***********************************************
	//	SigComp headers
	//
	t_uint8* header = output_buffer.getBuffer(pointer++);

	
	/* SigComp Header */
	if(lpCompartment->getReqFeedback() && lpCompartment->getReqFeedback()->getSize()){
		*header = 0xfc; // T=1
		::memmove(output_buffer.getBuffer(pointer), lpCompartment->getReqFeedback()->getBuffer(), lpCompartment->getReqFeedback()->getSize());
		pointer+=lpCompartment->getReqFeedback()->getSize();
	}
	else{
		*header = 0xf8;
	}
	/* Stateless or stateful? */
	if(lpCompartment->getRetFeedback() && lpCompartment->getRetFeedback()->getSize())
	{
		// FIXME: what about priorities
		SigCompBuffer* id = lpCompartment->getRetFeedback();
		t_uint8 size = (*id->getBuffer(0)&0x7f);
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
	else
	{
		//	Init zLIB windowBits
		this->zSetWindowBits( (smsCode-1) + 10 );

		t_uint16 codeLen = CODE_LEN;
		// first byte for codelen
		*output_buffer.getBuffer(pointer++) = ((codeLen>>4)& 0x00ff);
		// last 4 bits for codelen
		*output_buffer.getBuffer(pointer) = ((codeLen & 0x000f)<<4); 
		// first and last 4 bits for destination
		*output_buffer.getBuffer(pointer++) |= CODE_DESTINATION_CODE;

		//////////////////////////////////////////////////
		//	Upload UDVM bytecode
		//
		::memmove(output_buffer.getBuffer(pointer), (t_uint8*)deflate_bytecode, codeLen);
		pointer+= codeLen;

		//////////////////////////////////////////////////
		// FIXME: check for overflow and >=320
		//
		// [Req. Fed. Loc.]+[state_len]+[cpb||dms||sms]+[Sigcomp_version]+[states]
		//*output_buffer.getBuffer(pointer++) = 0x04; //reserved=0, Q=1, S=0, I=0
		//*output_buffer.getBuffer(pointer++) = (this->req_feedback_item++); //requested feedback item
		state_len_index  = pointer;
		*output_buffer.getBuffer(pointer+=4) = 0x00; // [hash_len]+[state_len]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()>>8); // [cpb||dms||sms]
		*output_buffer.getBuffer(pointer++) = (lpCompartment->getLocalParameters()->getParameters()&0x00ff); // [Sigcomp_version]
		*output_buffer.getBuffer(pointer++) = 0x00; // First dict byte
		*output_buffer.getBuffer(pointer++) = NO_DICT;
	}

	//***********************************************
	//	Compress data using ZLIB
	//
	size_t compressedDataLen = (output_size-pointer);
	bool zret = this->zCompress(input_ptr, input_size, output_buffer.getBuffer(pointer), &compressedDataLen);
	if(!zret){
		return false;
	}
	pointer+=compressedDataLen;
	output_size = (pointer);

	//
	// Update state length(FIXME - what about tcp/udp?)
	//
	if(state_len_index)
	{		
		t_uint16 state_len = ( (1<<(this->zWindowBits)) + UDVM_CIRCULAR_BUFFER_SIZE - 64 );
		t_uint32 hash_len = (state_len+8);
		
		// FIXME: 131072  could not go in 2-bytes
		*output_buffer.getBuffer(state_len_index) = (hash_len >> 8);
		*output_buffer.getBuffer(state_len_index+1) = (hash_len & 0x00ff);
		*output_buffer.getBuffer(state_len_index+2) = (state_len >> 8);
		*output_buffer.getBuffer(state_len_index+3) = (state_len & 0x00ff);
	}
}