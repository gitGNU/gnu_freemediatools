/***************************************************************************
 *  Copyright (C) 2003 Roman Kaljakin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  $Id: Lock.h,v 1.1 2005/06/15 11:24:21 jahshaka Exp $
 *
 **************************************************************************/

#ifndef __LOCK_H
#define __LOCK_H

#include <pthread.h>
#include <errno.h>

/**
 *  This class is simple mutex wrapper.
 */
class MutexLock {
public:
	MutexLock () {
		pthread_mutex_init (&mutex, 0);		
	};

	/**
	 *  Lock mutex.
	 *  @return
	 *          - true  - mutex locked.
	 *          - false - an error occured, mutex not locked.
	 */
	bool Lock () {
		int res = pthread_mutex_lock (&mutex);
		return res == 0?true:false;
	};

	/**
	 *  Unlock mutex.
	 *  @return
	 *          - true  - mutex unlocked.
	 *          - false - an error occured, mutex not unlocked.
	 */
	bool Unlock () {
		int res = pthread_mutex_unlock (&mutex);
		return res == 0?true:false;
	}

	/**
	 *  Try to lock mutex.
	 *  @return
	 *          - true  - mutex locked.
	 *          - false - an error occured, mutex not locked.
	 */
	bool Trylock () {
		int res = pthread_mutex_trylock (&mutex);
		return res == 0?true:false;
	}

	~MutexLock () {
		int res = pthread_mutex_destroy (&mutex);

		if (res == EBUSY) {
			printf ("WARNING! Mutex _must_be_ unlocked before destroying!!!");
		}
	};
private:
 	pthread_mutex_t mutex;
};

#endif /* __LOCK_H */
