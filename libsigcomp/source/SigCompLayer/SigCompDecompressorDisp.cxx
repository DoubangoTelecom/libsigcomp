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

#include <global_config.h>
#include <libsigcomp/SigCompLayer/SigCompDecompressorDisp.h>

#include <libsigcomp/SigCompLayer/SigCompUDVM.h>

#define MAX_STREAM_BUFFER_SIZE 65535
#define NACK_SUPPORTED (const_cast<SigCompStateHandler*>(this->stateHandler)->getSigCompParameters()->getSigCompVersion() >= 0x02)

/**
	SigCompDecompressorDisp
	@param stateHandler

	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompDecompressorDisp::SigCompDecompressorDisp(const SigCompStateHandler* _stateHandler)
:SafeObject()
{
	this->stateHandler = _stateHandler;
}

/**
	~SigCompDecompressorDisp
	RFC3320: [4.  SigComp Dispatchers]
*/
SigCompDecompressorDisp::~SigCompDecompressorDisp()
{
	// Delete all stream buffer
	std::map<t_uint64, SigCompBuffer*>::iterator it;
	SAFE_CLEAR_MAP(this->streamBuffers, it);
}

/**
	decompress
	@param input_ptr
	@param input_size
	@param output_ptr
	@param output_size

	@returns
*/
bool SigCompDecompressorDisp::decompress(LPCVOID input_ptr, size_t input_size, lpDecompressionResult lpResult)
{
	bool ret = true;
	bool stream = lpResult->getIsStreamBased();
	t_uint64 streamId = 0;

	//
	// Check if transport type changed
	//
	if(stream){
		streamId = lpResult->getStreamId();
		ret = this->appendStream(input_ptr, input_size, streamId);
		if(!ret){
			// FIXME: Global Error
			assert(0);
			return false;
		}
	}
	
	if(stream)
	{
		t_uint64 size=0;
		t_uint16 discard_count=0;
		SigCompBuffer* lpBuffer = this->streamBuffers[streamId];

		while(ret && this->getNextStreamMsg(streamId, discard_count, size))
		{
			ret&= this->internalDecompress(lpBuffer->getBuffer(), size, &lpResult);
			// remove buffer and discard
			lpBuffer->discardLastBytes(discard_count);
			ret&= lpBuffer->removeBuff(0, (size));
		}
		if(discard_count){
			lpBuffer->discardLastBytes(discard_count);
		}
		if(size){
			ret&= lpBuffer->removeBuff(0, (size));
		}
	}
	else
	{
		ret &= this->internalDecompress(input_ptr, input_size, &lpResult);
	}

	return ret;
}

/**
	internalDecompress
	@param input_ptr
	@param input_size
	@param lpResult

	@returns
*/

inline bool SigCompDecompressorDisp::internalDecompress(LPCVOID input_ptr, const size_t &input_size, lpDecompressionResult *lpResult)
{
	SigCompMessage sigCompMessage(input_ptr, input_size, (*lpResult)->getIsStreamBased());
	if( !sigCompMessage.getIsOK() )
	{
		return false;
	}
	else if(sigCompMessage.getIsNack() && NACK_SUPPORTED)
	{
		// Remote party send us a NACK --> handle it
		const_cast<SigCompStateHandler*>(this->stateHandler)->handleNack( sigCompMessage.getNackInfo() );
		(*lpResult)->setIsNack(true);
		return false;
	}

	// Create new UDVM entity for each SigComp message
	SigCompUDVM* sigCompUDVM = new SigCompUDVM(&sigCompMessage, this->stateHandler, *lpResult);
	
	// Decompress message
	bool ret = sigCompUDVM->decompress();

	// decompression failed --> returns nack if supported
	if(!ret)
	{
		// Decompression failed --> return NACK message to the application layer
		(*lpResult)->setIsNack(NACK_SUPPORTED);
	}
	
	// Delete UDVM entity
	SAFE_DELETE_PTR(sigCompUDVM);

	return ret;
}

/**
	createStates

	@param input_ptr
	@param input_size
	@param streamId

	@returns
*/
bool SigCompDecompressorDisp::appendStream(LPCVOID input_ptr, size_t input_size, t_uint64 streamId)
{
	SigCompBuffer* lpBuffer = NULL;
	if(this->streamBuffers.find(streamId)==this->streamBuffers.end()){
		// First time we get this stream ID
		lpBuffer = new SigCompBuffer();
		this->streamBuffers[streamId] = lpBuffer;
	}else{
		lpBuffer = this->streamBuffers[streamId];
	}
	if(!lpBuffer) return false;
	
	//  Check if buffer is too large
	if((lpBuffer->getSize() + input_size) >MAX_STREAM_BUFFER_SIZE){
		lpBuffer->freeBuff();
		return false;
	}

	// append new buffer
	if(!lpBuffer->appendBuff(input_ptr, input_size)){
		lpBuffer->freeBuff();
		return false;
	}
	
	return true;
}

/**
	getNextStreamMsg

	@param streamId
	@param escape_count
	@param size

	@returns;
*/
bool SigCompDecompressorDisp::getNextStreamMsg(t_uint64 streamId, t_uint16 &discard_count, t_uint64 &size)
{
	//
	// RFC 3320 - 4.2.1.  Decompressor Dispatcher Strategies [strategie 1]
	//
	SigCompBuffer* lpBuffer = this->streamBuffers[streamId];
	if(!lpBuffer) return false;
	
	size = 0;
	discard_count = 0;

	t_uint8 quote_count = 0;
	t_uint8* start = lpBuffer->getBuffer();
	t_uint8* end = (start+lpBuffer->getSize());
	while(start<end){
		if(*start==0xff)
		{
			start++;
			if(*start==0xff)
			{ // end message
				if(size) return true;
				else // message is empty --> delete this empty message(length=2)
				{ 
					start--;
					memmove(start, (start+2), (end-start));
					discard_count+=2;
					end-=2;
					continue; 
				}
			}

			quote_count = *start;
			memmove((start), (start+1), (end-start));
			end--;
			discard_count++;
			start+=(quote_count);
			size+=(1+quote_count);
		}else { start++; size++; }
	}

	return false;
}

bool handleNack(const lpstruct_nack_info)
{
	return false;
}