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

#include "global_config.h"
#include "SigCompCompressorData.h"

__NS_DECLARATION_BEGIN__

/**
*/
SigCompCompressorData::SigCompCompressorData(bool _stream)
:SafeObject(), stream(_stream)
{
	this->ghostState = NULL;
}

/**
*/
SigCompCompressorData::~SigCompCompressorData()
{
	// Delete Ghost state
	SAFE_DELETE_PTR(this->ghostState);
}

/**
*/
void SigCompCompressorData::ackGhost(const SigCompBuffer* stateid)
{
	this->lock();
	
	assert(0);
	
	this->unlock();
}

__NS_DECLARATION_END__