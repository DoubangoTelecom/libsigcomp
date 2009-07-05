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
#include <libsigcomp/SigCompMessage.h>

#define MIN_LEN 2
#define HEADER_GET_LEN() (this->headerSigComp & 0x03)
#define HEADER_GET_T() (this->headerSigComp & 0x04)
#define HEADER_IS_VALID() (this->headerSigComp >= 0xf8)

#define HEADER_GET_DEST_VALUE(destination) ( sigcomp_encoding_destination[destination] )
#define HEADER_GET_STATE_LENGTH(length) ( sigcomp_encoding_partial_id_length[length] )

__NS_DECLARATION_BEGIN__

/**
	SigCompMessage
	RFC3320: [7.  SigComp Message Format]

	@param input_ptr
	@param input_size
	@param stream
	
	@returns
*/
SigCompMessage::SigCompMessage(LPCVOID input_ptr, size_t input_size, bool stream)
:startPtr((const uint8_t*)input_ptr)
{
	if(input_size<MIN_LEN)
	{
		// FIXME: MESSAGE_TOO_SHORT
		assert(0);
		this->isOK = false;
		return;
	}
	
	this->isNack = false;
	uint8_t* dummy_ptr = ((uint8_t*)input_ptr);
	uint8_t* end_ptr = (dummy_ptr + input_size);

	// 
	this->totalSize = input_size;
	this->stream_based = stream;
	this->bytecodes_destination = 0;

	// Get sigcomp header
	this->headerSigComp = *dummy_ptr;
	dummy_ptr++;

	// Check message validity --> magic code (11111)?
	this->isOK = HEADER_IS_VALID();
	if( !this->isOK ) return;
	
	bool has_feedback = (HEADER_GET_T()!=0); // FIXME: is it right?

	// Feedback item
	if(has_feedback)
	{
		this->initFeedbackItem(dummy_ptr);
		if(!this->isOK) return;
	}

	/*If the len field is non-zero, then the SigComp message contains a state identifier 
	to access a state item at the receiving endpoint.*/
	uint8_t state_len = HEADER_GET_STATE_LENGTH( HEADER_GET_LEN() );
	if(state_len)
	{
		this->initStateId(dummy_ptr, state_len);
		this->initStateful( dummy_ptr, end_ptr );
	}
	else
	{
		if( !*dummy_ptr && !(*(dummy_ptr+1)&0xf0) ) // "code_len" field of zero --> it's a nack
		{
			this->initNack( dummy_ptr, end_ptr );
		}
		else
		{
			this->initStateless( dummy_ptr, end_ptr );
		}
	}

	/*The fields (RFC 3320 section 7) except for the "remaining SigComp message" are referred to
		as the "SigComp header" (note that this may include the uploaded UDVM bytecode).*/
	this->header_size = ( this->totalSize - this->remaining_sigcomp_buffer.getSize());
}

/**
	SigCompMessage
	RFC3320: [7.  SigComp Message Format]

	@param input_ptr
	@param input_size
	
	@returns
*/
SigCompMessage::~SigCompMessage()
{

}

/**
	initFeedbackItem

	@returns
*/
void SigCompMessage::initFeedbackItem(uint8_t* &start_ptr)
{
/*
     0   1   2   3   4   5   6   7       0   1   2   3   4   5   6   7
   +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
   | 0 |  returned_feedback_field  |   | 1 | returned_feedback_length  |
   +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
                                       |                               |
                                       :    returned_feedback_field    :
                                       |                               |
                                       +---+---+---+---+---+---+---+---+
*/
	if(*start_ptr<=128)
	{
		this->ret_feedback_buffer.referenceBuff(start_ptr, 1);
		start_ptr++;
	}
	else
	{
		this->ret_feedback_buffer.referenceBuff(start_ptr, 1+(*start_ptr&0x7f));
		start_ptr+=this->ret_feedback_buffer.getSize();
	}
}

/**
*/
void SigCompMessage::initStateId(uint8_t* &start_ptr, uint8_t state_len)
{
	this->stateId.referenceBuff(start_ptr, state_len);
	start_ptr+=state_len;
}

/**
	initStateful
	@param input_ptr
	@param input_size
	
	@returns
*/
void SigCompMessage::initStateful(uint8_t* &start_ptr, uint8_t* end_ptr)
{
	/*
   +---+---+---+---+---+---+---+---+
   |                               |
   :   partial state identifier    :
   |                               |
   +---+---+---+---+---+---+---+---+
   |                               |
   :   remaining SigComp message   :
   |                               |
   +---+---+---+---+---+---+---+---+
*/
	this->isOK &= (start_ptr<=end_ptr);
	if(this->isOK){
		this->remaining_sigcomp_buffer.referenceBuff(start_ptr, 
							((end_ptr-start_ptr)));
	}
}
	
/**
	initStateless
	@param input_ptr
	
	@returns
*/
void SigCompMessage::initStateless(uint8_t* &start_ptr, uint8_t* end_ptr)
{
	bool has_bytecode = (HEADER_GET_LEN() ==0); // No state ==> message contains udvm bytecode
	this->isOK &= has_bytecode;
	if(!this->isOK) return;

	/*
  +---+---+---+---+---+---+---+---+
  |           code_len            |
  +---+---+---+---+---+---+---+---+
  |   code_len    |  destination  |
  +---+---+---+---+---+---+---+---+
  |                               |
  :    uploaded UDVM bytecode     :
  |                               |
  +---+---+---+---+---+---+---+---+
  |                               |
  :   remaining SigComp message   :
  |                               |
  +---+---+---+---+---+---+---+---+
  */
	uint8_t* dummy_ptr = ((uint8_t*)start_ptr);

	// Code_len --> 12bits [8+4]
	uint16_t code_len1 = *dummy_ptr; dummy_ptr++; // skip first code_len 8bits
	uint8_t code_len2 = (*dummy_ptr) & 0xf0; // code_len 4 remaining bits
	uint8_t destination = (*dummy_ptr) & 0x0f; // 4bits after code_len
	dummy_ptr++; // skip code_len 4bits + destination 4bits ==> 1-byte

	// Get bytecodes length (12bits)
	uint16_t bytecodes_len = ( (code_len1<<4)|(code_len2>>4) );
	
	// Starting memory address (code destination address). In UDVM.
	this->bytecodes_destination = HEADER_GET_DEST_VALUE(destination); 
	if(this->bytecodes_destination<128 || this->bytecodes_destination>1024)
	{
		// FIXME: INVALID_CODE_LOCATION
		assert(0);
		this->isOK = false;
		return;
	}

	// Uploaded UDVM pointer
	 uint8_t* bytecodes_uploaded_udvm = dummy_ptr; // SigComp header, feedback_item, code_len and destination have been skipped
	
	 // Skip uploaded udvm
	dummy_ptr+=bytecodes_len;
	
	// remaining SigComp message
	uint8_t* remaining_SigComp_message = dummy_ptr;

	// check that remaining sigcomp message is valide
	if( !(this->isOK &= ( remaining_SigComp_message<=end_ptr )) )
	{
		// FIXME: MESSAGE_TOO_SHORT
		assert(0);
		return;
	}
	
	//
	// Setting buffers
	//
	this->uploaded_UDVM_buffer.referenceBuff(bytecodes_uploaded_udvm,
							(bytecodes_len));
	this->remaining_sigcomp_buffer.referenceBuff(remaining_SigComp_message, 
							((end_ptr-remaining_SigComp_message)));
}

/**
*/
void SigCompMessage::initNack(uint8_t* &start_ptr, uint8_t* end_ptr)
{
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
+---+---+---+---+---+---+---+---+*/

	this->isNack = true;
	if( (end_ptr-start_ptr)<25 )
	{
		this->isOK = false;
		return;
	}

	uint8_t* dummy_ptr = ((uint8_t*)start_ptr);
	dummy_ptr++; // skip first code_len byte
	if(!(this->isOK = (*dummy_ptr++ == NACK_VERSION))) 
	{
		return;
	}

	nack_info.reasonCode = *dummy_ptr++;
	nack_info.opcode = *dummy_ptr++;
	nack_info.pc = BINARY_GET_2BYTES(dummy_ptr); dummy_ptr+=2;
	::memmove(nack_info.sha1, dummy_ptr, 20); dummy_ptr+= 20;
	if(dummy_ptr < end_ptr)
	{
		// Has error details
		nack_info.details.appendBuff(dummy_ptr, (end_ptr-dummy_ptr));
	}
}

__NS_DECLARATION_END__