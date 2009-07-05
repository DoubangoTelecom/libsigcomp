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

	#if !defined(LPVOID)
	typedef void  *LPVOID;
	#endif
	
	#if !defined(LPCVOID)
	typedef const void  *LPCVOID;
	#endif

	#if !defined(t_int8)
	typedef char t_int8;
	#endif

	#if !defined(t_uint8)
	typedef unsigned char  t_uint8;
	#endif

	#if !defined(t_byte)
	typedef unsigned char  t_byte;
	#endif

	#if !defined(t_int16)
	typedef __int16  t_int16;
	#endif

	#if !defined(t_uint16)
	typedef unsigned short  t_uint16;
	#endif

	#if !defined(t_int32)
	typedef __int32  t_int32;
	#endif

	#if !defined(t_uint32)
	typedef unsigned int  t_uint32;
	#endif

	#if !defined(t_int64)
	typedef __int64  t_int64;
	#endif

	#if !defined(t_uint64)
	typedef unsigned __int64 t_uint64;	
	#endif

#endif // LIBSIGCOMP_MYTYPES_H