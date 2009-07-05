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

#if !defined(LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H)
#define LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/SigCompLayer/SigCompCompressor.h>
#include <libsigcomp/SigCompLayer/SigCompState.h>

#include <libsigcomp/zlib/zlib.h>

#define USE_DICTS_FOR_COMPRESSION			0

#define Z_DEFAULT_WINDOW_BITS 10 /* 1024*/

#define DEFLATE_DECOMPRESSION_PTR_INDEX			70
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	701
#else
#	define DEFLATE_UDVM_CIRCULAR_START_INDEX	630
#endif

#define DEFLATE_SIP_DICT_ONLY				0x00
#define DEFLATE_PRES_DICT_ONLY				0x01
#define DEFLATE_SIP_PRES_DICTS				0x02
#define DEFLATE_NO_DICT						0x03
#define DEFLATE_FIXME_DICT					DEFLATE_NO_DICT

#define DEFLATE_BYTECODE_DESTINATION_START	320
#define DEFLATE_BYTECODE_DESTINATION_CODE	0x04 // 320
#if USE_DICTS_FOR_COMPRESSION
#	define DEFLATE_BYTECODE_LEN				381
#else
#	define DEFLATE_BYTECODE_LEN				310
#endif


__NS_DECLARATION_BEGIN__

class LIBSIGCOMP_API DeflateCompressor : public SigCompCompressor
{
public:
	DeflateCompressor(int z_level=Z_BEST_COMPRESSION, int z_windowBits = Z_DEFAULT_WINDOW_BITS);
	~DeflateCompressor();

	virtual bool compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream);

private:
	//
	static const char* deflate_bytecode;
	static const char* deflate_bytecode1_ghost;

	//
	//	Ghost state
	//
	void createGhost(SigCompCompartment* lpCompartment, t_uint16 state_len, lpstruct_sigcomp_parameters params);
	void updateGhost(SigCompCompartment* lpCompartment, const t_uint8* input_ptr, size_t input_size);

	//
	//	zLIB
	//
	bool zInit();
	bool zUnInit();
	bool zReset() { bool ret = initialized?zUnInit():true; ret &= zInit(); return ret; }
	bool zCompress(const void* in, size_t inLen, void* out, size_t* outLen);

private:
	z_stream zStream;
	int zLevel;
	int zWindowBits;
	
	bool initialized;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H