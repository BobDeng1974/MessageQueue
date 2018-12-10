#ifndef __EVENT_MQMANAGER_H__
#define __EVENT_MQMANAGER_H__
#include "TimeValue.h"
#include "EventQueue.h"
#include "MessageQueue.h"

namespace Event
{
	class MQManager : public Mutex
	{
		protected:
			TimeValue time;
			EventQueue equeue;
			MessageQueue mqueue;
		public:
			const TimeValue& GetTime(void)const
			{
				return time;
			}
			const size_t Count(void)const
			{
				return mqueue.Count();
			}
			const xstring GetInfo(void)
			{
				return mqueue.GetInfo() + equeue.GetInfo();
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
