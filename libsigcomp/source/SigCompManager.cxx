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
#include <libsigcomp/SigCompManager.h>

__NS_DECLARATION_BEGIN__

/**
	SigComp manager constructor 
*/
SigCompManager::SigCompManager()
{
	this->stateHandler = new SigCompStateHandler();

	this->dispatcher_compressor = new SigCompCompressorDisp(this->stateHandler);
	this->dispatcher_decompressor = new SigCompDecompressorDisp(this->stateHandler);
}

/**
	SigComp managet destructor
*/
SigCompManager::~SigCompManager()
{
	SAFE_DELETE_PTR( this->dispatcher_compressor  );
	SAFE_DELETE_PTR( this->dispatcher_decompressor  );
	SAFE_DELETE_PTR( this->stateHandler  );
}

/**
compress

@param compartmentId
@param input_ptr
@param input_size
@param output_ptr
@param output_size
@param stream

@returns
*/

size_t SigCompManager::compress(uint64_t compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t output_size, bool stream)
{
	assert(input_size);
	assert(output_size);

	size_t ret_size = output_size;

	if(this->dispatcher_compressor->compress(compartmentId, input_ptr, input_size, output_ptr, ret_size, stream))
	{
		return ret_size;
	}
	return NULL;
}

/**
	decompress
	@param input_ptr
	@param input_size
	@param output_ptr
	@param output_size

	@returns
*/
size_t SigCompManager::decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult)
{
	assert(lpResult->getOutputBuffer()->getSize());

	lpResult->reset();
	if(this->dispatcher_decompressor->decompress(input_ptr, input_size, lpResult))
	{
		return lpResult->getOutputBuffer()->getIndexBytes();
	}
	return NULL;
}

size_t SigCompManager::getNextMessage(lpDecompressionResult lpResult)
{
	assert(lpResult->getOutputBuffer()->getSize());
	assert(lpResult->getIsStreamBased());

	lpResult->reset(false, false);
	if(this->dispatcher_decompressor->getNextMessage(lpResult))
	{
		return lpResult->getOutputBuffer()->getIndexBytes();
	}
	return NULL;
}

/**
*/
inline void SigCompManager::provideCompartmentId(lpDecompressionResult lpResult)
{
	this->stateHandler->handleResult(lpResult);
}

/**
*/
inline void SigCompManager::closeCompartment(uint64_t compartmentId)
{
	this->stateHandler->deleteCompartment(compartmentId);
}

/**
*/
inline void SigCompManager::addCompressor(SigCompCompressor* compressor)
{
	this->dispatcher_compressor->addCompressor(compressor);
}

/**
*/
inline void SigCompManager::addSipSdpDictionary()
{
	this->stateHandler->addSipSdpDictionary();
}

/**
*/
inline void SigCompManager::removeSipSdpDictionary()
{
	this->stateHandler->removeSipSdpDictionary();
}

__NS_DECLARATION_END__