#ifndef __EVENT_MESSAGEQUEUEMANAGER_H__
#define __EVENT_MESSAGEQUEUEMANAGER_H__
#include "EventQueue.h"
#include "MessageQueue.h"

namespace Event
{
	class MessageQueueManager : public Mutex
	{
		protected:
			EventQueue eventQueue;
			MessageQueue messageQueue;
			MessageQueueManager(void){}
		public:
			const xstring GetInfo(void)
			{
				return eventQueue.GetInfo() + messageQueue.GetInfo();
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
			static MessageQueueManager& CreateInstance(void)
			{
				return *(new MessageQueueManager());
			}
			typedef map<int,Event>::iterator iterator;
	};
};

#endif//__EVENT_MESSAGEQUEUEMANAGER_H__
