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

/**
*/
bool DeflateCompressor::zInit()
{
	/* allocate deflate state */
    this->zStream.zalloc = Z_NULL;
    this->zStream.zfree = Z_NULL;
    this->zStream.opaque = Z_NULL;
	this->zStream.data_type = Z_TEXT;
	
    //this->initialized  =  (deflateInit(&this->zStream, this->zLevel) == Z_OK);
	this->initialized = ( deflateInit2(&this->zStream, this->zLevel, Z_DEFLATED, -this->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(this->zStream)) == Z_OK );
	return this->initialized;
}

/**
*/
bool DeflateCompressor::zUnInit()
{
	if(this->initialized)
	{
		this->initialized = false;
		return (deflateEnd(&this->zStream) == Z_OK);
	}
	return true;
}

/**
*/
void DeflateCompressor::zSetWindowBits(int windowBits)
{
	if(this->zWindowBits != windowBits)
	{
		assert(windowBits>=8 && windowBits<=15); // FIXME
		this->zWindowBits = windowBits;
		assert(this->zReset());
	}
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
	if ((uLong)this->zStream.avail_out != *outLen) return outLen;
	
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