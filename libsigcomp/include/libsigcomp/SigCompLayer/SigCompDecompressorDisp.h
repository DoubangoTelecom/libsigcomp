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

#if !defined(LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H)
#define LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/structures/struct_decompResult.h>
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>

#include <map>

__NS_DECLARATION_BEGIN__

class SigCompManager;
class SigCompStateHandler;

class SigCompDecompressorDisp : public SafeObject
{
	friend class SigCompManager;

public:
	SigCompDecompressorDisp(const SigCompStateHandler* stateHandler);
	~SigCompDecompressorDisp();

private:
	bool decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult);
	bool getNextMessage(lpDecompressionResult lpResult);

	inline bool internalDecompress(LPCVOID input_ptr, const size_t &input_size, lpDecompressionResult *lpResult);
	bool appendStream(LPCVOID input_ptr, size_t input_size, uint64_t streamId);
	bool getNextStreamMsg(uint64_t streamId, uint16_t &discard_count, uint64_t &size);

private:
	const SigCompStateHandler* stateHandler;

	std::map<uint64_t, SigCompBuffer*> streamBuffers;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H