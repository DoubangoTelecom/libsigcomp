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

#if !defined(LIBSIGCOMP_SIPRFC5049_H)
#define LIBSIGCOMP_SIPRFC5049_H

#include <libsigcomp/libsigcomp_config.h>

/*****
Applying Signaling Compression (SigComp)
                to the Session Initiation Protocol (SIP)
*****/
//  4.1.  decompression_memory_size (DMS) for SIP/SigComp
#define SIP_RFC5049_DECOMPRESSION_MEMORY_SIZE 8192

// 4.2.  state_memory_size (SMS) for SIP/SigComp (per compartment)
#define SIP_RFC5049_STATE_MEMORY_SIZE 2048

// 4.3.  cycles_per_bit (CPB) for SIP/SigComp
#define SIP_RFC5049_CYCLES_PER_BIT 16

// 4.4.  SigComp_version (SV) for SIP/SigComp
#define SIP_RFC5049_SIGCOMP_VERSION 0x02 // (at least SigComp + NACK)

// 4.5.  locally available state (LAS) for SIP/SigComp
// Minimum LAS for SIP/SigComp: the SIP/SDP static dictionary as defined
   //in [RFC3485].


#endif //LIBSIGCOMP_SIPRFC5049_H