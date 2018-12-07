#ifndef __EVENT_MQMANAGER_H__
#define __EVENT_MQMANAGER_H__
#include "EventQueue.h"
#include "MessageQueue.h"

namespace Event
{
	class MQManager : public Mutex
	{
		protected:
			EventQueue equeue;
			MessageQueue mqueue;
		public:
			const xstring GetInfo(void)
			{
				xstring s;
				Lock();
				s += mqueue.GetInfo();
				s += equeue.GetInfo();
				Unlock();
				return s;
			}
			void Listen(int event, EventListener& listener)
			{
				equeue.Listen(event, listener);
			}
			void SendMessage(const Message& m)
			{
				mqueue.Push(m);
			}
			void Dispatch(void)
			{
				mqueue.Dispatch(equeue);
			}
			typedef map<int,Event>::iterator iterator;
	};
};

#endif//__EVENT_MANAGER_H__
