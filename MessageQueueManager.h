#ifndef __EVENT_MESSAGEQUEUEMANAGER_H__
#define __EVENT_MESSAGEQUEUEMANAGER_H__
#include "EventQueue.h"
#include "MessageQueue.h"

class MessageQueueManager : public Mutex
{
	protected:
		EventQueue eventQueue;
		MessageQueue messageQueue;
		MessageQueueManager(void){}
	public:
		const xstring GetInfo(void)
		{
			return eventQueue.GetInfo()
				 + messageQueue.GetInfo();
		}
		void Dispatch(void)
		{
			messageQueue.Dispatch(eventQueue);
		}
		void Listen(int e, EventListener& l)
		{
			eventQueue.Listen(e, l);
		}
		void SendMessage(const Message& message)
		{
			messageQueue.Push(message);
		}
		static MessageQueueManager& GetInstance(void)
		{
			static MessageQueueManager i; return i;
		}
		static MessageQueueManager* CreateInstance(void)
		{
			return new MessageQueueManager();
		}
		typedef map<int,Event>::iterator iterator;
};

#endif//__EVENT_MESSAGEQUEUEMANAGER_H__
