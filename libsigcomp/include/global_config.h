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

#ifndef LIBSIGCOMP_GLOBAL_CONFIG_H
#define LIBSIGCOMP_GLOBAL_CONFIG_H

#if defined(_MSC_VER) && !defined(LIBSIGCOMP_EXPORTS)
	#error Under Visual Studio you MUST define 'LIBSIGCOMP_EXPORTS' in order to generate your DLL.
#endif

#if defined(WIN32) || defined(_WIN32_WCE)
#	define USE_WIN32_THREADS
#	include <windows.h>
#else
#	define USE_PTHREAD_TRHREADS
#endif

#include<assert.h>

#endif // LIBSIGCOMP_GLOBAL_CONFIG_H