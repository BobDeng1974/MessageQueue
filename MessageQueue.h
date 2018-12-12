#ifndef __EVENT_MESSAGEQUEUE_H__
#define __EVENT_MESSAGEQUEUE_H__
#include <list>
#include <cmath>
#include "Mutex.h"
#include "TimeValue.h"
#include "EventQueue.h"
using namespace std;

class MessageQueue : public Mutex 
{
	protected:
		size_t			count;
		long long		total;
		long long		miss;
		long long		event[EVENT_MAX];
		TimeValue		time;
		list<Message>	queue;
	public:
		MessageQueue(void);
		const size_t Count(void)const;
		const long long Total(void)const;
		const long long Missing(void)const;
		const xstring GetInfo(void);
		void Push(const Message&);
		void Dispatch(EventQueue&);
		typedef list<Message>::iterator iterator;
};

#endif//__EVENT_QUEUE_H__
