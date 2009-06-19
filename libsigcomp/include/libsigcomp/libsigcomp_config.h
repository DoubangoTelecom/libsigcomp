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

#ifndef LIBSIGCOMP_CONFIG_H
#define LIBSIGCOMP_CONFIG_H


#if defined(_MSC_VER) && !defined(LIBSIGCOMP_EXPORTS)
#	define LIBSIGCOMP_IMPORTS
#endif


#if (defined(WIN32) || defined(_WIN32_WCE)) && defined(LIBSIGCOMP_EXPORTS)
# 	define LIBSIGCOMP_API __declspec(dllexport)
#	define ZLIB_DLL
#elif (defined(WIN32) || defined(_WIN32_WCE)) && defined(LIBSIGCOMP_IMPORTS)
# 	define LIBSIGCOMP_API __declspec(dllimport)
#else
# define LIBSIGCOMP
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
//	Nack - RFC 4077
//
#define NACK_VERSION			0x01
#define NACK_MAX_HISTORY_SIZE	0x0a

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