/*
============================================================================
Name		: demoS60.cpp
Author	  : Mamadou Diop
Copyright   : Copyright (C) 2009 Mamadou Diop <diopmamadou@yahoo.fr>

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
Description : Exe source file
============================================================================
*/

//  Include Files  

#include "demoS60.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console

#include <stdio.h>
#include <iostream>

// libSigComp Manager header (the only one needed to have access to all features)
#include <libsigcomp/SigCompManager.h>
// cross compilation (portable) types
#include <libsigcomp/MYTYPES.h>

#define COMPARTMENT_ID1		1983	// My first compartment id
#define COMPARTMENT_ID2		1984	// My second compartment id

#define IS_STREAM				false	// Using reliable transport
#define STREAM_ID			678		// stream identifier

#define MAX_BUFFER_SIZE		0xfff0

#define LOOP_COUNT			100

#define DECOMP_NACK_4_TEST	1

// messages to use for tests
const char* messages[] =
{
	{ 
		"REGISTER sip:v6.ims.test SIP/2.0\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841406\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=29358\r\n"
			"To: <sip:mamadou@v6.ims.test>\r\n"
			"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
			"CSeq: 201 REGISTER\r\n"
			"Max-Forwards: 70\r\n"
			"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
			"Contact: <sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
			"User-Agent: IM-client/OMA1.0 Mercuro-Gold/v4.0.1390.0\r\n"
			"Require: pref\r\n"
			"P-Preferred-Identity: <sip:mamadou@v6.ims.test>\r\n"
			"Supported: path\r\n"
			"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
			"Privacy: none\r\n"
			"Supported: timer\r\n"
			"Content-Length: 0\r\n"
			"\r\n"
	}
	,
	{ 
		"SIP/2.0 401 Unauthorized - Challenging the UE\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841406\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=29358\r\n"
			"To: <sip:mamadou@v6.ims.test>;tag=3241f316c9eb68efd2c34668c4fbf834-eaa0\r\n"
			"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
			"CSeq: 201 REGISTER\r\n"
			"Path: <sip:term@pcscf.v6.ims.test:4060;lr>\r\n"
			"Service-Route: <sip:orig@scscf.v6.ims.test:6060;lr>"
			"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
			"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
			"Content-Length: 0\r\n"
			"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.v6.ims.test:6060 out_uri=sip:scscf.v6.ims.test:6060 via_cnt==3\"\r\n"
			"WWW-Authenticate: Digest realm=\"v6.ims.test\", nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\", algorithm=AKAv1-MD5, qop=\"auth,auth-int\"\r\n"
			"\r\n"

	}
	,
	{ 
		"REGISTER sip:v6.ims.test SIP/2.0\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841407\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=29358\r\n"
			"To: <sip:mamadou@v6.ims.test>\r\n"
			"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
			"CSeq: 202 REGISTER\r\n"
			"Max-Forwards: 70\r\n"
			"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
			"Contact: <sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;transport=udp>;expires=600000;+deviceID=\"3ca50bcb-7a67-44f1-afd0-994a55f930f4\";mobility=\"fixed\";+g.3gpp.cs-voice;+g.3gpp.app%5fref=\"urn%3Aurnxxx%3A3gpp-application.ims.iari.gsmais\";+g.oma.sip-im.large-message;+g.oma.sip-im\r\n"
			"User-Agent: IM-client/OMA1.0 Mercuro-Gold/v4.0.1390.0\r\n"
			"Authorization: Digest algorithm=AKAv1-MD5,username=\"mamadou@v6.ims.test\",realm=\"v6.ims.test\",nonce=\"xFBhTyFaQ0/lBgboH2ZqDe3BDmFXDwAA2Peq/bxtLQs=\",uri=\"sip:v6.ims.test\",response=\"c499a6b49693d5b29c431786cff32ca4\",qop=auth-int,cnonce=\"9fcc19edace2d1beaa6122b86dd11256\",nc=00000001\r\n"
			"Require: pref\r\n"
			"P-Preferred-Identity: <sip:mamadou@v6.ims.test>\r\n"
			"Supported: path\r\n"
			"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
			"Privacy: none\r\n"
			"Supported: timer\r\n"
			"Content-Length: 0\r\n"
			"\r\n"
	}
	,
	{ 
		"SIP/2.0 200 OK - SAR succesful and registrar saved\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841407\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=29358\r\n"
			"To: <sip:mamadou@v6.ims.test>;tag=3241f316c9eb68efd2c34668c4fbf834-1b36\r\n"
			"Call-ID: M-fa53180346f7f55ceb8d8670f9223dbb\r\n"
			"CSeq: 202 REGISTER\r\n"
			"P-Associated-URI: <sip:mamadou@v6.ims.test>\r\n"
			"Contact: <sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;transport=udp>;expires=600000\r\n"
			"Path: <sip:term@pcscf.v6.ims.test:4060;lr>\r\n"
			"Service-Route: <sip:orig@scscf.v6.ims.test:6060;lr>"
			"Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, SUBSCRIBE, NOTIFY, PUBLISH, MESSAGE, INFO\r\n"
			"P-Charging-Function-Addresses: ccf=pri_ccf_address\r\n"
			"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
			"Content-Length: 0\r\n"
			"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24452 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=5060 in_uri=sip:scscf.v6.ims.test:6060 out_uri=sip:scscf.v6.ims.test:6060 via_cnt==3\"\r\n"
			"\r\n"
	}
	,
	{ 
		"SUBSCRIBE sip:mamadou@v6.ims.test SIP/2.0\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;rport;branch=z9hG4bK1245420841408\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=5705\r\n"
			"To: <sip:mamadou@v6.ims.test>\r\n"
			"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
			"CSeq: 301 SUBSCRIBE\r\n"
			"Max-Forwards: 70\r\n"
			"Allow: INVITE, ACK, CANCEL, BYE, MESSAGE, OPTIONS, NOTIFY, PRACK, UPDATE, REFER\r\n"
			"Contact: <sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;transport=udp>;+g.oma.sip-im\r\n"
			"User-Agent: IM-client/OMA1.0 Mercuro-Gold/v4.0.1390.0\r\n"
			"Expires: 600000\r\n"
			"Event: reg\r\n"
			"Accept: application/reginfo+xml\r\n"
			"Route: <sip:[2001:5c0:1502:1800::226]:4060;lr=true;transport=udp>,<sip:orig@scscf.v6.ims.test:6060;lr>\r\n"
			"P-Access-Network-Info: 3GPP-UTRAN-TDD;utran-cell-id-3gpp=00000000\r\n"
			"Privacy: none\r\n"
			"Supported: timer\r\n"
			"Allow-Events: presence, presence.winfo, conference\r\n"
			"Content-Length: 0\r\n"
			"\r\n"
	}
	,
	{ 
		"SIP/2.0 200 Subscription to REG saved\r\n"
			"Record-Route: <sip:mo@pcscf.v6.ims.test:4060;lr>\r\n"
			"Via: SIP/2.0/UDP [2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;comp=sigcomp;received=2001:5C0:1502:1800:1D41:BF77:F1BF:BB49;rport=1988;branch=z9hG4bK1245420841408\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=5705\r\n"
			"To: <sip:mamadou@v6.ims.test>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
			"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
			"CSeq: 301 SUBSCRIBE\r\n"
			"Expires: 600000\r\n"
			"Contact: <sip:mamadou@v6.ims.test;comp=sigcomp;>\r\n"
			"Server: Sip EXpress router (2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
			"Content-Length: 0\r\n"
			"Warning: 392 2001:5C0:1502:1800:0:0:0:226:6060 \"Noisy feedback tells:  pid=24454 req_src_ip=2001:5C0:1502:1800:0:0:0:226 req_src_port=4060 in_uri=sip:mamadou@v6.ims.test out_uri=sip:mamadou@v6.ims.test via_cnt==2\"\r\n"
			"\r\n"
	}
	,
	{ 
		"NOTIFY sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;transport=udp SIP/2.0\r\n"
			"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0\r\n"
			"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
			"To: <sip:mamadou@v6.ims.test>;tag=5705\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
			"CSeq: 10 NOTIFY\r\n"
			"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
			"Content-Length: 379\r\n"
			"User-Agent: Sip EXpress router(2.1.0-dev1 OpenIMSCore (i386/linux))\r\n"
			"Contact: <sip:mamadou@v6.ims.test; comp=sigcomp;>\r\n"
			"Event: reg\r\n"
			"Max-Forwards: 16\r\n"
			"Subscription-State: active;expires=600030\r\n"
			"Content-Type: application/reginfo+xml\r\n"
			"\r\n"
			"<?xml version=\"1.0\"?>\r\n"
			"<reginfo xmlns=\"urn:ietf:params:xml:ns:reginfo\" version=\"0\" state=\"full\">\r\n"
			"<registration aor=\"sip:mamadou@v6.ims.test\" id=\"0xb5d91fcc\" state=\"active\">\r\n"
			"<contact id=\"0xb5d8fb98\" state=\"active\" event=\"registered\" expires=\"600000\">\r\n"
			"<uri>sip:mamadou@[2001:5c0:1502:1800:1d41:bf77:f1bf:bb49]:1988;transport=udp</uri>\r\n"
			"</contact>\r\n"
			"</registration>\r\n"
			"</reginfo>\r\n"
	}
	,
	{
		"SIP/2.0 200 OK\r\n"
			"Max-Forwards: 70\r\n"
			"User-Agent: IM-client/OMA1.0 Mercuro-Gold/v4.0.1390.0\r\n"
			"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:4060;comp=sigcomp;branch=z9hG4bK2b3f.38818b91.0;received=2001:5c0:1502:1800::226\r\n"
			"Via: SIP/2.0/UDP [2001:5C0:1502:1800:0:0:0:226]:6060;received=2001:5C0:1502:1800:0:0:0:226;rport=6060;branch=z9hG4bK2b3f.6db77cf1.0\r\n"
			"To: <sip:mamadou@v6.ims.test>;tag=5705\r\n"
			"From: <sip:mamadou@v6.ims.test>;tag=3241f316c9eb68efd2c34668c4fbf834-5cce\r\n"
			"CSeq: 10 NOTIFY\r\n"
			"Call-ID: M-dd6e227ce416f853ca7bca49ad5b676d\r\n"
			"Content-Length: 0\r\n"
			"\r\n"
	}
};

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


//  Local Functions

LOCAL_C void MainL()
{

	console->Write(_L("libSigComp test for Symbian S60\n"));

	// temporary buffers  --> will hold compression/decompression results
	char buff1[MAX_BUFFER_SIZE]; 	memset(buff1, NULL, MAX_BUFFER_SIZE);
	char buff2[MAX_BUFFER_SIZE]; 	memset(buff2, NULL, MAX_BUFFER_SIZE);

	// Managers
	sigcomp::SigCompManager* manager1 = new sigcomp::SigCompManager();
	sigcomp::SigCompManager* manager2 = new sigcomp::SigCompManager();

	// Results --> it is recomanded to use one result struct for each manager
	sigcomp::DecompressionResult result1; result1.setCompartmentId(COMPARTMENT_ID1);
	sigcomp::DecompressionResult result2; result2.setCompartmentId(COMPARTMENT_ID2);

	// Set DMS and SMS
	manager1->setDecompression_Memory_Size(8192);
	manager2->setDecompression_Memory_Size(8192);
	manager1->setState_Memory_Size(8192);
	manager2->setState_Memory_Size(4096);

	for(int i = 0; i< (8*LOOP_COUNT); i++)
	{

		// 
		//	Compression using manager I
		//
		size_t outLen = manager1->compress(COMPARTMENT_ID1, messages[i%8], strlen(messages[i%8]), buff1, MAX_BUFFER_SIZE, IS_STREAM);
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
				std::cout<< std::string(buff2,outLen).c_str()  << std::endl<<std::endl;
				// provide the compartment id --> save temp states
				manager2->provideCompartmentId(&result2);
			}
			else // NOK
			{
				assert(result2.getIsNack());
#if DECOMP_NACK_4_TEST
				manager1->decompress(result2.getNackInfo()->getBuffer(), result2.getNackInfo()->getSize(), &result1);
#endif
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
		outLen = manager2->compress(COMPARTMENT_ID2, messages[i%8], strlen(messages[i%8]), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			result1.setOutputBuffer(buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = manager1->decompress(buff2, outLen, &result1);
			if(outLen)
			{
				std::cout<< std::string(buff1,outLen).c_str() << std::endl<<std::endl;
				manager1->provideCompartmentId(&result1);
			}
			else
			{
				assert(result2.getIsNack());
#if DECOMP_NACK_4_TEST
				manager2->decompress(result1.getNackInfo()->getBuffer(), result1.getNackInfo()->getSize(), &result2);
#endif
			}
		}
		else
		{
			assert(0);	
		}
	}	
	manager1->closeCompartment(COMPARTMENT_ID1);
	manager2->closeCompartment(COMPARTMENT_ID2);

	SAFE_DELETE_PTR(manager1);
	SAFE_DELETE_PTR(manager2);
}

LOCAL_C void DoStartL()
{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	MainL();

	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
}

//  Global Functions

GLDEF_C TInt E32Main()
{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Create output console
	TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
		KConsFullScreen, KConsFullScreen)));
	if (createError)
		return createError;

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());
	if (mainError)
		console->Printf(KTextFailed, mainError);
	console->Printf(KTextPressAnyKey);
	console->Getch();

	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
}

