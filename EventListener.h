#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__
#include <list>
#include <cmath>
#include "Mutex.h"
#include "Message.h"

#ifndef EVENT_MAX
#define EVENT_MAX 100
#endif//EVENT_MAX

#ifndef MESSAGEQUEUE_MAX
#define MESSAGEQUEUE_MAX 20000
#endif//MESSAGEQUEUE_MAX

#ifndef MESSAGEQUEUE_DISPATCH
#define MESSAGEQUEUE_DISPATCH 20
#endif//MESSAGEQUEUE_DISPATCH

namespace Event
{
	class EventListener : Mutex
	{
	protected:
		size_t count;
		long long total;
		long long missing;
		size_t eventmap[EVENT_MAX];
		list<Message> messageQueue;
	public:
		EventListener(void):count(0),total(0),missing(0)
		{
			memset(eventmap, 0, sizeof(eventmap));
		}
		void RecvMessage(const Message& m)
		{
			this->Lock();
			if(count < MESSAGEQUEUE_MAX/3)
			{
				messageQueue.push_back(m);
				total = total + 1LL;
				count = count + 0x1;
			}
			else
			{
				missing = missing + 1LL;
			}
			this->Unlock();
		}
		void Dispatch(void)
		{
			int dispatch = 0;

			this->Lock();
			dispatch = count % MESSAGEQUEUE_DISPATCH;
			if(count > MESSAGEQUEUE_MAX/9)
			{
				dispatch += count / 3;
			}
			this->Unlock();

			for(int i = 0; i < dispatch; i++)
			{
				Message message;

				this->Lock();
				message = messageQueue.front();
				eventmap[ (message.event % EVENT_MAX) ]++;
				this->messageQueue.pop_front();
				this->count = this->count - 1;
				this->Unlock();
				ProccessMessage(message);
			}
		}
		virtual const xstring GetInfo(void)=0;
	protected:
		virtual void ProccessMessage(const Message& m)=0;
	};
}

#endif//__EVENT_LISTENER_H__
