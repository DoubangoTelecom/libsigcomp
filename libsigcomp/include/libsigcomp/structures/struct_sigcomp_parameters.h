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

#if !defined(LIBSIGCOMP_STRUCTSIGCOMP_PARAMETERS_H)
#define LIBSIGCOMP_STRUCTSIGCOMP_PARAMETERS_H

#include <libsigcomp/mytypes.h>
#include <libsigcomp/rfc5049_sip.h>
#include <libsigcomp/binary_utils.h>

#include <assert.h>
#include <list>

__NS_DECLARATION_BEGIN__

/**3.3.  SigComp Parameters**/

/**
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

typedef struct struct_sigcomp_parameters
{
	struct_sigcomp_parameters()
	{
		reset();
	}

	~struct_sigcomp_parameters()
	{
		reset();
	}

	void reset(){
		cpbCode = dmsCode = smsCode = SigComp_version = 0;
		cpbValue = dmsValue = smsValue = 0;

		std::list<SigCompBuffer* >::iterator it;
		SAFE_CLEAR_LIST(this->returnedStates, it);
	}
	
	inline bool hasCpbDmsSms(){
		return (cpbCode||dmsCode||smsCode);
	}

	//
	//	cycles_per_bit
	//
	inline void setCpbCode(uint8_t _cpbCode) { cpbCode=_cpbCode; cpbValue = sigcomp_encoding_cpb[_cpbCode]; }
	inline uint8_t getCpbCode(){ return cpbCode; }
	inline void setCpbValue(uint8_t _cpbValue){
		for(int code=0;code<4;code++){
			if( _cpbValue<=sigcomp_encoding_cpb[code]){
				cpbCode = code;
				break;
			}
		}
		cpbValue = _cpbValue;
	}
	inline uint8_t getCpbValue(){ return  cpbValue; }

	//
	//	decompression_memory_size
	//
	inline void setDmsCode(uint8_t _dmsCode) { dmsCode=_dmsCode; dmsValue = sigcomp_encoding_dms[_dmsCode]; }
	inline uint8_t getDmsCode(){ return dmsCode; }
	inline void setDmsValue(uint32_t _dmsValue){
		for(int code=1;code<8;code++){
			if( _dmsValue<=sigcomp_encoding_dms[code]){
				dmsCode = code;
				break;
			}
		}
		dmsValue = _dmsValue;
	}
	inline uint32_t getDmsValue(){ return  dmsValue; }

	//
	//	state_memory_size
	//
	inline void setSmsCode(uint8_t _smsCode) { smsCode=_smsCode; smsValue = sigcomp_encoding_sms[_smsCode]; }
	inline uint8_t getSmsCode(){ return smsCode; }
	inline void setSmsValue(uint32_t _smsValue){
		for(int code=0;code<8;code++){
			if( _smsValue<=sigcomp_encoding_sms[code]){
				smsCode = code;
				break;
			}
		}
		smsValue = _smsValue;
		assert(smsValue >= SIP_RFC5049_STATE_MEMORY_SIZE && smsValue <= 65536); // FIXME: 65536 --> no sense
	}
	inline uint32_t getSmsValue(){ return  smsValue; }

	//
	//	SigComp_version
	//
	inline void setSigCompVersion(uint8_t version) { SigComp_version = version; }
	inline uint8_t getSigCompVersion(){ return SigComp_version; }

	//
	//	SigComp parameters (cpb+dms+sms+version)
	//
	inline uint16_t getParameters()
	{
	/*+---+---+---+---+---+---+---+---+
      |  cpb  |    dms    |    sms    |
      +---+---+---+---+---+---+---+---+
      |        SigComp_version        |
      +---+---+---+---+---+---+---+---+*/
		uint16_t result = ((cpbCode<<6)|(dmsCode<<3)|smsCode); // cpb_dms_sms
		result <<=8;
		return (result|SigComp_version);
	}
	inline void setParameters(uint16_t sigCompParameters)
	{
		// See above
		this->setCpbCode( (sigCompParameters>>14) );
		this->setDmsCode( ((sigCompParameters>>11) & 0x07) );
		this->setSmsCode( ((sigCompParameters>>8) & 0x07) );
		this->setSigCompVersion( (sigCompParameters & 0x00ff) );
	}

	inline std::list<SigCompBuffer*>* getReturnedStates() { return &returnedStates; }
private:
	uint8_t cpbCode;
	uint8_t dmsCode;
	uint8_t smsCode;

	uint8_t cpbValue;
	uint32_t dmsValue;
	uint32_t smsValue;

	uint8_t SigComp_version;
	std::list<SigCompBuffer*> returnedStates;
}
struct_sigcomp_parameters, *lpstruct_sigcomp_parameters;

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_STRUCTSIGCOMP_PARAMETERS_H