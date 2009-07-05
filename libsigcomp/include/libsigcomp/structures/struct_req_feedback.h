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

#if !defined(LIBSIGCOMP_STRUCTREQ_FEEDBACK_H)
#define LIBSIGCOMP_STRUCTREQ_FEEDBACK_H

#include <libsigcomp/mytypes.h>

__NS_DECLARATION_BEGIN__

// DON'T USE bit fields: http://en.wikipedia.org/wiki/Bit_field

/*
0   1   2   3   4   5   6   7
+---+---+---+---+---+---+---+---+
|     reserved      | Q | S | I |  requested_feedback_location
+---+---+---+---+---+---+---+---+
|                               |
:    requested feedback item    :  if Q = 1
|                               |
+---+---+---+---+---+---+---+---+
*/
typedef struct rfc3320_struct_req_feedback
{
	rfc3320_struct_req_feedback(){
		reset();
	}
	inline void setQ(t_uint8 q){ Q=q; }
	inline t_uint8 getQ(){ return Q; }

	inline void setS(t_uint8 s){ S=s; }
	inline t_uint8 getS(){ return S; }

	inline void setI(t_uint8 i){ I=i; }
	inline t_uint8 getI(){ return I; }

	inline SigCompBuffer* getReqFeedItem() { return &req_feed_item; }

	inline void reset(){
		req_feed_item.freeBuff();
		req_feed_item.reset();
		Q=S=I=0;
	}

private:
	/*The Q-bit indicates whether a requested feedback item is present or
	not.*/
	t_uint8 Q;
	/*The compressor sets the S-bit to 1 if it does not wish (or no longer
	wishes) to save state information at the receiving endpoint and also
	does not wish to access state information that it has previously saved.*/
	t_uint8 S;
	/*Similarly the compressor sets the I-bit to 1 if it does not wish (or
	no longer wishes) to access any of the locally available state items
	offered by the receiving endpoint.*/
	t_uint8 I;

	SigCompBuffer req_feed_item;
}
struct_req_feedback, *lpstruct_req_feedback;

__NS_DECLARATION_END__

#endif //LIBSIGCOMP_STRUCTREQ_FEEDBACK_H