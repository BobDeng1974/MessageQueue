#ifndef __EVENT_EVENT_H__
#define __EVENT_EVENT_H__
#include <map>
#include "Mutex.h"
#include "Message.h"
#include "EventListener.h"
using namespace std;

namespace Event
{
	class Event : Mutex
	{
	protected:
		map<EventListener*,bool> listeners;
	public:
		const xstring GetInfo(void)
		{
			xstring s("Event[%p]", this);

			Lock();
			s += xstring(".count(%d)", listeners.size());
			for(iterator i = listeners.begin(); i != listeners.end(); i++)
			{
				s += i->first->GetInfo();
			}
			Unlock();
			return s;
		}
		void Listen(EventListener& l)
		{
			Lock();
			listeners[&l] = true;
			Unlock();
		}
		void Dispatch(const Message& m)
		{
			Lock();
			for(iterator i = listeners.begin(); i != listeners.end(); i++)
			{
				i->first->RecvMessage(m);
			}
			Unlock();
		}
		typedef map<EventListener*,bool>::iterator iterator;
	};
};

#endif//__EVENT_EVENT_H__
