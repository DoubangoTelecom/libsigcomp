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

#if !defined(LIBSIGCOMP_COMPRESSORS_DEFLATEDATA_H)
#define LIBSIGCOMP_COMPRESSORS_DEFLATEDATA_H

#include "libsigcomp_config.h"

#include "SigCompCompressorData.h"
#include "struct_sigcomp_parameters.h"

#if HAS_ZLIB
#	include <zlib.h>
#else
#	include "zlib.h"
#endif

/** [[[[[[[[[[[[[[[[[ **/
#define USE_DICTS_FOR_COMPRESSION				0

#define Z_DEFAULT_WINDOW_BITS 10 /* 1024*/

#define DEFLATE_DECOMPRESSION_PTR_INDEX			70
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	701
#else
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	630
#endif

#define DEFLATE_SIP_DICT_ONLY					0x00
#define DEFLATE_PRES_DICT_ONLY					0x01
#define DEFLATE_SIP_PRES_DICTS					0x02
#define DEFLATE_NO_DICT							0x03
#define DEFLATE_FIXME_DICT						DEFLATE_NO_DICT

#define DEFLATE_BYTECODE_DESTINATION_START		320
#define DEFLATE_BYTECODE_DESTINATION_CODE		0x04 // 320
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_BYTECODE_LEN					381
#else
#	define DEFLATE_BYTECODE_LEN					310
#endif
/**	]]]]]]]]]]]]]] **/

__NS_DECLARATION_BEGIN__


typedef struct tag_deflateStream
{
public:
	z_stream zs;
	bool dataWaitingAck;
	bool stateful;

	tag_deflateStream(){
		stateful = false;
		dataWaitingAck = false;
	}
	
	int end(){
		return deflateEnd(&zs);
	}
	void copy(tag_deflateStream* source){
		deflateCopy(&zs, &(source->zs));
	}
}
deflateStream;

class DeflateData : public SigCompCompressorData
{
public:
	DeflateData(bool stream, int z_level=Z_BEST_COMPRESSION, int z_windowBits = Z_DEFAULT_WINDOW_BITS);
	~DeflateData();

public: /* OVERRIDE FROM [SigCompCompressorData] */
	INLINE void freeGhostState() { SAFE_DELETE_PTR(this->ghostState); this->ghost_copy_offset = 0; }
	void ackGhost(const SigCompBuffer* stateid);

public:
	void createGhost(uint16_t state_len, lpstruct_sigcomp_parameters params);
	void updateGhost(const uint8_t* input_ptr, size_t input_size);
	INLINE uint32_t &getGhostCopyOffset() { return this->ghost_copy_offset; }
	
	bool zReset() { bool ret = initialized?zUnInit():true; ret &= zInit(); return ret; }
	bool zCompress(const void* in, size_t inLen, void* out, size_t* outLen, bool *stateChanged);

	INLINE int zGetWindowBits(){ return this->zWindowBits; }
	INLINE void zSetWindowBits(int windowSize){ this->zWindowBits = windowSize; }

	INLINE bool isStateful() { return this->stream_acked.stateful; }
	
public:
	static const char* deflate_bytecode;

private:
	static const char* deflate_bytecode1_ghost;
	
	bool zInit();
	bool zUnInit();

private:
	deflateStream stream_1;
	deflateStream stream_acked;
	int zLevel;
	int zWindowBits;
	
	bool initialized;
	
	uint32_t ghost_copy_offset;
};

__NS_DECLARATION_END__

#endif /* LIBSIGCOMP_COMPRESSORS_DEFLATEDATA_H */