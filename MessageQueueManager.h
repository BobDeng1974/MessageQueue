#ifndef __EVENT_MESSAGEQUEUEMANAGER_H__
#define __EVENT_MESSAGEQUEUEMANAGER_H__
#include "TimeValue.h"
#include "EventQueue.h"
#include "MessageQueue.h"

namespace Event
{
	class MessageQueueManager : public Mutex
	{
		protected:
			TimeValue timeValue;
			EventQueue eventQueue;
			MessageQueue messageQueue;
			MessageQueueManager(void){}
		public:
			const TimeValue& GetTime(void)const
			{
				return timeValue;
			}
			const size_t Count(void)const
			{
				return messageQueue.Count();
			}
			const long long Total(void)const
			{
				return messageQueue.Total();
			}
			const long long Missing(void)const
			{
				return messageQueue.Missing();
			}
			const xstring GetInfo(void)
			{
				return messageQueue.GetInfo() + eventQueue.GetInfo();
			}
			void Listen(int event, EventListener& listener)
			{
				eventQueue.Listen(event, listener);
			}
			void SendMessage(const Message& m)
			{
				messageQueue.Push(m);
			}
			void Dispatch(void)
			{
				messageQueue.Dispatch(eventQueue);
			}
			static MessageQueueManager& GetInstance(void)
			{
				static MessageQueueManager instance;return instance;
			}
			typedef map<int,Event>::iterator iterator;
	};
};

#endif//__EVENT_MESSAGEQUEUEMANAGER_H__
