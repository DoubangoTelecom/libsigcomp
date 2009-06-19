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

#include <global_config.h>
#include <libsigcomp/SigCompLayer/SigCompCompressorDisp.h>

#define NACK_SUPPORTED (this->stateHandler->getSigCompParameters()->getSigCompVersion() >= 0x02)

/**
	SigCompCompressorDisp
	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompCompressorDisp::SigCompCompressorDisp(const SigCompStateHandler* _stateHadler)
:SafeObject()
{
	this->stateHandler = const_cast<SigCompStateHandler*>(_stateHadler);
	this->compressor = new DeflateCompressor();
}

/**
	~SigCompCompressorDisp
	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompCompressorDisp::~SigCompCompressorDisp()
{
	SAFE_DELETE_PTR(this->compressor);
}

/**
*/
bool SigCompCompressorDisp::compress(t_uint64 compartmentId, LPCVOID input_ptr, size_t input_size, LPVOID output_ptr, size_t &output_size, bool stream)
{
	// For each compartment id create/retrieve one compressor instance
	SigCompCompartment* lpCompartment = this->stateHandler->getCompartment(compartmentId);
	
	if(!lpCompartment)
	{
		return false;
	}

	// FIXME: loop for compressor
	bool ret = this->compressor->compress(lpCompartment, input_ptr, input_size, output_ptr, output_size, stream);
	if(ret && NACK_SUPPORTED)
	{
		// store nack for later retrieval in case of errors
		t_uint8 nackId[SHA1HashSize];

		SHA1Context sha;
		::SHA1Reset(&sha);
		::SHA1Input(&sha, (const t_uint8*)output_ptr, output_size);
		::SHA1Result(&sha, nackId);
		lpCompartment->addNack(nackId);
	}

	return ret;
}