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

#if !defined(LIBSIGCOMP_LAYER_COMPRESSORDISP_H)
#define LIBSIGCOMP_LAYER_COMPRESSORDISP_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>
#include <libsigcomp/SigCompLayer/SigCompStateHandler.h>
#include <libsigcomp/SafeObject.h>

#include <libsigcomp/Compressors/DeflateCompressor.h>
#include <libsigcomp/Compressors/DummyCompressor.h>

class SigCompManager;

__NS_DECLARATION_BEGIN__

class SigCompCompressorDisp : public SafeObject
{
	friend class SigCompManager;

public:
	SigCompCompressorDisp(const SigCompStateHandler* stateHadler);
	~SigCompCompressorDisp();

	bool compress(uint64_t compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream);

	void addCompressor(SigCompCompressor* compressor);

private:
	SigCompStateHandler* stateHandler;
	std::list<SigCompCompressor*> compressors;
		
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_LAYER_COMPRESSORDISP_H