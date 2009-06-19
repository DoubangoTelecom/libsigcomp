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

/*
This application demonstrate how to use libSigComp to perform some basic tasks:
--> compression
--> decompression
--> state creation by providing compartment id after decompression
--> NACK handling (if decompression fails)

This application uses DEFLATE (RFC 1951) as default compress algorithm.
RFC 5049 recommands to use a state memory state value >= 2048 but if you want to get good compression ratio
you should use a sms value >= 4096.

Don't worry about multithreading --> libSigComp is thread-safe
Don't worry for tcp case --> complexity is hidden
*/

#include "stdafx.h"

// libSigComp Manager header (the only one needed to have access to all features)
#include <libsigcomp/SigCompManager.h>
// cross compilation (portable) types
#include <libsigcomp/MYTYPES.h>

#define COMPARTMENT_ID1		1983	// My first compartment id
#define COMPARTMENT_ID2		1984	// My second compartment id

#define IS_STREAM				false	// Using reliable transport
#define STREAM_ID			678		// stream identifier

#define MAX_BUFFER_SIZE		1024

#define LOOP_COUNT			100

// messages to use for tests
const char* messages[] =
{
	{ 
		"How are you?"
			"\r\n"
	},
	{ 
		"I'm fine. and you?"
			"\r\n"
		},
		{ 
			"I'm currently testing libSigComp. The Open Source SigComp Framework?"
				"\r\n"
		},
		{ 
			"What is SigComp?"
				"\r\n"
			},
			{ 
				"SigComp stands for Signaling Compression."
					"\r\n"
			},
			{ 
				"Where can I download libSigComp?"
					"\r\n"
				},
				{ 
					"http://code.google.com/p/libsigcomp/"
						"\r\n"
				}
};

int _tmain(int argc, _TCHAR* argv[])
{
	// temporary buffers  --> will hold compression/decompression results
	char buff1[MAX_BUFFER_SIZE]; 	memset(buff1, NULL, MAX_BUFFER_SIZE);
	char buff2[MAX_BUFFER_SIZE]; 	memset(buff2, NULL, MAX_BUFFER_SIZE);

	// Managers
	SigCompManager* manager1 = new SigCompManager();
	SigCompManager* manager2 = new SigCompManager();

	// Results --> it is recomanded to use one result struct for each manager
	DecompressionResult result1; result1.setCompartmentId(COMPARTMENT_ID1);
	DecompressionResult result2; result2.setCompartmentId(COMPARTMENT_ID2);

	// Set DMS and SMS
	manager1->setDecompression_Memory_Size(8192);
	manager2->setDecompression_Memory_Size(8192);
	manager1->setState_Memory_Size(4096);
	manager2->setState_Memory_Size(4096);

	for(int i = 0; i< (7*LOOP_COUNT); i++)
	{

		// 
		//	Compression using manager I
		//
		size_t outLen = manager1->compress(COMPARTMENT_ID1, messages[i%7], strlen(messages[i%7]), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			//* TODO: sendto(SendSocket, buff1, outLen, 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));

			//
			// Decompress the compressed message using manager II
			//
			result2.setOutputBuffer(buff2, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = manager2->decompress(buff1, outLen, &result2);
			if(outLen) // OK
			{
				// buff2 contains the result and outLen is result length
				//* TODO: sendto(SendSocket, buff2, outLen, 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));
				std::cout<< buff2 << std::endl<<std::endl;
				// provide the compartment id --> save temp states
				manager2->provideCompartmentId(&result2);
			}
			else // NOK
			{
				assert(result2.geIsNack());
				// Decompression failed --> handle NACK (done by remote party)
				// NACK will be retourned only if SigCompVersion >= 2
				// NACK must be sent to the remote party (SIP/IMS use-case) over the network
				//* TODO: sendto(SendSocket, result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));
			}
		}
		else
		{
			assert(0); // MUST never happen	
		}


		// 
		//	Compression using manager II
		//
		outLen = manager2->compress(COMPARTMENT_ID2, messages[i%7], strlen(messages[i%7]), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			result1.setOutputBuffer(buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = manager1->decompress(buff2, outLen, &result1);
			if(outLen)
			{
				std::cout<< buff1 << std::endl<<std::endl;
				manager1->provideCompartmentId(&result1);
			}
			else
			{
				assert(result2.geIsNack());
			}
		}
		else
		{
			assert(0);	
		}
	}

	return 0;
}

