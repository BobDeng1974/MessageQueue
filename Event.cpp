#include "Event.h"

const xstring Event::GetInfo(void)
{
	xstring s;

	Lock();
	s += xstring(".listeners(%d)", listeners.size());
	for(iterator i = listeners.begin(); i != listeners.end(); i++)
	{
		s += i->first->GetInfo();
	}
	Unlock();
	return s;
}
void Event::Listen(EventListener& l)
{
	Lock();
	listeners[&l] = true;
	Unlock();
}
void Event::Dispatch(const Message& m)
{
	Lock();
	for(iterator i = listeners.begin(); i != listeners.end(); i++)
	{
		i->first->RecvMessage(m);
	}
	Unlock();
}
