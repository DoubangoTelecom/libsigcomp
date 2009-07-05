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
	inline void setCpbCode(t_uint8 _cpbCode) { cpbCode=_cpbCode; cpbValue = sigcomp_encoding_cpb[_cpbCode]; }
	inline t_uint8 getCpbCode(){ return cpbCode; }
	inline void setCpbValue(t_uint8 _cpbValue){
		for(int code=0;code<4;code++){
			if( _cpbValue<=sigcomp_encoding_cpb[code]){
				cpbCode = code;
				break;
			}
		}
		cpbValue = _cpbValue;
	}
	inline t_uint8 getCpbValue(){ return  cpbValue; }

	//
	//	decompression_memory_size
	//
	inline void setDmsCode(t_uint8 _dmsCode) { dmsCode=_dmsCode; dmsValue = sigcomp_encoding_dms[_dmsCode]; }
	inline t_uint8 getDmsCode(){ return dmsCode; }
	inline void setDmsValue(t_uint32 _dmsValue){
		for(int code=1;code<8;code++){
			if( _dmsValue<=sigcomp_encoding_dms[code]){
				dmsCode = code;
				break;
			}
		}
		dmsValue = _dmsValue;
	}
	inline t_uint32 getDmsValue(){ return  dmsValue; }

	//
	//	state_memory_size
	//
	inline void setSmsCode(t_uint8 _smsCode) { smsCode=_smsCode; smsValue = sigcomp_encoding_sms[_smsCode]; }
	inline t_uint8 getSmsCode(){ return smsCode; }
	inline void setSmsValue(t_uint32 _smsValue){
		for(int code=0;code<8;code++){
			if( _smsValue<=sigcomp_encoding_sms[code]){
				smsCode = code;
				break;
			}
		}
		smsValue = _smsValue;
		assert(smsValue >= SIP_RFC5049_STATE_MEMORY_SIZE && smsValue <= 65536); // FIXME: 65536 --> no sense
	}
	inline t_uint32 getSmsValue(){ return  smsValue; }

	//
	//	SigComp_version
	//
	inline void setSigCompVersion(t_uint8 version) { SigComp_version = version; }
	inline t_uint8 getSigCompVersion(){ return SigComp_version; }

	//
	//	SigComp parameters (cpb+dms+sms+version)
	//
	inline t_uint16 getParameters()
	{
	/*+---+---+---+---+---+---+---+---+
      |  cpb  |    dms    |    sms    |
      +---+---+---+---+---+---+---+---+
      |        SigComp_version        |
      +---+---+---+---+---+---+---+---+*/
		t_uint16 result = ((cpbCode<<6)|(dmsCode<<3)|smsCode); // cpb_dms_sms
		result <<=8;
		return (result|SigComp_version);
	}
	inline void setParameters(t_uint16 sigCompParameters)
	{
		// See above
		this->setCpbCode( (sigCompParameters>>14) );
		this->setDmsCode( ((sigCompParameters>>11) & 0x07) );
		this->setSmsCode( ((sigCompParameters>>8) & 0x07) );
		this->setSigCompVersion( (sigCompParameters & 0x00ff) );
	}

	inline std::list<SigCompBuffer*>* getReturnedStates() { return &returnedStates; }
private:
	t_uint8 cpbCode;
	t_uint8 dmsCode;
	t_uint8 smsCode;

	t_uint8 cpbValue;
	t_uint32 dmsValue;
	t_uint32 smsValue;

	t_uint8 SigComp_version;
	std::list<SigCompBuffer*> returnedStates;
}
struct_sigcomp_parameters, *lpstruct_sigcomp_parameters;

__NS_DECLARATION_END__

#endif // LIBSIGCOMP_STRUCTSIGCOMP_PARAMETERS_H