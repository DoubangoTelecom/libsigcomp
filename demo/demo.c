/**
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
*/

/* Copyright (C) 2008 Inexbee
*
* Authors:	Mamadou Diop <mamadou.diop@inexbee.com>
*			Laurent Etiemble <laurent.etiemble@inexbee.com>
*/

#include <c_wrapper.h>

#define COMPARTMENT_ID1		1983	// My first compartment id
#define COMPARTMENT_ID2		1984	// My second compartment id

#define IS_STREAM			0	// Using reliable transport
#define STREAM_ID			678		// stream identifier

#define MAX_BUFFER_SIZE		0xfff0

#define LOOP_COUNT			100

#define DECOMP_NACK_4_TEST	0

#define PRINTLN printf("\r\n");

// messages to use for tests
const char* messages[] =
{
	//{ 
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
	//}
	,
	//{ 
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

	//}
		,
	//{ 
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
	//}
		,
	//{ 
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
	//}
		,
	//{ 
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
	//}
		,
	//{ 
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
	//}
		,
	//{ 
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
	//}
	,
	//{
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
	//}
};

int main(int argc, char **a_argv)
{
	int i = 0;
	size_t outLen = 0;

	SigCompManager *manager1 = 0, *manager2 = 0;
	DecompressionResult *result1 = 0, *result2 = 0;

	// temporary buffers  --> will hold compression/decompression results
	char buff1[MAX_BUFFER_SIZE]; 
	char buff2[MAX_BUFFER_SIZE];
	
	// Managers
	if(!SigCompManager_new(&manager1)){ return -1; }
	if(!SigCompManager_new(&manager2)){ return -1; }

	// Add SIP/Presence dictionnaries
	SigCompManager_addSipSdpDictionary(manager1);
	SigCompManager_addPresenceDictionary(manager1);
	SigCompManager_addSipSdpDictionary(manager2);
	SigCompManager_addPresenceDictionary(manager2);
	
	// Results --> it is recomanded to use one result struct for each manager
	if(!DecompressionResult_new(&result1)){ return -1; }
	if(!DecompressionResult_new(&result2)){ return -1; }

	DecompressionResult_setCompartmentId(result1, COMPARTMENT_ID1);
	DecompressionResult_setCompartmentId(result2, COMPARTMENT_ID2);

	// Set DMS and SMS
	SigCompManager_setDecompression_Memory_Size(manager1, 8192);
	SigCompManager_setDecompression_Memory_Size(manager2, 8192);
	SigCompManager_setCycles_Per_Bit(manager1, 64);
	SigCompManager_setCycles_Per_Bit(manager2, 64);
	SigCompManager_setState_Memory_Size(manager1, 8192);
	SigCompManager_setState_Memory_Size(manager2, 8192);

	for(i = 0; i< (8*LOOP_COUNT); i++)
	{
		memset(buff1, '\0', MAX_BUFFER_SIZE);
		memset(buff2, '\0', MAX_BUFFER_SIZE);

		// 
		//	Compression using manager I
		//
		outLen = SigCompManager_compress(manager1, COMPARTMENT_ID1, messages[i%8], strlen(messages[i%8]), buff1, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			//* TODO: sendto(SendSocket, buff1, outLen, 0, (SOCKADDR *) &SenderAddr, sizeof(SenderAddr));

			//
			// Decompress the compressed message using manager II
			//
			DecompressionResult_setOutputBuffer(result2, buff2, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = SigCompManager_decompress(manager2, buff1, outLen, result2);
			if(outLen) // OK
			{
				// buff2 contains the result and outLen is result length
				printf(buff2);
				PRINTLN;

				// provide the compartment id --> save temp states
				SigCompManager_provideCompartmentId(manager2, result2);
			}
			else // NOK
			{
				printf("ERROR (1)");
				PRINTLN;
				//--assert(DecompressionResult_getIsNack(result2));
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
			//std::cout<< "ERROR (2)" << std::endl;
			//--assert(0); // MUST never happen	
		}


		// 
		//	Compression using manager II
		//
		outLen = SigCompManager_compress(manager2, COMPARTMENT_ID2, messages[i%8], strlen(messages[i%8]), buff2, MAX_BUFFER_SIZE, IS_STREAM);
		if(outLen)
		{
			DecompressionResult_setOutputBuffer(result1, buff1, MAX_BUFFER_SIZE, IS_STREAM, STREAM_ID); // set the output buffer where to copy decompressed message
			outLen = SigCompManager_decompress(manager1, buff2, outLen, result1);
			if(outLen)
			{
				printf(buff1);
				PRINTLN;
				SigCompManager_provideCompartmentId(manager1, result1);
			}
			else
			{
				printf("ERROR (3)");
				//--assert(DecompressionResult_getIsNack(result2));
#if DECOMP_NACK_4_TEST
				manager2->decompress(result1.getNackInfo()->getBuffer(), result1.getNackInfo()->getSize(), &result2);
#endif
			}
		}
		else
		{
			printf("ERROR (4)");
			//--assert(0);	
		}
	}

	// Close compartments
	SigCompManager_closeCompartment(manager1, COMPARTMENT_ID1);
	SigCompManager_closeCompartment(manager2, COMPARTMENT_ID2);

	// Delete Results
	DecompressionResult_delete(&result1);
	DecompressionResult_delete(&result2);

	// Delete managers
	SigCompManager_delete(&manager1);
	SigCompManager_delete(&manager2);

}