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

#if !defined(LIBSIGCOMP_LAYER_COMPARTMENT_H)
#define LIBSIGCOMP_LAYER_COMPARTMENT_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/MYTYPES.h>

#include <libsigcomp/SigCompLayer/SigCompState.h>
#include <libsigcomp/structures/struct_decompResult.h>
#include <libsigcomp/SafeObject.h>

#include <list>

//class SigCompStateHandler;

/*An application-specific grouping of messages that relate to a peer
      endpoint.  Depending on the signaling protocol, this grouping may
      relate to application concepts such as "session", "dialog",
      "connection", or "association".  The application allocates state
      memory on a per-compartment basis, and determines when a
      compartment should be created or closed.*/
class SigCompCompartment : public SafeObject
{
public:
	SigCompCompartment(t_uint64 id, t_uint16 sigCompParameters);
	~SigCompCompartment();
	
	inline t_uint64 getIdentifier();
	t_uint16 getTotalMemorySize();
	t_uint16 getTotalMemoryLeft();

	//
	//	SigComp Parameters
	//
	void setRemoteParams(lpstruct_sigcomp_parameters lpParams);
	lpstruct_sigcomp_parameters getRemoteParameters() { return &this->remote_parameters; }
	lpstruct_sigcomp_parameters getLocalParameters() { return &this->local_parameters; }

	//
	//	Feedbacks
	//
	void setReqFeedback(SigCompBuffer* feedback);
	SigCompBuffer* getReqFeedback() { return this->lpReqFeedback; }
	void setRetFeedback(SigCompBuffer* feedback);
	SigCompBuffer* getRetFeedback() { return this->lpRetFeedback; }

	void clearStates();
	void freeStateByPriority();
	void freeState(SigCompState* &lpState);
	void freeStates(lptempStateToFreeDesc *tempStates, t_uint8 size);
	void addState(SigCompState* &lpState);
	t_uint16 findState(const SigCompBuffer* partial_identifier, SigCompState** lpState);

	//
	//	Nacks
	//
	void addNack(const t_uint8 nackId[SHA1HashSize]);
	bool hasNack(const SigCompBuffer* nackId);

private:
	/*An identifier (in a locally chosen format) that uniquely
      references a compartment.*/
	t_uint64 identifier;

	std::list<SigCompState*> states;
	struct_sigcomp_parameters remote_parameters;
	struct_sigcomp_parameters local_parameters;
	t_uint16 total_memory_size;
	t_uint16 total_memory_left;

	SigCompBuffer* lpReqFeedback;
	SigCompBuffer* lpRetFeedback;

	std::list<SigCompBuffer*> nacks;
};

#endif // LIBSIGCOMP_LAYER_COMPARTMENT_H