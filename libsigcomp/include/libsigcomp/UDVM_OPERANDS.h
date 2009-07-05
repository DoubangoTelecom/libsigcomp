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

#if !defined(LIBSIGCOMP_UDVM_OPERANDS_H)
#define LIBSIGCOMP_UDVM_OPERANDS_H

// RFC 3320-Section_8.5.  Instruction operands and UDVM bytecode

#define UDVM_OPERAND__LITERAL		0 /*#*/
#define UDVM_OPERAND__REFERENCE     1 /*$*/
#define UDVM_OPERAND__MULTITYPE     2 /*%*/
#define UDVM_OPERAND__ADDRESS		3 /*@*/

#endif // LIBSIGCOMP_UDVM_OPERANDS_H