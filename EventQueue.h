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
			long long count[EVENT_MAX];
		public:
			EventQueue(void)
			{
				memset(count, 0, sizeof(count));
			}
			const xstring GetInfo(void)
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
			void Listen(int event, EventListener& listener)
			{
				Lock();
				queue[ (event % EVENT_MAX) ].Listen(listener);
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
