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

#if !defined(LIBSIGCOMP_SIGCOMP_MANAGER_H)
#define LIBSIGCOMP_SIGCOMP_MANAGER_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/structures/struct_decompResult.h>

#include <libsigcomp/SigCompLayer/SigCompDecompressorDisp.h>
#include <libsigcomp/SigCompLayer/SigCompCompressorDisp.h>
#include <libsigcomp/SigCompLayer/SigCompStateHandler.h>

#define MAX_DMS	131072
#define MAX_SMS	131072
#define MAX_CPB	128

__NS_DECLARATION_BEGIN__

class LIBSIGCOMP_API SigCompManager
{
public:
	SigCompManager();
	~SigCompManager();

public:
	//
	//	Compression / Decompression
	//
	size_t compress(uint64_t compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t output_size, bool stream);
	size_t decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult);
	size_t getNextMessage(lpDecompressionResult lpResult);

	//
	//	Compartment management
	//
	inline void provideCompartmentId(lpDecompressionResult lpResult);
	inline void closeCompartment(uint64_t compartmentId);

	//
	//	SigComp Parameters
	//
	//
	inline void setDecompression_Memory_Size(uint32_t dms) { dms=dms>MAX_DMS?MAX_DMS:dms; this->stateHandler->getSigCompParameters()->setDmsValue(dms); }
	inline void setState_Memory_Size(uint32_t sms) { sms=sms>MAX_SMS?MAX_SMS:sms; this->stateHandler->getSigCompParameters()->setSmsValue(sms); }
	inline void setCycles_Per_Bit(uint8_t cpb) { cpb>MAX_CPB?MAX_CPB:cpb; this->stateHandler->getSigCompParameters()->setCpbValue(cpb); }
	inline void setSigComp_Version(uint8_t version) { this->stateHandler->getSigCompParameters()->setSigCompVersion(version); }

	//
	//	Compressors
	//
	inline void addCompressor(SigCompCompressor* compressor);
	
	//
	//	Dictionnaries
	//
	inline void addSipSdpDictionary();
	inline void addPresenceDictionary();

private:
	SigCompCompressorDisp* dispatcher_compressor;
	SigCompDecompressorDisp* dispatcher_decompressor;
	SigCompStateHandler* stateHandler;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_SIGCOMP_MANAGER_H