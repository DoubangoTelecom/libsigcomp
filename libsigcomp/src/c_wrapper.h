/**
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
*/

/* Copyright (C) 2009 Inexbee
*
* Authors:	Mamadou Diop <mamadou.diop@inexbee.com>
*			Laurent Etiemble <laurent.etiemble@inexbee.com>
*/

#ifndef _C_WRAPPER_H_
#define _C_WRAPPER_H_

#include "MYTYPES.h"

#ifdef __cplusplus
#	include "libsigcomp_config.h"
#	include "SigCompManager.h"
#endif

#ifdef __cplusplus
#	define EXTERN_C	extern "C" LIBSIGCOMP_API
#	define BOOLEAN	bool
#else
#	define EXTERN_C extern
#	define BOOLEAN	short
#endif

#ifndef __cplusplus
typedef void SigCompManager;
typedef void DecompressionResult;
#endif

EXTERN_C int SigCompManager_new(SigCompManager**);
EXTERN_C void SigCompManager_addSipSdpDictionary(SigCompManager*);
EXTERN_C void SigCompManager_addPresenceDictionary(SigCompManager*);
EXTERN_C void SigCompManager_setDecompression_Memory_Size(SigCompManager*, uint32_t);
EXTERN_C void SigCompManager_setState_Memory_Size(SigCompManager*, uint32_t);
EXTERN_C void SigCompManager_setCycles_Per_Bit(SigCompManager* manager, uint8_t cpb);
EXTERN_C size_t SigCompManager_compress(SigCompManager*, uint64_t compartmentId, const void* input_ptr, size_t input_size, void* output_ptr, size_t output_size, BOOLEAN stream);
EXTERN_C size_t SigCompManager_decompress(SigCompManager*, const void* input_ptr, size_t input_size, struct DecompressionResult* );
EXTERN_C void SigCompManager_provideCompartmentId(SigCompManager* , struct DecompressionResult* );
EXTERN_C void SigCompManager_closeCompartment(SigCompManager*, uint64_t compartmentId);
EXTERN_C void SigCompManager_delete(SigCompManager**);


EXTERN_C int DecompressionResult_new(DecompressionResult**);
EXTERN_C void DecompressionResult_setCompartmentId(DecompressionResult*, uint64_t compartmentId);
EXTERN_C uint64_t DecompressionResult_getCompartmentId(DecompressionResult*);
EXTERN_C void DecompressionResult_setCompartmentId2(DecompressionResult*, const void* id, size_t len);
EXTERN_C void DecompressionResult_setOutputBuffer(DecompressionResult*, void* output_ptr, size_t output_size, BOOLEAN stream, uint64_t streamId);
EXTERN_C BOOLEAN DecompressionResult_getIsNack(DecompressionResult* );
EXTERN_C uint8_t* DecompressionResult_getNackInfo(DecompressionResult*);
EXTERN_C size_t DecompressionResult_getNackInfoSize(DecompressionResult*);
EXTERN_C void DecompressionResult_delete(DecompressionResult**);

//#undef BOOLEAN
#undef EXTERN_C

#endif //_C_WRAPPER_H__