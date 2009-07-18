#if !defined(LIBSIGCOMP_SIGCOMP_HEADERS_H)
#define LIBSIGCOMP_SIGCOMP_HEADERS_H

#include "libsigcomp_config.h"

/*
 0             7 8            15
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |       UDVM_memory_size        |  0 - 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |        cycles_per_bit         |  2 - 3
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |        SigComp_version        |  4 - 5
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |    partial_state_ID_length    |  6 - 7
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |         state_length          |  8 - 9
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                               |
 :           reserved            :  10 - 31
 |                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define UDVM_HEADER_UDVM_MEMORY_SIZE_INDEX				0
#define UDVM_HEADER_CYCLES_PER_BIT_INDEX				2
#define UDVM_HEADER_SIGCOMP_VERSION_INDEX				4
#define UDVM_HEADER_PARTIAL_STATE_ID_LENGTH_INDEX		6
#define UDVM_HEADER_STATE_LENGTH_INDEX					8
#define UDVM_HEADER_RESERVED_INDEX						10


/*
0             7 8            15
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        byte_copy_left         |  64 - 65
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        byte_copy_right        |  66 - 67
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        input_bit_order        |  68 - 69
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+					
|        stack_location         |  70 - 71
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

0             7 8            15
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         reserved        |F|H|P|  68 - 69
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define UDVM_HEADER_BYTE_COPY_LEFT_INDEX				64
#define UDVM_HEADER_BYTE_COPY_RIGHT_INDEX				66
#define UDVM_HEADER_INPUT_BIT_ORDER_INDEX				68
#define UDVM_HEADER_STACK_LOCATION_INDEX				70


#endif // LIBSIGCOMP_SIGCOMP_HEADERS_H