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

#if !defined(LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H)
#define LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/SigCompLayer/SigCompCompressor.h>

#include <libsigcomp/zlib/zlib.h>

#define Z_DEFAULT_WINDOW_BITS 10 /* 1024*/

class LIBSIGCOMP_API DeflateCompressor : public SigCompCompressor
{
public:
	DeflateCompressor(int z_level=Z_DEFAULT_COMPRESSION, int z_windowBits = Z_DEFAULT_WINDOW_BITS);
	~DeflateCompressor();

	virtual bool compress(SigCompCompartment* lpCompartment, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream);

private:
	//
	//	zLIB
	//
	bool zInit();
	bool zUnInit();
	void zSetWindowBits(int windowBits);
	bool zReset() { bool ret = initialized?zUnInit():true; ret &= zInit(); return ret; }
	bool zCompress(const void* in, size_t inLen, void* out, size_t* outLen);

private:
	z_stream zStream;
	int zLevel;
	int zWindowBits;
	
	bool initialized;
};


#endif // LIBSIGCOMP_SIGCOMP_DEFLATECOMPRESSOR_H