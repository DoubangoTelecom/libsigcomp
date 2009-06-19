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

#if !defined(LIBSIGCOMP_SIGCOMPBUFFER_H)
#define LIBSIGCOMP_SIGCOMPBUFFER_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <string>

#define P_BIT_MSB_TO_LSB 0
#define P_BIT_LSB_TO_MSB 1

class LIBSIGCOMP_API SigCompBuffer
{
public:
	SigCompBuffer();
	~SigCompBuffer();  

	void reset();

	// Operators
	operator t_uint8* () { return this->lpbuffer; }
	operator void* () { return (void*)this->lpbuffer; }
	operator t_uint64 () { return this->size; }
	inline bool operator == (const SigCompBuffer &buffer) const;
	inline bool startsWith(const SigCompBuffer* buff) const;

	inline const t_uint8* getReadOnlyBuffer(t_uint64 position=0)const;
	inline t_uint8* getBuffer(t_uint64 position=0);
	inline const t_uint64 getSize()const;
	inline const t_uint64 getRemainingBits()const;

	inline t_uint8* readBytes(t_uint64 size);
	t_uint16 readLsbToMsb(t_uint64 length);
	t_uint16 readMsbToLsb(t_uint64 length);
	inline void discardBits();
	inline void discardLastBytes(t_uint16 count);

	void allocBuff(t_uint64 size);
	void referenceBuff(t_uint8* externalBuff, t_uint64 size);
	bool appendBuff(const void* data, t_uint64 size);
	bool removeBuff(t_uint64 pos, t_uint64 size);
	void freeBuff();

	inline t_uint64& getIndexBytes() { return this->index_bytes; }
	inline t_uint64& getIndexBits() { return this->index_bits; }
	
	inline t_uint8& getP_BIT() { return this->P_BIT; }

	void print(t_int64 size=-1);

private:
	t_uint64	size;
	t_uint8*	lpbuffer;
	t_uint64    index_bytes;
	t_uint64    index_bits;
	bool		owner;
	t_uint8		P_BIT;
};


#endif // LIBSIGCOMP_SIGCOMPBUFFER_H