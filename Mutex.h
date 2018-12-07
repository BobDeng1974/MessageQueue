#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <string.h>
#include <pthread.h>

#ifdef DEBUG
#define DEBUG_ERR(err)	if(0 != err)throw strerror(err)
#else
#define DEBUG_ERR(err)
#endif

class Mutex
{
protected:
	pthread_mutex_t mutex;
public:
	Mutex(void)
	{
		int err = pthread_mutex_init(&mutex, 0);
		DEBUG_ERR(err);
	}
	Mutex(pthread_mutexattr_t *attr)
	{
		int err = pthread_mutex_init(&mutex, attr);
		DEBUG_ERR(err);
	}
	bool Destroy(void)
	{
		int err = pthread_mutex_destroy(&mutex);
		DEBUG_ERR(err);
		return err==0;
	}
	bool Lock(void)
	{
		int err = pthread_mutex_lock(&mutex);
		DEBUG_ERR(err);
		return err==0;
	}
	bool Trylock(void)
	{
		int err = pthread_mutex_lock(&mutex);
		DEBUG_ERR(err);
		return err==0;
	}
	bool Unlock(void)
	{
		int err = pthread_mutex_unlock(&mutex);
		DEBUG_ERR(err);
		return err==0;
	}
};
class MutexAuto : public Mutex
{
	public:
		MutexAuto(void)
		{
			Lock();
		}
		~MutexAuto(void)
		{
			Unlock();
			Destroy();
		}
};

#endif//__MUTEX_H__
