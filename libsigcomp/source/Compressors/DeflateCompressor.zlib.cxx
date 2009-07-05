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
#if 0
#	include <libsigcomp/rfc3485_dictionary_sip.h>
#endif

__NS_DECLARATION_BEGIN__

/**
*/
bool DeflateCompressor::zInit()
{
	/* allocate deflate state */
    this->zStream.zalloc = Z_NULL;
    this->zStream.zfree = Z_NULL;
    this->zStream.opaque = Z_NULL;
	this->zStream.data_type = Z_TEXT;

    //bool ret  =  (deflateInit(&this->zStream, this->zLevel) == Z_OK);
	if( deflateInit2(&this->zStream, this->zLevel, Z_DEFLATED, -this->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK )
	{
		return false;
	}
#if 0
	if( deflateSetDictionary(&this->zStream, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK )
	{
		return false;
	}
#endif

	this->initialized = true;
	return true;
}

/**
*/
bool DeflateCompressor::zUnInit()
{
	if(this->initialized)
	{
		this->initialized = false;
		return !(deflateEnd(&this->zStream) == Z_STREAM_ERROR);
	}
	return true;
}

/**
*/
bool DeflateCompressor::zCompress(const void* in, size_t inLen, void* out, size_t* outLen)
{
#define STATE_LESS 0

#if STATE_LESS
	assert (deflateInit(&this->zStream, this->zLevel) == Z_OK);
#endif

	// Initialized?
	if(!this->initialized) assert(this->zInit());

	// IN
	this->zStream.next_in = (Bytef*)in;
    this->zStream.avail_in = (uInt)inLen;

	// OUT
	this->zStream.next_out = (Bytef*)out;
    this->zStream.avail_out = (uInt)*outLen;
		
#if STATE_LESS
	int ret = deflate(&this->zStream, Z_FINISH);

	*outLen = this->zStream.total_out;
	deflateEnd(&this->zStream);
	return (ret==Z_STREAM_END);
#else
	int ret = deflate(&this->zStream, Z_SYNC_FLUSH);

	*outLen -= this->zStream.avail_out;

	return (ret==Z_OK);
#endif

}

__NS_DECLARATION_END__