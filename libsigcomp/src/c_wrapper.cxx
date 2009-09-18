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

/* Copyright (C) 2008 Inexbee
*
* Authors:	Mamadou Diop <mamadou.diop@inexbee.com>
*			Laurent Etiemble <laurent.etiemble@inexbee.com>
*/

#include "c_wrapper.h"

#define GET_MANAGER(manager) ((sigcomp::SigCompManager*)manager)
#define GET_RESULT(result) ((sigcomp::DecompressionResult*)result)

/**
	new SigCompManager()
	Returns 1 if success, otherwise 0
*/
int SigCompManager_new(SigCompManager** manager)
{
	*manager = (SigCompManager*)new sigcomp::SigCompManager();
	return *manager?1:0;
}

/**
	inline SigCompManager::addSipSdpDictionary()
*/
void SigCompManager_addSipSdpDictionary(SigCompManager* manager)
{
	GET_MANAGER(manager)->addSipSdpDictionary();
}

/**
	inline SigCompManager::addPresenceDictionary();
*/
void SigCompManager_addPresenceDictionary(SigCompManager* manager)
{
	GET_MANAGER(manager)->addPresenceDictionary();
}

/**
	inline void SigCompManager::setDecompression_Memory_Size(uint32_t dms)
*/
void SigCompManager_setDecompression_Memory_Size(SigCompManager* manager, uint32_t dms)
{
	GET_MANAGER(manager)->setDecompression_Memory_Size(dms);
}

/**
	inline  void setState_Memory_Size(uint32_t sms)
*/
void SigCompManager_setState_Memory_Size(SigCompManager* manager, uint32_t sms)
{
	GET_MANAGER(manager)->setState_Memory_Size(sms);
}

/**
	inline void setCycles_Per_Bit(uint8_t cpb)
*/
void SigCompManager_setCycles_Per_Bit(SigCompManager* manager, uint8_t cpb)
{
	GET_MANAGER(manager)->setCycles_Per_Bit(cpb);
}

/**
	size_t SigCompManager::SigCompManager::compress(uint64_t compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t output_size, bool stream)
*/
size_t SigCompManager_compress(SigCompManager* manager, uint64_t compartmentId, const void* input_ptr, size_t input_size, void* output_ptr, size_t output_size, BOOLEAN stream)
{
	return GET_MANAGER(manager)->compress(compartmentId, input_ptr, input_size, output_ptr, output_size, stream);
}

/**
	size_t SigCompManager::decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult)
*/
size_t SigCompManager_decompress(SigCompManager* manager, const void* input_ptr, size_t input_size, struct DecompressionResult* lpResult)
{
	return GET_MANAGER(manager)->decompress(input_ptr, input_size, GET_RESULT(lpResult));
}

/**
	inline void SigCompManager::provideCompartmentId(lpDecompressionResult lpResult)
*/
void SigCompManager_provideCompartmentId(SigCompManager* manager, struct DecompressionResult* lpResult)
{
	GET_MANAGER(manager)->provideCompartmentId(GET_RESULT(lpResult));
}

/**
	SigCompManager::closeCompartment(...)
*/
void SigCompManager_closeCompartment(SigCompManager* manager, uint64_t compartmentId)
{
	GET_MANAGER(manager)->closeCompartment(compartmentId);
}

/**
	delete SigCompManager
*/
void SigCompManager_delete(SigCompManager** manager)
{
	SAFE_DELETE_PTR(*manager);
}









/**
	new DecompressionResult()
	Returns 1 if success, otherwise 0
*/
int DecompressionResult_new(struct DecompressionResult** result)
{
	*result = (struct DecompressionResult*)new sigcomp::DecompressionResult();
	return *result?1:0;
}

/**
	inline void DecompressionResult::setCompartmentId(uint64_t id)
*/
void DecompressionResult_setCompartmentId(struct DecompressionResult* result, uint64_t compartmentId)
{
	GET_RESULT(result)->setCompartmentId(compartmentId);
}

/**
	inline void DecompressionResult::setOutputBuffer(LPVOID output_ptr, size_t output_size, bool stream, uint64_t streamId=0)
*/
void DecompressionResult_setOutputBuffer(struct DecompressionResult* result, void* output_ptr, size_t output_size, BOOLEAN stream, uint64_t streamId)
{
	GET_RESULT(result)->setOutputBuffer(output_ptr, output_size, stream,  streamId);
}
/**
	inline bool getIsNack()
*/
BOOLEAN DecompressionResult_getIsNack(struct DecompressionResult* result)
{
	return GET_RESULT(result)->getIsNack();
}

/**
	inline SigCompBuffer* getNackInfo()
*/
uint8_t* DecompressionResult_getNackInfo(struct DecompressionResult* result)
{
	return GET_RESULT(result)->getNackInfo()->getBuffer();
}

/**
	DecompressionResult::getNackInfo()->getSize()
*/
size_t DecompressionResult_getNackInfoSize(struct DecompressionResult* result)
{
	return GET_RESULT(result)->getNackInfo()->getSize();
}

/**
	delete result
*/
void DecompressionResult_delete(struct DecompressionResult** result)
{
	SAFE_DELETE_PTR(*result);
}