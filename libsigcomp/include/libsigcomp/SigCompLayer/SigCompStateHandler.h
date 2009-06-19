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

#if !defined(LIBSIGCOMP_LAYER_STATEHANDLER_H)
#define LIBSIGCOMP_LAYER_STATEHANDLER_H

#include <libsigcomp/libsigcomp_config.h>

#include <libsigcomp/mytypes.h>
#include <libsigcomp/SigCompLayer/SigCompCompartment.h>
#include <libsigcomp/structures/struct_nack_info.h>
#include <libsigcomp/SafeObject.h>

#include <map>

class SigCompStateHandler : public SafeObject
{
public:
	SigCompStateHandler();
	~SigCompStateHandler();

	SigCompCompartment* getCompartment(t_uint64 id);
	void deleteCompartment(t_uint64 id);
	bool compartmentExist(t_uint64 id);
	t_uint16 findState(const SigCompBuffer* partial_identifier, SigCompState** lpState);

	void handleResult(lpDecompressionResult &lpResult);
	bool handleNack(const lpstruct_nack_info);

	//
	//	SigComp Parameters
	//
	lpstruct_sigcomp_parameters getSigCompParameters() { return &this->sigcomp_parameters; }
	
	//
	// Dictionnaries
	//
	void addSipSdpDictionary();
	void removeSipSdpDictionary();

	const std::map<SigCompBuffer*, SigCompState*>* getDictionaries() { return &this->dictionaries; }
	

private:
	std::map<t_uint64, SigCompCompartment*> compartments;
	struct_sigcomp_parameters sigcomp_parameters;

	std::map<SigCompBuffer*, SigCompState*> dictionaries;
	bool hasSipSdpDictionary;

};

#endif // LIBSIGCOMP_LAYER_STATEHANDLER_H