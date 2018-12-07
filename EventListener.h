#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__
#include "Mutex.h"
#include "Message.h"

namespace Event
{
	class EventListener : public Mutex
	{
	public:
		virtual void RecvMessage(const Message&)=0;
		virtual const xstring GetInfo(void)=0;
	};
}

#endif//__EVENT_LISTENER_H__
