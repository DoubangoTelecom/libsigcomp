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
#include <libsigcomp/SigCompBuffer.h>
#include <libsigcomp/binary_utils.h>

#include <iostream>
#include <math.h>

__NS_DECLARATION_BEGIN__

// From: 
//
SigCompBuffer::SigCompBuffer()
{
	this->size = 0; 
	this->index_bytes = 0;
	this->index_bits = 0;
	this->lpbuffer = NULL;
	this->owner = true;
	/*The P-bit controls the order in which bits are passed from the
	dispatcher to the INPUT instructions*/
	this->P_BIT = P_BIT_MSB_TO_LSB;
}

//
SigCompBuffer::~SigCompBuffer()
{
	this->freeBuff();
}

//
void SigCompBuffer::reset()
{
	this->index_bytes = 0;
	this->index_bits = 0;
	memset(this->lpbuffer, 0, this->size);
}

inline bool SigCompBuffer::operator == (const SigCompBuffer &buffer) const
{
	if( getSize() == buffer.getSize() )
	{
		return startsWith(&buffer);
	}
	return false;
}

inline bool SigCompBuffer::startsWith(const SigCompBuffer* buff)const
{
	for(size_t i=0; i<buff->getSize(); i++)
	{
		if(*getReadOnlyBuffer(i)!=*buff->getReadOnlyBuffer(i)){
			return false;
		}
	}
	return true;
}

inline const size_t SigCompBuffer::getSize() const
{
	return this->size;
}

inline const size_t SigCompBuffer::getRemainingBits()const
{
	t_int64 result = ((this->size*8)-((this->index_bytes*8)+this->index_bits));
	return (result<0)?0:result;
}

inline const t_uint8* SigCompBuffer::getReadOnlyBuffer(size_t position/*=0*/)const
{
	return (this->lpbuffer + position);
}

inline t_uint8* SigCompBuffer::getBuffer(size_t position/*=0*/)
{
	return (this->lpbuffer + position);
}

inline t_uint8* SigCompBuffer::readBytes(size_t length)
{
	if((this->index_bytes+length)>(this->size)) {
		return NULL;
	}

	size_t old_index = this->index_bytes;
	this->index_bytes+=length;

	return this->getBuffer(old_index);
}

// FIXME: use mask instead of strtol
t_uint16 SigCompBuffer::readMsbToLsb(size_t length)
{
	// UDV Memory is always MSB first
	// MSB  --> LSB
	//
	t_int8 pos = 0;
	t_uint16 result_val = 0;
	char result_str[16]; memset(result_str, NULL, 16);
	while(pos < length)
	{
		result_str[pos++] = (this->lpbuffer[this->index_bytes]
			&(128 >> (this->index_bits)))?'1':'0';
		if(++this->index_bits == 8)
		{
			this->index_bytes++;
			this->index_bits = 0;
		}
	}
	
	char* end = (result_str+length);
	result_val = strtol(result_str, &end, 2);
	
	return result_val;
}

// FIXME: use mask instead of strtol
t_uint16 SigCompBuffer::readLsbToMsb(size_t length)
{
	// UDV Memory is always MSB first
	// MSB  <-- LSB
	//
	t_uint8 pos = 0;
	t_uint16 result_val = 0;
	char result_str[16]; memset(result_str, NULL, 16);
	while(pos < length)
	{
		result_str[pos++] = (this->lpbuffer[this->index_bytes]
			&(1 << (this->index_bits)))?'1':'0';
		if(++this->index_bits == 8)
		{
			this->index_bytes++;
			this->index_bits = 0;
		}
	}
	
	char* end = (result_str+length);
	result_val = strtol(result_str, &end, 2);

	return result_val;
}

inline void SigCompBuffer::discardBits()
{
	if(this->index_bits){
		this->index_bits=0;
		this->index_bytes++;
	}
}

// FIXME: very bad but ....
inline void SigCompBuffer::discardLastBytes(t_uint16 count)
{
	if(this->size>count){
		this->size-=count;
	}
	else{
		this->freeBuff();
	}
}

/**
	allocBuff

	@param Size
*/
void SigCompBuffer::allocBuff(size_t _size)
{
	assert(this->owner);
	assert(_size);
	//assert(!this->size && !this->lpbuffer);
	if(this->lpbuffer) ::free(this->lpbuffer);

	this->index_bits = this->index_bytes = 0;
	this->lpbuffer = (t_uint8*) ::malloc( _size );
	::memset( this->lpbuffer, NULL, _size);
	this->size = _size;
}

/**
	referenceBuff
	@param externalBuff
	@param _size 

	@returns
*/
void SigCompBuffer::referenceBuff(t_uint8* externalBuff, size_t _size)
{
	if(this->size && this->owner)
	{
		assert(0);
	}

	this->size = _size;
	this->lpbuffer = externalBuff;
	this->index_bytes = 0;
	this->index_bits = 0;
	this->owner = false;
}

/**
	appendBuff

	@param data
	@param _size

	@returns
*/
bool SigCompBuffer::appendBuff(const void* data, size_t _size)
{
	size_t oldSize = this->size;
	size_t newSize = (oldSize+_size);
	{
		// realloc buffer
		if(!this->size){
			this->lpbuffer = (t_uint8*) ::malloc( newSize );
		}
		else{
			this->lpbuffer = (t_uint8*) ::realloc( this->lpbuffer, newSize );
		}
	}
	if(!this->lpbuffer) return false;

	if( data )
	{
		::memmove(this->lpbuffer+oldSize, data, _size);
	}
	else
	{
		::memset(this->lpbuffer+oldSize, NULL, _size);
	}

	this->size = newSize;
	return true;
}

/**
	removeBuff

	@param pos
	@param _size

	@returns
*/
bool SigCompBuffer::removeBuff(size_t pos, size_t _size)
{
	if(((pos+_size)>this->size)) _size=this->size-pos;
	::memmove((this->lpbuffer+pos), (this->lpbuffer+pos+_size), (this->size-(pos+_size)));
	
	size_t oldSize = this->size;
	size_t newSize = (oldSize-_size);
	{
		// realloc
		if(!this->size){
			this->lpbuffer = (t_uint8*) malloc( newSize );
		}
		else{
			this->lpbuffer = (t_uint8*) realloc( this->lpbuffer, newSize );
		}
	}
	if(this->lpbuffer){
		this->size=newSize;
		return true;
	}
	return false;
}

/**
	freeBuff

	@returns
*/
void SigCompBuffer::freeBuff()
{
	if(this->lpbuffer && this->size && this->owner) 
	{
		free(this->lpbuffer);
	}
	this->lpbuffer = NULL;
	this->size = this->index_bytes = this->index_bits = NULL;
}

void SigCompBuffer::print(t_int64 size/*=-1*/)
{
#if !_DEBUG && !DEBUG
	assert(0);
#endif

	size_t size_to_print = (size<0)?this->size:size;
	if( !size_to_print ) return;

	for(int i=0; i<size_to_print; i+=2)
	{
		char s[10]; memset(s, 0, 10);
		t_uint16 value;

		if((i+1)==size_to_print) 
		{
			// last 2-byte lay in 1byte
			value = *this->getBuffer(i);
#ifdef WIN32
			sprintf_s(s, 10, i?"%0.2x":"0x%0.2x", value);
#else
			sprintf(s, i?"%0.2x":"0x%0.2x", value);
#endif
		}
		else
		{
			value = BINARY_GET_2BYTES(this->getBuffer(i));
#ifdef WIN32
			sprintf_s(s, 10, i?"%0.4x":"0x%0.4x", value);
#else
			sprintf(s, i?"%0.4x":"0x%0.4x", value);
#endif
		}
		std::cout << s << " ";
	}
	std::cout << std::endl << std::endl;
}

__NS_DECLARATION_END__