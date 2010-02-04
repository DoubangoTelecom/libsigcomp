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

#include "global_config.h"

#include "SigCompUDVM.h"
#include "rfc3174_us-sha1.h"
#include "rfc1662_ppfcs16.h"

#include <math.h>
#include <algorithm>

//
// IMPORTANT: MSBs are stored before LSBs in the UDVM memory --> BIG ENDIAN
//

#define F_BIT_MSB_TO_LSB 0
#define F_BIT_LSB_TO_MSB 1
#define H_BIT_MSB_TO_LSB F_BIT_MSB_TO_LSB
#define H_BIT_LSB_TO_MSB F_BIT_LSB_TO_MSB

#define CEILLINGLOG2(x) ceil( (log((double)x)/log(2.0)) )

//
//	Consume cycles
//
#define CONSUME_CYCLES(cycles)									\
	this->consumed_cycles+= (cycles);							\
	if( this->consumed_cycles > this->maximum_UDVM_cycles )		\
	{															\
		this->createNackInfo(CYCLES_EXHAUSTED);					\
		return false;											\
	}

/**
	This structure is used to keep index-value pairs after sorting.
*/
typedef struct tag_IndexValuePair
{
	uint16_t index;
	uint16_t value;
}IndexValuePair;

/**
 Sort Ascending predicate.
*/
struct SortAscendingPredicate
{
	bool operator()(IndexValuePair el1, IndexValuePair el2) const
	{
		return (el1.value < el2.value);
	}
};
/**
 Sort Descending predicate.
*/
struct SortDescendingPredicate
{
	bool operator()(IndexValuePair el1, IndexValuePair el2) const
	{
		return (el1.value > el2.value);
	}
};

__NS_DECLARATION_BEGIN__
///////////////////////////////////////////////////
//
//
//
//	RFC3320-Section-9.  UDVM Instruction Set
//

/**
DECOMPRESSION-FAILURE
Reference:  RFC3320 Section 9.4.1

This instruction triggers a manual decompression failure.  This is useful if the UDVM bytecode discovers that it
cannot successfully decompress the message (e.g., by using the CRC instruction).

@returns true if succeed, otherwise return false 
*/
bool SigCompUDVM::EXEC_INST__DECOMPRESSION_FAILURE() 
{ 
	//CONSUME_CYCLES(1);
	this->createNackInfo(USER_REQUESTED);
	return false; 
}

/**
AND ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.1
Formula: [operand_1 := operand_1 & operand_2]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 Second operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__AND(uint16_t operand_1, uint16_t operand_2)
{
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL( operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) & operand_2) );

	return true; 
}

/**
OR ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.1
Formula: [operand_1 := operand_1 | operand_2]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 Second operand

@returns true if succeed, otherwise return false
*/

bool SigCompUDVM::EXEC_INST__OR(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL( operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) | operand_2) );

	return true; 
}

/**
NOT ($operand_1)
Reference:  RFC3320 Section 9.1.1
Formula: [operand_1 := ~operand_1]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__NOT(uint16_t operand_1)
{ 
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL( operand_1, 
		~( UDVM_GET_2BYTES_VAL(operand_1) ) );

	return true;
}

/**
LSHIFT ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.1
Formula: [LSHIFT (m, n) := m * 2^n (modulo 2^16)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__LSHIFT(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	// (m * 2^n)  == (m<<n)
	// (2^16) === 65536
	UDVM_SET_2BYTES_VAL( operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) << operand_2) );

	return true; 
}

/**
RSHIFT ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.1
Formula: [RSHIFT (m, n) := floor(m / 2^n)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__RSHIFT(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);
	
	// floor(m / 2^n) == (m>>n)
	UDVM_SET_2BYTES_VAL(operand_1,
		(UDVM_GET_2BYTES_VAL(operand_1) >> operand_2) );

	return true; 
}

/**
ADD ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.2
Formula: [ADD (m, n) := m + n (modulo 2^16)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__ADD(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL(operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) + operand_2) );

	return true; 
}

/**
SUBTRACT ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.2
Formula: [SUBTRACT (m, n)  := m - n (modulo 2^16)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__SUBTRACT(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL(operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) - operand_2) );

	return true; 
}

/**
MULTIPLY ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.2
Formula: [MULTIPLY (m, n)  := m * n (modulo 2^16)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__MULTIPLY(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	UDVM_SET_2BYTES_VAL(operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) * operand_2) );

	return true; 
}

/**
DIVIDE ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.2
Formula: [DIVIDE (m, n) := floor(m / n)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand. Decompression failure occurs if this operand is zero.

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__DIVIDE(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	if(!operand_2)
	{
		this->createNackInfo(DIV_BY_ZERO);
		return false;
	}

	UDVM_SET_2BYTES_VAL(operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) / operand_2) );

	return true; 
}

/**
REMAINDER ($operand_1, %operand_2)
Reference:  RFC3320 Section 9.1.2
Formula: [REMAINDER (m, n) := m - n * floor(m / n)]

@param operand_1 2-byte value encoded by the operand. After the operation is complete, the 2-byte word at the memory address specified by
   this operand is overwritten with the result
@param operand_2 2-byte value encoded by the operand. Decompression failure occurs if this operand is zero.

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__REMAINDER(uint16_t operand_1, uint16_t operand_2)
{ 
	CONSUME_CYCLES(1);

	if(!operand_2)
	{
		this->createNackInfo(DIV_BY_ZERO);
		return false;
	}

	UDVM_SET_2BYTES_VAL(operand_1, 
		(UDVM_GET_2BYTES_VAL(operand_1) % operand_2) );

	return true;
}

/**
SORT-ASCENDING (%start, %n, %k)
Reference:  RFC3320 Section 9.1.3

This instruction sort lists of 2-byte words in ascending order.

@param start The starting memory address of the block of data to be sorted
@param n Number of lists
@param k Lists length (2-byte words)

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__SORT_ASCENDING(uint16_t start, uint16_t n, uint16_t k)
{ 
	CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); // 1 + k * (ceiling(log2(k)) + n)

	bool segfault = false;
	uint16_t* list_temp = NULL;
	IndexValuePair* list1_values = NULL;

	if( this->memory.getSize() <= (start+(n*k*2)) ){ segfault=true; goto __SEGFAULT; };

	//
	// Create FirstList with key-value pairs
	//
	list1_values = new IndexValuePair[k];
	if(!list1_values) { segfault=true; goto __SEGFAULT; };
	for(int j=0, pos=0; pos<k; j+=2,pos++)
	{
		list1_values[pos].index = pos;
		list1_values[pos].value = *((uint16_t*)this->memory.getBuffer(start+j));
	}

	//
	// Sort Fisrt List Values
	//
	std::sort(list1_values, (list1_values+k), SortAscendingPredicate());


	// Sort all lists
	list_temp = new uint16_t[k];
	if(!list1_values) { segfault=true; goto __SEGFAULT; };
	for( uint16_t list_index=0; list_index<n; list_index++ )
	{
		uint16_t* list_start = (uint16_t*)this->memory.getBuffer( start + (list_index*k*2) );
		::memmove(list_temp, list_start, k*2);
		for( uint16_t list_el=0; list_el<k; list_el++ )
		{
			list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
		}
	}

	SAFE_DELETE_ARRAY(list_temp);
	SAFE_DELETE_ARRAY(list1_values);

__SEGFAULT:
	if(segfault)
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}

	return true;
}

/**
SORT-ASCENDING (%start, %n, %k)
Reference:  RFC3320 Section 9.1.3

This instruction sort lists of 2-byte words in descending order.

@param start The starting memory address of the block of data to be sorted
@param n Number of lists
@param k Lists length (2-byte words)

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__SORT_DESCENDING(uint16_t start, uint16_t n, uint16_t k)
{ 
	CONSUME_CYCLES(( 1 + k *(CEILLINGLOG2(k) + n) )); // 1 + k * (ceiling(log2(k)) + n)

	bool segfault = false;
	IndexValuePair* list1_values = NULL;
	uint16_t* list_temp = NULL;

	if( this->memory.getSize() <= (start+(n*k*2)) ) { segfault=true; goto __SEGFAULT; };

	//
	// Create FirstList with key-value pairs
	//
	list1_values = new IndexValuePair[k];
	if(!list1_values) { segfault=true; goto __SEGFAULT; };
	for(int j=0, pos=0; pos<k; j+=2,pos++)
	{
		list1_values[pos].index = pos;
		list1_values[pos].value = *((uint16_t*)this->memory.getBuffer(start+j));
	}

	//
	// Sort Fisrt List Values
	//
	std::sort(list1_values, (list1_values+k), SortDescendingPredicate());


	// Sort all lists
	list_temp = new uint16_t[k];
	if(!list1_values) { segfault=true; goto __SEGFAULT; };
	for( uint16_t list_index=0; list_index<n; list_index++ )
	{
		uint16_t* list_start = (uint16_t*)this->memory.getBuffer( start + (list_index*k*2) );
		::memmove(list_temp, list_start, k*2);
		for( uint16_t list_el=0; list_el<k; list_el++ )
		{
			list_start[(list_el)] = list_temp[ list1_values[list_el].index ];
		}
	}

	SAFE_DELETE_ARRAY(list_temp);
	SAFE_DELETE_ARRAY(list1_values);

__SEGFAULT:
	if(segfault)
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}

	return true;
}

/**
SHA-1 (%position, %length, %destination)
Reference:  RFC3320 Section 9.1.4

This instruction calculates a 20-byte SHA-1 hash [RFC-3174] over the specified area of UDVM memory.

@param position the starting memory address
@param length the length of the byte string over which the SHA-1 hash is calculated
@param destination the starting address to which the resulting 20-byte hash will be copied

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__SHA_1(uint16_t position, uint16_t length, uint16_t destination)
{ 
	CONSUME_CYCLES(1+length);

	bool ok = true;
	uint8_t* data = NULL;

	if(!length || ((destination+length)>this->memory.getSize()))
	{
		this->createNackInfo(INVALID_OPERAND);
		ok = false;
		goto bail;
	}
	
	data = new uint8_t[length]; memset(data, 0, length);
	ok &= this->bytecopy_from(data, position, length);

	//
	// Compute SHA-1
	//
	SHA1Context sha;
	int32_t err;
	uint8_t Message_Digest[SHA1HashSize];
	if( (err = ::SHA1Reset(&sha)) )
	{
		this->createNackInfo(INTERNAL_ERROR);
		ok = false;
		goto bail;
	}
	if ( (err = ::SHA1Input(&sha, data, length)) )
	{
		this->createNackInfo(INTERNAL_ERROR);
		ok = false;
		goto bail;
	}

	if( (err = ::SHA1Result(&sha, Message_Digest)) )
	{
		this->createNackInfo(INTERNAL_ERROR);
		ok = false;
		goto bail;
	}

	//
	// Copy sha1 result to udvm memory
	//
	ok &= this->bytecopy_to(destination, Message_Digest, SHA1HashSize);

bail:
	SAFE_DELETE_ARRAY(data);

	return ok; 
}

/**
LOAD(%address, %value)
Reference:  RFC3320 Section 9.2.1

This instruction sets a 2-byte word to a certain specified value
As usual, MSBs are stored before LSBs in the UDVM memory.

@param address specifies the starting address of a 2-byte word
@param value specifies the value to be loaded into this word

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__LOAD(uint16_t address, uint16_t value)
{ 
	CONSUME_CYCLES(1);

	if( address >= this->memory.getSize() )
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}

	UDVM_SET_2BYTES_VAL(address, value);

	return true; 
}

/**
MULTILOAD(%address, #n, %value_0, ..., %value_n-1)
Reference:  RFC3320 Section 9.2.2

This instruction sets a contiguous block of 2-byte words in the UDVM memory to specified values.
value_0 through to value_n-1 specify the values to load into these words (in the same order as
	they appear in the instruction).

@param address  starting address of the contiguous 2-byte words
@param n number of values

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__MULTILOAD(uint16_t address, uint16_t n)
{ 
	CONSUME_CYCLES(1+n);

	if( (address + (2*n)) > this->executionPointer && (this->executionPointer >= address) )
	{
		this->createNackInfo(MULTILOAD_OVERWRITTEN);
		return false;
	}

	// FIXME: check for overwritten
	for(uint16_t index=0,_address=address; index<n; index++,_address+=2)
	{
		uint16_t value_n = this->opget_multitype_param();
		UDVM_SET_2BYTES_VAL( _address, value_n );
	}

	return true; 
}

/**
PUSH (%value)
Reference:  RFC3320 Section 9.2.3

This instruction pushes the value specified by its operand on the stack.

@param value 2-byte word to push

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__PUSH(int16_t value /*= -1*/)
{ 
	bool callback = (value>=0);
	if(!callback)
	{
		value = this->opget_multitype_param();
	}

	CONSUME_CYCLES(callback?0:1);

	uint16_t stack_location = UDVM_GET_2BYTES_VAL(UDVM_HEADER_STACK_LOCATION_INDEX);
	uint16_t stack_fill = UDVM_GET_2BYTES_VAL(stack_location);

	// copying the value to stack[stack_fill]
	// stack[n] = stack_location+2*n+2
	UDVM_SET_2BYTES_VAL((stack_location+(2*stack_fill)+2), value);

	// increasing stack_fill by 1
	UDVM_SET_2BYTES_VAL(stack_location, (stack_fill+1));

	return true; 
}

/**
POP (%address)
Reference:  RFC3320 Section 9.2.3

This instruction pops a value from the stack and then copies the value to the specified memory address.

@param value 2-byte word to pop from the stack

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__POP(uint16_t* value /*= NULL*/)
{ 
	bool callback = (value != NULL);

	CONSUME_CYCLES(callback?0:1);

	uint16_t address = callback?0:this->opget_multitype_param();

	uint16_t stack_location = UDVM_GET_2BYTES_VAL(UDVM_HEADER_STACK_LOCATION_INDEX);
	uint16_t stack_fill = UDVM_GET_2BYTES_VAL(stack_location);
	uint16_t _value = 0;

	/*Decompression failure occurs if stack_fill is
	zero at the commencement of a popping operation.  POP and RETURN pop
	values from the stack.*/
	if(stack_fill == 0)
	{
		// FIXME: DECOMPRESSION FAILURE
		_value = 0;
		goto end;
	}

	/*"Popping" a value from the stack is an abbreviation for decreasing
	stack_fill by 1, and then using the value stored in stack[stack_fill].*/
	--stack_fill;
	UDVM_SET_2BYTES_VAL(stack_location, stack_fill);
	// stack[n] = stack_location+2*n+2
	_value = UDVM_GET_2BYTES_VAL( (stack_location + (2*stack_fill) + 2) );

end:
	if(callback){
		*value = _value;
	}else{
		UDVM_SET_2BYTES_VAL(address, _value);
	}

	return true; 
}

/**
COPY(%position, %length, %destination)
Reference:  RFC3320 Section 9.2.4

This instruction is used to copy a string of bytes from one part of the UDVM memory to another.

@param position specifies the memory address of the first byte in the string to be copied
@param length specifies the number of bytes to be copied
@param destination gives the address to which the first byte in the string will be copied

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__COPY(uint16_t position, uint16_t length, uint16_t destination)
{ 
	bool ok = true;

	CONSUME_CYCLES(1+length);
	
	if( (position + length)> this->memory.getSize() || (destination + length)> this->memory.getSize() )
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}

	/*The COPY instruction is used to copy a string of bytes from one part
	of the UDVM memory to another.*/
	ok &= this->bytecopy_self(destination, position, length);

	return ok;
}

/**
COPY-LITERAL(%position, %length, $destination)
Reference:  RFC3320 Section 9.2.5

The COPY-LITERAL instruction behaves as a COPY instruction except
   that after copying is completed, the value of the destination operand
   is replaced by the address to which the next byte of data would be
   copied.

@param position specifies the memory address of the first byte in the string to be copied
@param length specifies the number of bytes to be copied
@param destination gives the address to which the first byte in the string will be copied

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__COPY_LITERAL(uint16_t position, uint16_t length, uint16_t destination)
{ 
	CONSUME_CYCLES(1+length);

	uint16_t destination_index = UDVM_GET_2BYTES_VAL(destination);
	bool ok = this->bytecopy_self(destination_index, position, length);
	if(ok){ // set next byte
		UDVM_SET_2BYTES_VAL(destination, destination_index);
	}

	return ok; 
}

/**
COPY-OFFSET(%offset, %length, $destination)
Reference:  RFC3320 Section 9.2.6

This instruction behaves as a COPY-LITERAL instruction
   except that an offset operand is given instead of a position operand.

@param offset the offset value
@param length specifies the number of bytes to be copied
@param destination gives the address to which the first byte in the string will be copied

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__COPY_OFFSET(uint16_t offset, uint16_t length, uint16_t destination)
{ 
	CONSUME_CYCLES(1+length);

	uint16_t DEST = UDVM_GET_2BYTES_VAL(destination);
	uint16_t LEFT = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_LEFT_INDEX);
	uint16_t RIGTH = UDVM_GET_2BYTES_VAL(UDVM_HEADER_BYTE_COPY_RIGHT_INDEX);
	int32_t position = -1;

	/*
	DEST: ses
	D: distance between LEFT and DEST
	O: offset
	T: total size between LEFT and RIGTH

	[*****
	case 1:
	-----LEFT--------DEST------------RIGTH----
	<-----D---->
	<--O->
	<---------------T------------>
	****]
	[*****
	case 2:
	-----LEFT--------DEST------------RIGTH----
	<-----D---->
	<--------O-------->
	<---------------T------------>
	****]
	[*****
	case 3:
	-------DEST-----LEFT-------------RIGTH----
	****]
	*/
	int16_t D = (DEST - LEFT);
	int16_t T = (RIGTH - LEFT);
	uint16_t O = offset;

	if( D>=0 && O<=D )
	{
		//case 1: easy case
		position = (DEST-O);
	}
	else if( D>=0 && O>D )
	{
		//case 2:
		double PAD = (D + ((ceil(((double)O-(double)D)/(double)T))*T))-O;
		position = LEFT+PAD;
	}
	else if( D<0 )
	{
		//case 3:
		position = DEST-O;
	}

	// Check position
	if(position<0)
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}

	// EXEC_INST__COPY_LITERAL
	uint16_t destination_index = UDVM_GET_2BYTES_VAL(destination);
	bool ok = this->bytecopy_self(destination_index, position, length);
	if(ok){
		UDVM_SET_2BYTES_VAL(destination, destination_index);
	}

	return true; 
}

/**
MEMSET(%address, %length, %start_value, %offset)
Reference:  RFC3320 Section 9.2.7
Formula: Seq[n] := (start_value + n * offset) modulo 256

This instruction initializes an area of UDVM memory to a specified sequence of values.

@param address destination address
@param length number of 1-byte values to set
@param start_value starting value
@param offset offset used for computation

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__MEMSET(uint16_t address, uint16_t length, uint16_t start_value, uint16_t offset)
{ 
	CONSUME_CYCLES(1+length);

	/*The values Seq[0] to Seq[length - 1] inclusive are each interpreted
	as a single byte, and then concatenated to form a byte string where
	the first byte has value Seq[0], the second byte has value Seq[1] and
	so on up to the last byte which has value Seq[length - 1].*/
	uint8_t* seqs_temp = new uint8_t[length];
	if(!seqs_temp) return false;
	for(uint16_t n=0; n<length; n++)
	{
		seqs_temp[n] = (start_value + n * offset)%256;
	}
	/*The string is then byte copied into the UDVM memory beginning at the
	memory address specified as an operand to the MEMSET instruction,
	obeying the rules of Section 8.4.
	*/
	bool ok = this->bytecopy_to(address, seqs_temp, length);

	SAFE_DELETE_ARRAY(seqs_temp);

	return ok; 
}

/**
JUMP (@address)
Reference:  RFC3320 Section 9.3.1

This instruction moves program execution to the specified memory address.
Decompression failure occurs if the value of the address operand lies
   beyond the overall UDVM memory size.

@param address defines the address to jump to

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__JUMP(int16_t address/*=-1*/)
{
	bool callback = (address>=0);
	CONSUME_CYCLES(callback?0:1);

	if(!callback)
	{
		address = this->opget_address_param(this->last_memory_address_of_instruction);
	}

	if( address > this->memory.getSize()  )
	{
		this->createNackInfo(SEGFAULT);
		return false;
	}
	this->executionPointer = address;

	return true; 
}

/**
COMPARE(%value_1, %value_2, @address_1, @address_2, @address_3)
Reference:  RFC3320 Section 9.3.2

This instruction compares two operands and then jumps to one of three specified memory addresses depending on the result.
if(value_1 < value_2) --> address_1
elif(value_1 = value_2) --> address_2
elif(value_1 > value_2) --> address_3

@param value_1 first value to compare
@param value_2 second value to compare
@param address_1 address to jump to if (value_1 < value_2)
@param address_2 address to jump to if (value_1 = value_2)
@param address_3 address to jump to if (value_1 > value_2)

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__COMPARE(uint16_t value_1, uint16_t value_2, uint16_t address_1, uint16_t address_2, uint16_t address_3)
{ 
	CONSUME_CYCLES(1);

	bool ok = true;

	if(value_1 < value_2){
		ok &= this->EXEC_INST__JUMP(address_1);
		goto end;
	}

	if(value_1 == value_2){
		ok &= this->EXEC_INST__JUMP(address_2);
		goto end;
	}

	if(value_1 > value_2){
		ok &= this->EXEC_INST__JUMP(address_3);
		goto end;
	}

end:
	return ok; 
}

/**
CALL(@address)
Reference:  RFC3320 Section 9.3.3

This instruction finds the memory address of the instruction immediately following 
the CALL instruction and pushes this 2-byte value on the stack, ready for later retrieval.  
It then continues instruction execution at the memory address specified by the address operand.

@param address next address

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__CALL(uint16_t address)
{ 
	CONSUME_CYCLES(1);
	
	this->EXEC_INST__PUSH(this->executionPointer);
	bool ok = this->EXEC_INST__JUMP(address);

	return true; 
}

/**
RETURN
Reference:  RFC3320 Section 9.3.3

This instruction pops a value from the stack and then continues instruction 
execution at the memory address just popped.

@returns true if succeed, otherwise return false 
*/
bool SigCompUDVM::EXEC_INST__RETURN()
{ 
	CONSUME_CYCLES(1);

	uint16_t value = 0;
	bool ok = true;
	if( (ok = this->EXEC_INST__POP(&value)) )
	{
		ok &= this->EXEC_INST__JUMP(value);
	}

	return true; 
}

/**
SWITCH(#n, %j, @address_0, @address_1, ... , @address_n-1)
Reference:  RFC3320 Section 9.3.4

This instruction performs a conditional jump based on the value of one of its operands.
Decompression failure occurs if j specifies a value of n or more, or
   if the address lies beyond the overall UDVM memory size.

@param n number of possibilities
@param j THE possibility

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__SWITCH(uint16_t n, uint16_t j)
{ 
	uint16_t next = 0;
	bool ok = true;

	CONSUME_CYCLES(1+n);

	/*Decompression failure occurs if j specifies a value of n or more*/
	if(j >= n)
	{
		this->createNackInfo(SWITCH_VALUE_TOO_HIGH);
		ok = false;
		goto end;
	}

	do
	{
		next = this->opget_address_param(this->last_memory_address_of_instruction);
	}
	while(j--);

end:
	if(ok)
	{
		ok = this->EXEC_INST__JUMP(next);
	}

	return ok; 
}

/**
CRC(%value, %position, %length, @address)
Reference:  RFC3320 Section 9.3.5

This instruction verifies a string of bytes using a 2-byte CRC.
The CRC value is computed exactly as defined for the 16-bit FCS calculation in [RFC-1662].

@param value contains the expected integer value of the 2-byte CRC
@param position defines the position of the string of bytes over which the CRC is evaluated
@param length defines the length of the string of bytes over which the CRC is evaluated
@param address the address to jump to if the calculated CRC value do not match

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__CRC(uint16_t value, uint16_t position, uint16_t length, uint16_t address)
{ 
	bool ok = true;

	CONSUME_CYCLES(1+length);

	uint8_t* data = new uint8_t[length];
	if(!data)
	{
		this->createNackInfo(INTERNAL_ERROR);
		return false;
	}

	// copy data
	ok &= this->bytecopy_from(data, position, length);

	/*The CRC value is computed exactly as defined for the 16-bit FCS
	calculation in [RFC-1662]*/
	uint16_t crc_value = ::pppfcs16(PPPINITFCS16, data, length);

	// delete data
	SAFE_DELETE_ARRAY(data);

	/* If the calculated CRC matches the expected value then the UDVM
	continues instruction execution at the following instruction.
	Otherwise the UDVM jumps to the memory address specified by the
	address operand.*/
	if(value != crc_value)
	{
		this->EXEC_INST__JUMP(address);
	}

	return ok; 
}

/**
INPUT-BITS (%length, %destination, @address)
Reference:  RFC3320 Section 9.4.2

This instruction requests a certain number of bytes of compressed data from the decompressor dispatcher.

@param length indicates the requested number of bytes of compressed data
@param destination specifies the starting memory address to which they should be copied
@param address defines the address to jump to if the instruction requests data that lies beyond the end of the SigComp message

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__INPUT_BYTES(uint16_t length, uint16_t destination, uint16_t address)
{ 
	bool ok = true;

	CONSUME_CYCLES(1+length);

	/*If the INPUT-BYTES is encountered after an INPUT-BITS or an INPUT-
	HUFFMAN instruction has been used, and the dispatcher currently holds
	a fraction of a byte, then the fraction MUST be discarded before any
	data is passed to the UDVM.  The first byte to be passed is the byte
	immediately following the discarded data.*/
	this->sigCompMessage->remaining_sigcomp_buffer.discardBits();

	const uint8_t* compressedDataAddress = this->sigCompMessage->remaining_sigcomp_buffer.readBytes(length);
	uint8_t* destinationAddress  = this->memory.getBuffer(destination);
	if(compressedDataAddress)
	{
		ok &= this->bytecopy_to(destination, compressedDataAddress, length);
		if(ok){
			// (8 * n + 1000) * cycles_per_bit
			this->maximum_UDVM_cycles += ((8*length+1000)*this->stateHandler->getSigCompParameters()->getCpbCode());
		}
	}
	else
	{
		/*If the instruction requests data that lies beyond the end of the
		SigComp message, no data is returned.  Instead the UDVM moves program
		execution to the address specified by the address operand.*/
		//cycles++;
		ok &= this->EXEC_INST__JUMP(address);
	}

	return ok;
}

/**
INPUT-BITS(%length, %destination, @address)
Reference:  RFC3320 Section 9.4.3

This instruction requests a certain number of bits of compressed data from the decompressor dispatcher.

@param length indicates the requested number of bits of compressed data
@param destination specifies the starting memory address to which they should be copied
@param address defines the address to jump to if the instruction requests data that lies beyond the end of the SigComp message

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__INPUT_BITS(uint16_t length, uint16_t destination, uint16_t address)
{ 
	/* The input_bit_order register contains the following three flags:
	0             7 8            15
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|         reserved        |F|H|P|  68 - 69
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/

	CONSUME_CYCLES(1);

	bool ok = true;

	const uint16_t input_bit_order = UDVM_GET_2BYTES_VAL(UDVM_HEADER_INPUT_BIT_ORDER_INDEX);
	const uint16_t reserved = (input_bit_order & 0xf8);
	/*Decompression failure occurs if an INPUT-BITS or an INPUT-HUFFMAN
	instruction is encountered and the input_bit_order register does not
	lie between 0 and 7 inclusive.*/
	if(reserved) // MUST BE ZEROS --> Only 3bits --> [0-7]
	{
		this->createNackInfo(BAD_INPUT_BITORDER);
		return false;
	}

	// F and P BITS
	const uint8_t F_BIT = (input_bit_order & 0x0004)?1:0;
	const uint8_t P_BIT = (input_bit_order & 0x0001);

	/*Decompression failure occurs if this operand (length) does not lie between 0
	and 16 inclusive.*/
	if( length<0 || length>16)
	{
		this->createNackInfo(INVALID_OPERAND);
		return false;
	}

	/*P:The P-bit controls the order in which bits are passed from the
	dispatcher to the INPUT instructions
	P=0 --> MSB_TO_LSB
	P=1 --> LSB_TO_MSB*/
	uint8_t& old_P_BIT = this->sigCompMessage->remaining_sigcomp_buffer.getP_BIT();
	if( old_P_BIT != P_BIT )
	{
		/*If the P-bit has changed since the last INPUT instruction, any fraction of a
		byte still held by the dispatcher MUST be discarded (even if the
		INPUT instruction requests zero bits)*/
		this->sigCompMessage->remaining_sigcomp_buffer.discardBits();
		old_P_BIT = P_BIT;
	}

	/*If the instruction requests data that lies beyond the end of the
	SigComp message, no data is returned.  Instead the UDVM moves program
	execution to the address specified by the address operand.*/
	if( (length) > this->sigCompMessage->remaining_sigcomp_buffer.getRemainingBits() )
	{
		ok &= this->EXEC_INST__JUMP(address);
		goto end;
	}

	/*If the F-bit is set to 0, the INPUT-BITS instruction interprets the
	received bits as arriving MSBs first, and if it is set to 1, it
	interprets the bits as arriving LSBs first.
	F=0 --> MSB_TO_LSB
	F=1 --> LSB_TO_MSB*/
	if(P_BIT==P_BIT_MSB_TO_LSB){
		// MSB_TO_LSB
		uint16_t value = this->sigCompMessage->remaining_sigcomp_buffer.readMsbToLsb(length);
		if(F_BIT==F_BIT_LSB_TO_MSB) value = (BINARY_REVERSE_2BYTE(value)>>(16-length));
		UDVM_SET_2BYTES_VAL(destination, value);
	}
	else{
		// LSB_TO_MSB
		uint16_t value = this->sigCompMessage->remaining_sigcomp_buffer.readLsbToMsb(length);
		if(F_BIT==F_BIT_LSB_TO_MSB) value = (BINARY_REVERSE_2BYTE(value)>>(16-length));
		UDVM_SET_2BYTES_VAL(destination, value);
	}

end:
	this->maximum_UDVM_cycles += (length * this->stateHandler->getSigCompParameters()->getCpbValue());
	return ok; 
}

/**
INPUT-HUFFMAN(%destination, @address, #n, %bits_1, %lower_bound_1, %upper_bound_1, %uncompressed_1, ... , %bits_n, %lower_bound_n, %upper_bound_n, %uncompressed_n)
Reference:  RFC3320 Section 9.4.4

This instruction requests a variable number of bits of compressed data from the decompressor dispatcher.  The instruction
initially requests a small number of bits and compares the result against a certain criterion; if the criterion is not met, then
additional bits are requested until the criterion is achieved.

@param destination the udvm destination address
@param address address to jump to if data is requested that lies beyond the end of the SigComp message
@param n additional sets of operands count

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__INPUT_HUFFMAN(uint16_t destination, uint16_t address, uint16_t n)
{ 
	/* The input_bit_order register contains the following three flags:
	0             7 8            15
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|         reserved        |F|H|P|  68 - 69
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	*/
	bool ok = true;

	CONSUME_CYCLES(1+n);

	/*Note that if n = 0 then the INPUT-HUFFMAN instruction is ignored and
	program execution resumes at the following instruction.*/
	if(n == 0){
		//goto end;
		return ok;
	}

	const uint16_t input_bit_order = UDVM_GET_2BYTES_VAL(UDVM_HEADER_INPUT_BIT_ORDER_INDEX);
	const uint16_t reserved = (input_bit_order & 0xf8);
	/*Decompression failure occurs if an INPUT-BITS or an INPUT-HUFFMAN
	instruction is encountered and the input_bit_order register does not
	lie between 0 and 7 inclusive.*/
	if(reserved) // MUST BE ZEROS --> Only 3bits --> [0-7]
	{
		this->createNackInfo(BAD_INPUT_BITORDER);
		return false;
	}

	// H and P
	const uint8_t H_BIT = (input_bit_order & 0x0002)?1:0;
	const uint8_t P_BIT = (input_bit_order & 0x0001);

	/*P:The P-bit controls the order in which bits are passed from the
	dispatcher to the INPUT instructions
	P=0 --> MSB_TO_LSB
	P=1 --> LSB_TO_MSB*/
	uint8_t& old_P_BIT = this->sigCompMessage->remaining_sigcomp_buffer.getP_BIT();
	if( old_P_BIT != P_BIT )
	{
		/*If the P-bit has changed since the last INPUT instruction, any fraction of a
		byte still held by the dispatcher MUST be discarded (even if the
		INPUT instruction requests zero bits)*/
		this->sigCompMessage->remaining_sigcomp_buffer.discardBits();
		old_P_BIT = P_BIT;
	}

	//
	//	HUFFMAN COMPUTATION
	//
	uint16_t bits_j, lower_bound_j, upper_bound_j, uncompressed_j;
	uint16_t bits_total = 0, k = 0, H, J;
	bool criterion_ok = false;

	// 1. Set j := 1 and set H := 0.
	for(J = 1, H = 0; J<=n; J++)
	{
		/*Request bits_j compressed bits.  Interpret the returned bits as an
		integer k from 0 to 2^bits_j - 1, as explained in Section 8.2.*/
		bits_j = this->opget_multitype_param();
		lower_bound_j = this->opget_multitype_param();
		upper_bound_j = this->opget_multitype_param();
		uncompressed_j = this->opget_multitype_param();
		bits_total += bits_j;

		/*Decompression failure occurs if (bits_1 + ... + bits_n) > 16.*/
		if(bits_total > 16)
		{
			ok = false;
			// FIXME: DECOMPRESSION failure TOO_MANY_BITS_REQUESTED
			goto end;
		}

		if(criterion_ok) continue;

//step_4:
		/*4.If data is requested that lies beyond the end of the SigComp
		message, terminate the INPUT-HUFFMAN instruction and move program
		execution to the memory address specified by the address operand.*/
		if( (bits_j) > this->sigCompMessage->remaining_sigcomp_buffer.getRemainingBits() )
		{
			ok &= this->EXEC_INST__JUMP(address);
			goto end;
		}

//step_2:
		/*2. Request bits_j compressed bits.  Interpret the returned bits as an
		integer k from 0 to 2^bits_j - 1, as explained in Section 8.2.*/
		if(P_BIT==P_BIT_MSB_TO_LSB){
			k = this->sigCompMessage->remaining_sigcomp_buffer.readMsbToLsb(bits_j);
			if(H_BIT==H_BIT_LSB_TO_MSB) k = (BINARY_REVERSE_2BYTE(k)>>(16-bits_j));
		}
		else{
			k = this->sigCompMessage->remaining_sigcomp_buffer.readLsbToMsb(bits_j);
			if(H_BIT==H_BIT_LSB_TO_MSB) k = (BINARY_REVERSE_2BYTE(k)>>(16-bits_j));
		}
//step_3:
		// 3. Set H := H * 2^bits_j + k.
		H = H * pow(2.0, bits_j) + k;

//step_5:
		/*5. If (H < lower_bound_j) or (H > upper_bound_j) then set j := j + 1.
		Then go back to Step 2, unless j > n in which case decompression
		failure occurs.*/
		if( (H < lower_bound_j) || (H > upper_bound_j) )
		{
			continue;
			//goto step_2;
		}
		else
		{
			/*Copy (H + uncompressed_j - lower_bound_j) modulo 2^16 to the
			memory address specified by the destination operand.*/
			H = (H + uncompressed_j - lower_bound_j) % 65536;
			criterion_ok = true;
		}
	}

	if(!criterion_ok)
	{
		// FIXME: DECOMP FAILURE HUFFMAN_NO_MATCH
		ok = false;
		goto end;
	}
	else if(ok)
	{
		UDVM_SET_2BYTES_VAL(destination, H);
		this->maximum_UDVM_cycles += (bits_total * this->stateHandler->getSigCompParameters()->getCpbValue());
	}
end:
	return ok; 
}

/**
STATE-ACCESS(%partial_identifier_start, %partial_identifier_length, %state_begin, %state_length, %state_address, %state_instruction)
Reference:  RFC3320 Section 9.4.5

This instruction retrieves some previously stored state information.

@param partial_identifier_start specifies the location of the partial state identifier used to retrieve the state information
@param partial_identifier_length specifies the length of the partial state identifier used to retrieve the state information
@param state_begin defines the starting byte to copy from the state_value contained in the returned item of state
@param state_length defines the number of bytes to copy from the state_value contained in the returned item of state
@param state_address contains a UDVM memory address
@param state_instruction next instruction to jump to

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__STATE_ACCESS(uint16_t partial_identifier_start, uint16_t partial_identifier_length, uint16_t state_begin, uint16_t state_length, uint16_t state_address, uint16_t state_instruction)
{ 
	/*Decompression failure occurs if partial_identifier_length does not
	lie between 6 and 20 inclusive.*/
	if(partial_identifier_length<6 || partial_identifier_length>20)
	{
		this->createNackInfo(INVALID_STATE_ID_LENGTH);
		return false;
	}
	/*decompression failure will always occur if the state_length operand
	is set to 0 but the state_begin operand is non-zero.*/
	if(!state_length && state_begin){
		this->createNackInfo(INVALID_STATE_PROBE);
		return false;
	}

	//
	// Find matching state
	//
	SigCompState* lpState = NULL;
	SigCompBuffer partial_id;
	partial_id.referenceBuff( this->memory.getBuffer(partial_identifier_start), partial_identifier_length );
	uint16_t match_count = const_cast<SigCompStateHandler*>(this->stateHandler)->findState(&partial_id, &lpState);
	/*Decompression failure occurs if no state item matching the partial state identifier can be found, if
	more than one state item matches the partial identifier*/
	if(!lpState || !match_count || match_count>1)
	{
		this->createNackInfo((match_count>1)?ID_NOT_UNIQUE:STATE_NOT_FOUND, &partial_id);
		return false;
	}
	else if(partial_identifier_length <lpState->getMinimumAccessLength())
	{
		/*Decompression failure occurs if partial_identifier_length is less than the minimum_access_length of
		the matched state item*/
		this->createNackInfo(STATE_NOT_FOUND, &partial_id);
		return false;
	}

	/*If any of the operands state_address, state_instruction or
	state_length is set to 0 then its value is taken from the returned
	item of state instead.*/
	if(!state_address) state_address = lpState->getStateAddress();
	if(!state_instruction) state_instruction = lpState->getStateInstruction();
	if(!state_length) state_length = lpState->getStateLength();

	CONSUME_CYCLES(1+state_length);

	/*Decompression failure occurs if bytes are copied from beyond the end of the state_value.*/
	if((size_t)(state_begin+state_length) > lpState->getStateValue()->getSize()){
		this->createNackInfo(STATE_TOO_SHORT, &partial_id);
		return false;
	}

	/*The state_address operand contains a UDVM memory address.  The
	requested portion of the state_value is byte copied to this memory
	address using the rules of Section 8.4.*/
	this->bytecopy_to(state_address, lpState->getStateValue()->getBuffer(state_begin), state_length);

	/*Program execution then resumes at the memory address specified by
	state_instruction, unless this address is 0 in which case program
	execution resumes at the next instruction following the STATE-ACCESS
	instruction.*/
	if(state_instruction){
		if(!this->EXEC_INST__JUMP(state_instruction)){
			return false;
		}
	}

	return true; 
}

/**
STATE-CREATE (%state_length, %state_address, %state_instruction, %minimum_access_length, %state_retention_priority)
Reference:  RFC3320 Section 9.4.6

This instruction requests the creation of a state item at the receiving endpoint.

@param state_length defines the length of the state to create
@param state_address defines the udvm address of the state to create
@param state_instruction defines the state instruction
@param minimum_access_length defines the minimun access length
@param state_retention_priority defines the state retenion priority

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__STATE_CREATE(uint16_t state_length, uint16_t state_address, uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)
{ 
	CONSUME_CYCLES(1+state_length);

	//
	//	Create temporary state?
	//
	if(!this->createTempState(state_length, state_address, state_instruction, minimum_access_length, state_retention_priority, false))
	{
		return false;
	}

	return true;
}

/**
STATE-FREE(%partial_identifier_start, %partial_identifier_length)
Reference:  RFC3320 Section 9.4.7

This instruction informs the receiving endpoint that the sender no longer wishes to use a particular state item.

@param partial_identifier_start defines the first byte address of partial start identifier
@param partial_identifier_length defines the partial identifier length

@returns true if succeed, otherwise return false 
*/
bool SigCompUDVM::EXEC_INST__STATE_FREE(uint16_t partial_identifier_start, uint16_t partial_identifier_length)
{ 
	CONSUME_CYCLES(1);

	/*Decompression failure MUST occur if more than four state free
	requests are made before the END-MESSAGE instruction is encountered.*/
	if(this->lpResult->getTempStatesToFreeSize() >=4)
	{
		this->createNackInfo(TOO_MANY_STATE_REQUESTS);
		return false;
	}

	/*Decompression failure also occurs if partial_identifier_length does
	not lie between 6 and 20 inclusive.*/
	if(partial_identifier_length<6 || partial_identifier_length>20)
	{
		this->createNackInfo(INVALID_STATE_ID_LENGTH);
		return false;
	}

	lptempStateToFreeDesc lpDesc = new tempStateToFreeDesc();
	lpDesc->partial_identifier_length = partial_identifier_length;
	lpDesc->partial_identifier_start = partial_identifier_start;
	this->lpResult->addTempStateToFree(lpDesc);
	// Do not ByteCopy data, wait for END_MESSAGE --> see RFC 3320 9.4.9

	return true; 
}

/**
OUTPUT (%output_start, %output_length)
Reference:  RFC3320 Section 9.4.8

This instruction provides successfully decompressed data to the dispatcher.

@param output_start defines the starting memory address of the byte string to be provided to the dispatcher
@param output_length defines the length of the byte string to be provided to the dispatcher

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__OUTPUT(uint16_t output_start, uint16_t output_length)
{ 
	CONSUME_CYCLES(1+output_length);

	size_t& outputbuffer_size = this->lpResult->getOutputBuffer()->getIndexBytes();
	if( (outputbuffer_size + output_length) > 65536 )
	{
		/*Decompression failure occurs if the cumulative number of bytes
		provided to the dispatcher exceeds 65536 bytes.*/
		this->createNackInfo(OUTPUT_OVERFLOW);
		return false;
	}

	// FIXME: do it once?
	bool ok = this->bytecopy_from(this->lpResult->getOutputBuffer()->getBuffer(outputbuffer_size), output_start,  output_length );

	outputbuffer_size += output_length;

#if DEBUG || _DEBUG
	//this->lpResult->getOutputBuffer()->print(outputbuffer_size);
#endif
	return ok;
}

/**
END-MESSAGE (%requested_feedback_location, %returned_parameters_location, %state_length, %state_address, %state_instruction, %minimum_access_length, %state_retention_priority)
Reference:  RFC3320 Section 9.4.9

This instruction successfully terminates the UDVM and forwards the state creation and state free requests to the state
handler together with any supplied feedback data.

@param requested_feedback_location defines the requested feedback location
@param returned_parameters_location defines the returned parameters location which contains remote party capabilities
@param state_length length of the state to create
@param state_address udvm memory address of the state to create
@param state_instruction defines the state instruction
@param minimum_access_length defines the state's minimum access length
@param state_retention_priority determines the order in which state will be deleted when the compartment exceeds its allocated state memory

@returns true if succeed, otherwise return false
*/
bool SigCompUDVM::EXEC_INST__END_MESSAGE(uint16_t requested_feedback_location, uint16_t returned_parameters_location, uint16_t state_length, uint16_t state_address,
										 uint16_t state_instruction, uint16_t minimum_access_length, uint16_t state_retention_priority)
{
	CONSUME_CYCLES(1+state_length);

	//
	//	Create temporary state provided by END_MESSAGE?
	//
	if(!this->createTempState(state_length, state_address, state_instruction, minimum_access_length, state_retention_priority, true))
	{
		return false;
	}

	//
	// Byte copy all waiting STATE-FREE/STATE-CREATE/END-MESSAGE states
	//
	if(!this->byteCopy_TempStates())
	{
		this->createNackInfo(INTERNAL_ERROR);
		return false;
	}

	//
	//	Feedback has been requested?
	//
	if(requested_feedback_location)
	{
		if(requested_feedback_location>=this->memory.getSize()){
			this->createNackInfo(SEGFAULT);
			return false;
		}
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
		uint8_t r_f_l = *this->memory.getBuffer(requested_feedback_location);
		this->lpResult->getReqFeedBack()->setI((r_f_l & 0x01));
		this->lpResult->getReqFeedBack()->setS((r_f_l & 0x02)?1:0);
		this->lpResult->getReqFeedBack()->setQ((r_f_l & 0x04)?1:0);

		requested_feedback_location++; //skip 1-byte header
		if(this->lpResult->getReqFeedBack()->getQ())
		{
			// we have a requested feedback item
			uint8_t r_f_i = *this->memory.getBuffer(requested_feedback_location);
			uint8_t length = 1; //[1-128]
			if(r_f_i & 0x80){
				// case 2
				length += (r_f_i & 0x7f); // seven last bits
			}

			if(requested_feedback_location>=this->memory.getSize()){
				this->createNackInfo(SEGFAULT);
				return false;
			}
			// copy from udvm
			// FIXME: use realloc
			this->lpResult->getReqFeedBack()->getReqFeedItem()->freeBuff();
			this->lpResult->getReqFeedBack()->getReqFeedItem()->allocBuff(length);
			this->bytecopy_from(this->lpResult->getReqFeedBack()->getReqFeedItem()->getBuffer(),
				requested_feedback_location, length);
			//this->lpResult->getReqFeedBack()->getReqFeedItem()->print();
		}
	}

	//
	//	SigComp parameters have been returned?
	//
	if(returned_parameters_location)
	{
		/*
		0   1   2   3   4   5   6   7
		+---+---+---+---+---+---+---+---+
		|  cpb  |    dms    |    sms    |  returned_parameters_location
		+---+---+---+---+---+---+---+---+
		|        SigComp_version        |
		+---+---+---+---+---+---+---+---+
		| length_of_partial_state_ID_1  |
		+---+---+---+---+---+---+---+---+
		|                               |
		:  partial_state_identifier_1   :
		|                               |
		+---+---+---+---+---+---+---+---+
		:               :
		+---+---+---+---+---+---+---+---+
		| length_of_partial_state_ID_n  |
		+---+---+---+---+---+---+---+---+
		|                               |
		:  partial_state_identifier_n   :
		|                               |
		+---+---+---+---+---+---+---+---+
		*/

		if(returned_parameters_location>=this->memory.getSize()){
			this->createNackInfo(SEGFAULT);
			return false;
		}
		//** cpb+dms+sms
		uint8_t r_p_l = *this->memory.getBuffer(returned_parameters_location);
		returned_parameters_location++;
		if(r_p_l){
			this->lpResult->getRemoteParams()->setCpbCode(((r_p_l & 0xc0)>>6));
			this->lpResult->getRemoteParams()->setDmsCode(((r_p_l & 0x38)>>3));
			this->lpResult->getRemoteParams()->setSmsCode((r_p_l & 0x07));
		}
		//** sigcomp version
		uint8_t SigComp_version = *this->memory.getBuffer(returned_parameters_location);
		returned_parameters_location++;
		if(SigComp_version){
			this->lpResult->getRemoteParams()->setSigCompVersion(SigComp_version);
		}
		//** state items
		for(uint16_t index = returned_parameters_location; index <(this->memory.getSize()-1); )
		{
			uint8_t length = *this->memory.getBuffer(index); // 1-byte
			if(length<6 || length>20) break;
			index++;
			if((index+length)>=this->memory.getSize()){
				this->createNackInfo(SEGFAULT);
				return false;
			}
			SigCompBuffer* partial_id = new SigCompBuffer();
			partial_id->allocBuff(length);
			this->bytecopy_from(partial_id->getBuffer(), index, length);
			this->lpResult->getRemoteParams()->getReturnedStates()->push_back(partial_id);
			index+=length;
		}
	}

	return true; 
}

__NS_DECLARATION_END__
