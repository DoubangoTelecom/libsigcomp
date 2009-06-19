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

#ifndef _RFC3174_US_SHA1_H_
#define _RFC3174_US_SHA1_H_

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>

#if (defined(WIN32) || defined(_WIN32_WCE))
	typedef __int16 int_least16_t;
#else
#	include <stdint.h>
#endif

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError       /* called Input after Result */
};
#endif
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
    t_uint32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

    t_uint32 Length_Low;            /* Message length in bits      */
    t_uint32 Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    int_least16_t Message_Block_Index;
    t_uint8 Message_Block[64];      /* 512-bit message blocks      */

    t_int32 Computed;               /* Is the digest computed?         */
    t_int32 Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

/*
 *  Function Prototypes
 */

t_int32 SHA1Reset(  SHA1Context *);
t_int32 SHA1Input(  SHA1Context *,
                const t_uint8 *,
                unsigned t_int32);
t_int32 SHA1Result( SHA1Context *,
                t_uint8 Message_Digest[SHA1HashSize]);
void SHA1Final(t_uint8 *Message_Digest, 
			   SHA1Context *context);



#endif // _RFC3174_US_SHA1_H_