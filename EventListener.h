#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__
#include <list>
#include <cmath>
#include "Mutex.h"
#include "Message.h"

#ifndef MESSAGEQUEUE_MAX
#define MESSAGEQUEUE_MAX 20000
#endif//MESSAGEQUEUE_MAX

#ifndef MESSAGEQUEUE_DISPATCH
#define MESSAGEQUEUE_DISPATCH 20
#endif//MESSAGEQUEUE_DISPATCH

namespace Event
{
	class EventListener : public Mutex
	{
	protected:
		size_t count;
		long long total;
		long long missing;
		list<Message> messageQueue;
	public:
		EventListener(void):count(0),total(0),missing(0)
		{
		}
		virtual void RecvMessage(const Message& m)
		{
			Lock();
			if(count < MESSAGEQUEUE_MAX)
			{
				messageQueue.push_back(m);
				total = total + 1LL;
				count++;
			}
			else
			{
				missing = missing + 1LL;
			}
			Unlock();
		}
		virtual const xstring GetInfo(void)=0;
	};
}

#endif//__EVENT_LISTENER_H__
