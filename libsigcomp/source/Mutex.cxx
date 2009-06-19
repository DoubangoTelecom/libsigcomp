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
#include <libsigcomp/Mutex.h>

#ifdef USE_WIN32_THREADS
#elif defined(USE_PTHREAD_TRHREADS)
#	include <pthread.h>
#else
#error No mutex type defined
#endif

Mutex::Mutex()
{
#if defined(USE_WIN32_THREADS)
	this->lphandle = new HANDLE();
	*((HANDLE*)this->lphandle) = CreateMutex(NULL, FALSE, NULL);
	assert(this->lphandle);
#elif defined(USE_PTHREAD_TRHREADS)
	this->lphandle = new pthread_mutex_t;
	pthread_mutexattr_t *lpattributes = NULL;
	pthread_mutex_init( (pthread_mutex_t*)this->lphandle, lpattributes );

else
#error Not implemented
#endif
}

Mutex::~Mutex()
{
#if defined(USE_WIN32_THREADS)
	// do nothing
#elif defined(USE_PTHREAD_TRHREADS)
	pthread_mutex_destroy((pthread_mutex_t*)this->lphandle);
#else
	#error Not implemented
#endif

	SAFE_DELETE_PTR(this->lphandle);
}

/**
Lock section
*/
void Mutex::lock()
{
#if defined(USE_WIN32_THREADS)
	assert(WaitForSingleObject(*((HANDLE*)this->lphandle), INFINITE) != WAIT_FAILED);
#elif defined(USE_PTHREAD_TRHREADS)
	assert(!pthread_mutex_lock((pthread_mutex_t*)this->lphandle));
#else
#error Not implemented
#endif
}

/**
Unlock section
*/
void Mutex::unlock()
{
#if defined(USE_WIN32_THREADS)
	assert(ReleaseMutex(*((HANDLE*)this->lphandle)));
#elif defined(USE_PTHREAD_TRHREADS)
	assert(!pthread_mutex_unlock((pthread_mutex_t*)handle_ptr));
#else
#error Not implemented
#endif
}