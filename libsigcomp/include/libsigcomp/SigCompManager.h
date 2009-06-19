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

class LIBSIGCOMP_API SigCompManager
{
public:
	SigCompManager();
	~SigCompManager();

public:
	size_t compress(t_uint64 compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t output_size, bool stream);
	size_t decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult);
	inline void provideCompartmentId(lpDecompressionResult lpResult);

	//
	//	SigComp Parameters
	//
	//
	inline void setDecompression_Memory_Size(t_uint32 dms) { dms=dms>MAX_DMS?MAX_DMS:dms; this->stateHandler->getSigCompParameters()->setDmsValue(dms); }
	inline void setState_Memory_Size(t_uint32 sms) { sms=sms>MAX_SMS?MAX_SMS:sms; this->stateHandler->getSigCompParameters()->setSmsValue(sms); }
	inline void setCycles_Per_Bit(t_uint8 cpb) { cpb>MAX_CPB?MAX_CPB:cpb; this->stateHandler->getSigCompParameters()->setCpbValue(cpb); }
	inline void setSigComp_Version(t_uint8 version) { this->stateHandler->getSigCompParameters()->setSigCompVersion(version); }

	//
	// Dictionnaries
	//
	inline void addSipSdpDictionary();
	inline void removeSipSdpDictionary();

private:
	SigCompCompressorDisp* dispatcher_compressor;
	SigCompDecompressorDisp* dispatcher_decompressor;
	SigCompStateHandler* stateHandler;
};

#endif // LIBSIGCOMP_SIGCOMP_MANAGER_H