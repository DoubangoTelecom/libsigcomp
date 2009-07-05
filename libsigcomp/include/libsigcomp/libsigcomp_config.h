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

#ifndef LIBSIGCOMP_CONFIG_H
#define LIBSIGCOMP_CONFIG_H


#if !defined(LIBSIGCOMP_EXPORTS)
#	define LIBSIGCOMP_IMPORTS
#endif


#if defined(LIBSIGCOMP_EXPORTS)
# 	define LIBSIGCOMP_API __declspec(dllexport)
//#	define ZLIB_DLL
#elif defined(LIBSIGCOMP_IMPORTS)
# 	define LIBSIGCOMP_API __declspec(dllimport)
#else
# define LIBSIGCOMP
#endif

//
//	Using Namespace or not?
//
#define USE_NS			1
#define MY_FAVORITE_NS	sigcomp	// change this if you have conflict

#if USE_NS
#	define __NS_DECLARATION_BEGIN__	namespace MY_FAVORITE_NS {
#	define __NS_DECLARATION_END__	}
#else
#	define __NS_DECLARATION_BEGIN__
#	define __NS_DECLARATION_END__
#endif

//
// Disable some well-known warnings
//
#ifdef _MSC_VER
#	pragma warning ( disable : 4267 )
#	pragma warning ( disable : 4275 )
#	pragma warning ( disable : 4244 )
#	pragma warning ( disable : 4018 )
#endif

//
//	Windows Mobile specific
//
//#define NO_ERRNO_H 1

//
//DEFLATE block type 01 (data compressed with fixed Huffman codes)*/
//
#define FORCE_STATIC /*zlib*/

//
//	Your System already has zlib?
//
#if defined(__SYMBIAN32__) || defined(TOTO)
#	define HAS_ZLIB		1
#else
#	define HAS_ZLIB		0
#endif

//
//	Nack - RFC 4077
//
#define NACK_VERSION			0x01
#define NACK_MAX_HISTORY_SIZE	0x0a

//
//	SAFE DELETE
//
#define SAFE_DELETE_PTR(ptr) {if(ptr){delete ptr; ptr=NULL;}}
#define SAFE_DELETE_ARRAY(ptr) {if(ptr){delete []ptr; ptr=NULL;}}
#define SAFE_CLEAR_LIST(list, it)											\
	{																		\
		for ( it=list.begin(); it!=list.end(); it++ )						\
			SAFE_DELETE_PTR(*it);											\
		list.clear();														\
	}

#define SAFE_CLEAR_MAP(map, it)											\
	{																	\
		for ( it=map.begin(); it!=map.end(); it++ )						\
			SAFE_DELETE_PTR(it->second);								\
		map.clear();													\
	}
#endif // LIBSIGCOMP_CONFIG_H
