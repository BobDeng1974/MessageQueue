#include "EventQueue.h"

EventQueue::EventQueue(void)
{
	memset(count, 0, sizeof(count));
}
const xstring EventQueue::GetInfo(void)
{
	xstring s;
	Lock();
	for(iterator i = queue.begin(); i != queue.end(); i++)
	{
		s += xstring("\nEvent[%d].count(%lld)", i->first, count[ (i->first % EVENT_MAX) ]);
		s += i->second.GetInfo();
	}
	Unlock();
	return s;
}
void EventQueue::Listen(int event, EventListener& listener)
{
	Lock();
	queue[ (event % EVENT_MAX) ].Listen(listener);
	Unlock();
}
void EventQueue::Dispatch(Message& m)
{
	Lock();
	for(iterator i = queue.begin(); i != queue.end(); i++)
	{
		if(i->first == m.event)
		{
			i->second.Dispatch(m);
		}
	}
	Unlock();
}
