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

#if !defined(LIBSIGCOMP_STRUCTDECOMPRESULT_H)
#define LIBSIGCOMP_STRUCTDECOMPRESULT_H

#include "libsigcomp_config.h"

#include "MYTYPES.h"

#include "SigCompState.h"
#include "struct_req_feedback.h"
#include "struct_sigcomp_parameters.h"

#include <list>

__NS_DECLARATION_BEGIN__

//
//	tempStateToFreeDesc
//
typedef struct struct_tempStateToFreeDesc
{
	// Operands
	uint16_t partial_identifier_start;
	uint16_t partial_identifier_length;
	// identifier
	SigCompBuffer identifier;

	struct_tempStateToFreeDesc(){
		//identifier = NULL;
	}
	~struct_tempStateToFreeDesc(){
		//SAFE_DELETE_PTR(identifier);
	}
}
tempStateToFreeDesc, *lptempStateToFreeDesc;

//
//	decompResult
//
typedef struct struct_decompResult
{
public:

		struct_decompResult()
		{
			compartmentId = 0;
			statesToCreateIndex = 0;
			statesToFreeIndex = 0;
			isStreamBased=false;
			streamId=0;
			isNack=false;
			for(uint8_t i=0;i<4;i++){
				statesToCreate[i]=NULL;
				statesToFree[i]=NULL;
			}
		}

		~struct_decompResult()
		{
			reset(true);
		}

		void reset(bool destructor=false, bool resetOutput=true){
			for(uint8_t i=0; i<statesToCreateIndex; i++){
				SAFE_DELETE_PTR(statesToCreate[i]);
			}
			for(uint8_t i=0; i<statesToFreeIndex; i++){
				SAFE_DELETE_PTR(statesToFree[i]);
			}
			if(!destructor)
			{
				statesToCreateIndex = 0;
				statesToFreeIndex = 0;

				remote_parameters.reset();
				if(resetOutput)output_buffer.reset();

				req_feedback.reset();
				ret_feedback.freeBuff();

				isNack=false;
				nack_info.freeBuff();
			}
		}
		
		/**
			setMessageBuffer

			@param output_ptr
			@param output_size

			@returns
		*/
		inline void setOutputBuffer(LPVOID output_ptr, size_t output_size, bool stream, uint64_t streamId=0)
		{
			this->output_buffer.referenceBuff((uint8_t*)output_ptr, output_size);
			this->isStreamBased = stream;
			this->streamId = streamId;
		}

		inline SigCompBuffer* getOutputBuffer()
		{
			return &this->output_buffer;
		}

		inline void setCompartmentId(uint64_t id){
			this->compartmentId = id;
		}

		inline uint64_t getCompartmentId(){
			return this->compartmentId;
		}

		inline bool getIsStreamBased(){
			return this->isStreamBased;
		}
		inline uint64_t getStreamId(){
			return this->streamId;
		}

		//
		// STATE-CREATE
		//
		inline void addTempStateToCreate(SigCompState* lpState)
		{
			/*Note that there is a maximum limit of four state creation requests
			per instance of the UDVM.*/
			if(this->statesToCreateIndex >= 4) return;
						
			// Add state
			this->statesToCreate[this->statesToCreateIndex++] = lpState;
		}
		inline SigCompState* *getTempStatesToCreate(){
			return this->statesToCreate;
		}
		inline uint8_t getTempStatesToCreateSize(){
			return this->statesToCreateIndex;
		}

		//
		// STATE-FREE
		//
		inline void addTempStateToFree(lptempStateToFreeDesc lpDesc)
		{
			/*Decompression failure MUST occur if more than four state free
				requests are made before the END-MESSAGE instruction is encountered.*/
			if(this->statesToFreeIndex >= 4) return;
			this->statesToFree[this->statesToFreeIndex++] = lpDesc;
		}
		inline lptempStateToFreeDesc *getTempStatesToFree(){
			return this->statesToFree;
		}
		inline uint8_t getTempStatesToFreeSize(){
			return this->statesToFreeIndex;
		}
		
		//
		//	Requested feedback
		//
		inline lpstruct_req_feedback getReqFeedBack(){
			return &this->req_feedback;
		}

		//
		//	Returned parameters
		//
		inline lpstruct_sigcomp_parameters getRemoteParams(){
			return &this->remote_parameters;
		}

		//
		//	Returned feedback
		//
		inline SigCompBuffer* getRetFeedback(){
			return &this->ret_feedback;
		}

		//
		//	Nack
		//
		inline void setIsNack(bool isnack)
		{
			this->isNack = isnack;
		}
		inline bool getIsNack()
		{
			return this->isNack;
		}
		inline SigCompBuffer* getNackInfo()
		{
			return &this->nack_info;
		}

private:
	uint64_t compartmentId;
	SigCompState* statesToCreate[4];
	uint8_t statesToCreateIndex;

	lptempStateToFreeDesc statesToFree[4];
	uint8_t statesToFreeIndex;

	uint64_t streamId;
	bool isStreamBased;
	SigCompBuffer output_buffer;

	struct_req_feedback req_feedback;
	struct_sigcomp_parameters remote_parameters;
	SigCompBuffer ret_feedback;

	bool isNack;
	SigCompBuffer nack_info;
}
DecompressionResult, *lpDecompressionResult;

__NS_DECLARATION_END__

#endif
