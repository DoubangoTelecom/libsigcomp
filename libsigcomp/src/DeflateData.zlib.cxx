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

/*
	09-2009, by Inexbee: Only acknowledged states MUST be loaded for compression
*/

#include "global_config.h"
#include "DeflateData.h"
#if 0
#	include "rfc3485_dictionary_sip.h"
#endif

__NS_DECLARATION_BEGIN__

/**
*/
bool DeflateData::zInit()
{
	if(this->initialized) return true;

	/* allocate deflate state */
    this->stream_1.zs.zalloc = this->stream_acked.zs.zalloc = Z_NULL;
    this->stream_1.zs.zfree = this->stream_acked.zs.zfree = Z_NULL;
    this->stream_1.zs.opaque = this->stream_acked.zs.opaque = Z_NULL;
#ifndef __SYMBIAN32__
	this->stream_1.zs.data_type = this->stream_acked.zs.data_type = Z_TEXT;
#endif

    //bool ret  =  (deflateInit(&this->zStream, this->zLevel) == Z_OK);
	if( deflateInit2(&this->stream_1.zs, this->zLevel, Z_DEFLATED, -this->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK 
		|| deflateInit2(&this->stream_acked.zs, this->zLevel, Z_DEFLATED, -this->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK )
	{
		return false;
	}
#if USE_DICTS_FOR_COMPRESSION
	if( deflateSetDictionary(this->stream_1.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK 
		|| deflateSetDictionary(this->stream_acked.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK )
	{
		return false;
	}
#endif

	this->stream_1.stateful = this->stream_acked.stateful = false;
	this->stream_1.dataWaitingAck = this->stream_acked.dataWaitingAck = false;
	this->initialized = true;

	return true;
}

/**
*/
bool DeflateData::zUnInit()
{
	if(this->initialized)
	{
		this->initialized = false;
		
		this->stream_1.dataWaitingAck = this->stream_acked.dataWaitingAck = false;
		this->stream_1.stateful = this->stream_acked.stateful = false;

		return (this->stream_1.end() != Z_STREAM_ERROR) && (this->stream_acked.end() != Z_STREAM_ERROR);
	}
	return true;
}

/**
	Two streams [1] and [2]
	
	* ZINIT/ZUNINIT/ZRESET
		XXX([1])
		XXX([2])
	
	* COMPRESSION
		[1]->END()
		[1]<-COPY-[2]
		[1]->COMPRESS()
	
	* ACK
		[2]->END()
		[2]<-COPY-[1]
		updateGhost([1])

*/
bool DeflateData::zCompress(const void* in, size_t inLen, void* out, size_t* outLen, bool *stateChanged)
{
	this->lock();
	
	// Initialized?
	if(!this->initialized){
		assert(this->zInit());
	}
	
#if USE_ONLY_ACKED_STATES
	if(!this->stream_acked.dataWaitingAck){
		this->stream_acked.dataWaitingAck = true;
		*stateChanged = true;
	}else{
		*stateChanged = false;
	}

	// END() + COPY() ==> use acked state
	this->stream_1.end();
	this->stream_1.copy(&this->stream_acked);
#else
	*stateChanged = true;
#endif
	
	// IN
	this->stream_1.zs.next_in = (Bytef*)in;
    this->stream_1.zs.avail_in = (uInt)inLen;
	
	// OUT
	this->stream_1.zs.next_out = (Bytef*)out;
    this->stream_1.zs.avail_out = (uInt)*outLen;
	
	int ret = deflate(&this->stream_1.zs, Z_SYNC_FLUSH);
	
	*outLen -= this->stream_1.zs.avail_out;
	
	this->unlock();
	
	return (ret==Z_OK);
}

__NS_DECLARATION_END__
