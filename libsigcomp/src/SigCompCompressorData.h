/**
	Copyright (C) 2009 Inexbee

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
*/

#if !defined(LIBSIGCOMP_LAYER_COMPRESSORDATA_H)
#define LIBSIGCOMP_LAYER_COMPRESSORDATA_H

#include "libsigcomp_config.h"

#include "SigCompState.h"
#include "SafeObject.h"

__NS_DECLARATION_BEGIN__

class SigCompCompressorData : public SafeObject
{
public:
	SigCompCompressorData(bool stream);
	~SigCompCompressorData();

	virtual void ackGhost(const SigCompBuffer* stateid);

	virtual inline SigCompState* &getGhostState() { return this->ghostState; }
	virtual inline void freeGhostState() { SAFE_DELETE_PTR(this->ghostState); }
	//virtual inline void setGhostAcked(bool bAcked) { this->acked = bAcked; }
	//virtual inline bool getGhostAcked() { return this->acked;  }
	virtual inline bool isStream() { return this->stream; }
	
protected:
	SigCompState* ghostState;
	//bool acked;
	bool stream;
};

__NS_DECLARATION_END__

#endif /* LIBSIGCOMP_LAYER_COMPRESSORDATA_H */

