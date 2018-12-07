#ifndef __EVENT_EVENTQUEUE_H__
#define __EVENT_EVENTQUEUE_H__
#include <map>
#include "Mutex.h"
#include "Event.h"
using namespace std;

namespace Event
{
	class EventQueue : public Mutex
	{
		protected:
			map<int,Event> queue;
		public:
			const xstring GetInfo(void)
			{
				xstring s;
				Lock();
				for(iterator i = queue.begin(); i != queue.end(); i++)
				{
					s += xstring("\n[%d]{", i->first);
					s += i->second.GetInfo();
				    s += xstring("}");
				}
				Unlock();
				return s;
			}
			void Listen(int event, EventListener& listener)
			{
				Lock();
				queue[event].Listen(listener);
				Unlock();
			}
			void Dispatch(Message& m)
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
			typedef map<int,Event>::iterator iterator;
	};
};

#endif//__EVENT_QUEUE_H__
