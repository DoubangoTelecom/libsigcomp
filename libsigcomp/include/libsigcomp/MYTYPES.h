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

#if !defined(LIBSIGCOMP_MYTYPES_H)
#define LIBSIGCOMP_MYTYPES_H

#ifdef _MSC_VER
	#if !defined(int8_t)
	typedef char int8_t;
	#endif

	#if !defined(uint8_t)
	typedef unsigned char  uint8_t;
	#endif

	#if !defined(byte_t)
	typedef unsigned char  byte_t;
	#endif

	#if !defined(int16_t)
	typedef __int16  int16_t;
	#endif

	#if !defined(uint16_t)
	typedef unsigned short  uint16_t;
	#endif

	#if !defined(int32_t)
	typedef __int32  int32_t;
	#endif

	#if !defined(uint32_t)
	typedef unsigned int  uint32_t;
	#endif

	#if !defined(int64_t)
	typedef __int64  int64_t;
	#endif

	#if !defined(uint64_t)
	typedef unsigned __int64 uint64_t;	
	#endif
#else
	#include <stdint.h>
#endif


	#if !defined(LPVOID)
	typedef void  *LPVOID;
	#endif
	
	#if !defined(LPCVOID)
	typedef const void  *LPCVOID;
	#endif

#endif // LIBSIGCOMP_MYTYPES_H