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

#if !defined(LIBSIGCOMP_SAFEOBJ_H)
#define LIBSIGCOMP_SAFEOBJ_H

#include "libsigcomp_config.h"
#include "Mutex.h"

__NS_DECLARATION_BEGIN__

class SafeObject
{
public:
	SafeObject();
	
protected:
	void lock();
	void unlock();

private:
	Mutex mutex;
};

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_SAFEOBJ_H
