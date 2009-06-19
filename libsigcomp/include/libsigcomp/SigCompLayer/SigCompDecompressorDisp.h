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

#if !defined(LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H)
#define LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#include <libsigcomp/structures/struct_decompResult.h>
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>

#include <map>

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
	
	inline bool internalDecompress(LPCVOID input_ptr, const size_t &input_size, lpDecompressionResult *lpResult);
	bool appendStream(LPCVOID input_ptr, size_t input_size, t_uint64 streamId);
	bool getNextStreamMsg(t_uint64 streamId, t_uint16 &discard_count, t_uint64 &size);

private:
	const SigCompStateHandler* stateHandler;

	std::map<t_uint64, SigCompBuffer*> streamBuffers;
};

#endif // LIBSIGCOMP_LAYER_DECOMPRESSORDISP_H