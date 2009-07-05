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

#if !defined(LIBSIGCOMP_SIGCOMPBUFFER_H)
#define LIBSIGCOMP_SIGCOMPBUFFER_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <string>

#define P_BIT_MSB_TO_LSB 0
#define P_BIT_LSB_TO_MSB 1

__NS_DECLARATION_BEGIN__

class LIBSIGCOMP_API SigCompBuffer
{
public:
	SigCompBuffer();
	~SigCompBuffer();  

	void reset();

	// Operators
	operator uint8_t* () { return this->lpbuffer; }
	operator void* () { return (void*)this->lpbuffer; }
	operator size_t () { return this->size; }
	inline bool operator == (const SigCompBuffer &buffer) const;
	inline bool startsWith(const SigCompBuffer* buff) const;

	inline const uint8_t* getReadOnlyBuffer(size_t position=0)const;
	inline uint8_t* getBuffer(size_t position=0);
	inline const size_t getSize()const;
	inline const size_t getRemainingBits()const;

	inline uint8_t* readBytes(size_t size);
	uint16_t readLsbToMsb(size_t length);
	uint16_t readMsbToLsb(size_t length);
	inline void discardBits();
	inline void discardLastBytes(uint16_t count);

	void allocBuff(size_t size);
	void referenceBuff(uint8_t* externalBuff, size_t size);
	bool appendBuff(const void* data, size_t size);
	bool removeBuff(size_t pos, size_t size);
	void freeBuff();

	inline size_t& getIndexBytes() { return this->index_bytes; }
	inline size_t& getIndexBits() { return this->index_bits; }
	
	inline uint8_t& getP_BIT() { return this->P_BIT; }

	void print(int64_t size=-1);

private:
	size_t	size;
	uint8_t*	lpbuffer;
	size_t    index_bytes;
	size_t    index_bits;
	bool		owner;
	uint8_t		P_BIT;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_SIGCOMPBUFFER_H