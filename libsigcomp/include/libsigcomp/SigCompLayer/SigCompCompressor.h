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

#if !defined(LIBSIGCOMP_LAYER_COMPRESSOR_H)
#define LIBSIGCOMP_LAYER_COMPRESSOR_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>
#include <libsigcomp/SafeObject.h>

#include <map>

class SigCompCompressor : public SafeObject
{
public:
	SigCompCompressor();
	~SigCompCompressor();

	virtual bool SigCompCompressor::compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream) = 0; 

private:
	
};

#endif // LIBSIGCOMP_LAYER_COMPRESSOR_H