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
#include <libsigcomp/SigCompLayer/SigCompCompressorDisp.h>

#define NACK_SUPPORTED (this->stateHandler->getSigCompParameters()->getSigCompVersion() >= 0x02)

__NS_DECLARATION_BEGIN__

/**
	SigCompCompressorDisp
	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompCompressorDisp::SigCompCompressorDisp(const SigCompStateHandler* _stateHadler)
:SafeObject()
{
	this->stateHandler = const_cast<SigCompStateHandler*>(_stateHadler);
	//this->addCompressor( new DummyCompressor() ); // RFC 4896 [11. Uncompressed Bytecode]
	this->addCompressor( new DeflateCompressor() ); // If you don't want deflate compressor then remove this line
}

/**
	~SigCompCompressorDisp
	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompCompressorDisp::~SigCompCompressorDisp()
{
	// Clear compressors
	std::list<SigCompCompressor* >::iterator it_compressors;
	SAFE_CLEAR_LIST(this->compressors, it_compressors);
}

/**
*/
bool SigCompCompressorDisp::compress(uint64_t compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream)
{
	bool ret = true;

	// For each compartment id create/retrieve one compressor instance
	SigCompCompartment* lpCompartment = this->stateHandler->getCompartment(compartmentId);
	
	if(!lpCompartment)
	{
		return false;
	}

	// FIXME: buffer overflow
	this->lock();
	std::list<SigCompCompressor*>::iterator it_compressor;
	for( it_compressor=this->compressors.begin(); it_compressor!=this->compressors.end(); it_compressor++  )
	{
		ret = (*it_compressor)->compress(lpCompartment, input_ptr, input_size, output_ptr, output_size, stream);
		if(ret) break;
	}
	this->unlock();

	//
	//	STREAM case. FIXME:Because I'm a lazy man I only support 0xFF00 case
	//
	//
	if(stream)
	{
		size_t escapedBufferSize = (output_size + 2); // 2 = strlen(0xffff)
		for(register int i=0;i<output_size;i++) escapedBufferSize += ((uint8_t*)output_ptr)[i]==0xff?1:0;
		uint8_t* escapedBuffer = (uint8_t*)malloc(escapedBufferSize);
		assert(escapedBuffer);

		for(register int i=0, j=0; i<output_size; i++, j++)
		{
			escapedBuffer[j] = ((uint8_t*)output_ptr)[i];
			if(escapedBuffer[j] == 0xff) 
			{
				escapedBuffer[++j] = 0x00;
			}
		}
		
		// End stream
		escapedBuffer[escapedBufferSize-1] = escapedBuffer[escapedBufferSize-2] = 0xff;
		::memmove(output_ptr, escapedBuffer, escapedBufferSize);
		
		output_size = escapedBufferSize; // Update size
		free(escapedBuffer); // free
	}

	//
	// NACK
	//
	if(ret && NACK_SUPPORTED)
	{
		// store nack for later retrieval in case of errors
		uint8_t nackId[SHA1HashSize];

		SHA1Context sha;
		::SHA1Reset(&sha);
		::SHA1Input(&sha, (const uint8_t*)output_ptr, output_size);
		::SHA1Result(&sha, nackId);
		lpCompartment->addNack(nackId);
	}

	return ret;
}
/**
*/
void SigCompCompressorDisp::addCompressor(SigCompCompressor* compressor)
{
	this->lock();
	this->compressors.push_front(compressor);
	this->unlock();
}

__NS_DECLARATION_END__