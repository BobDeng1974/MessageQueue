#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "MessageQueueManager.h"
using namespace Event;

#define EVENT_MAX	100
#define THREAD_MAX	20

class Test : public EventListener
{
	protected:
		int id;
		pthread_t tid;
		TimeValue time;
		MessageQueueManager &manager;
	public:
		Test():id(0),manager(MessageQueueManager::GetInstance())
		{
			for(int i = 0; i < EVENT_MAX; i++)
			{
				eventmap[i] = 0;
			}
		}
		void SetId(int iid)
		{
			this->id = iid;
		}
		const xstring GetInfo(void)
		{
			xstring s;
			s.format("\n\t"
					"Listener(%p)"
					".thread(%lX)"
					".run(%s)"
					".total(%lld)"
					".count(%d)"
					".miss(%lld)"
					".events(%d):"
					"{%s}",
					this, 
					tid, 
					time.ToString("%Y-%m-%d %H:%M:%S").data(), total, 
					count, 
					missing, 
					EventCount(),
					GetEvents().data());
			return s;
		}
		int EventCount(void)
		{
			int r = 0;
			for(int i = 0; i < EVENT_MAX; i++)
			{
				if(eventmap[i])r++;
			}
			return r;
		}
		const xstring GetEvents(void)
		{
			xstring s;
			for(int i = 0; i < EVENT_MAX; i++)
			{
				if(eventmap[i])
				{
					s += xstring("%d,", i);
				}
			}
			return s;
		}
		void ProccessMessage(const Message& message)
		{
			for(int i = 0; i < 1000; i++);
		}
};

void *thread(void *p)
{
	Test test;
	TimeValue now;
	MessageQueueManager &manager = MessageQueueManager::GetInstance();

	for(int i = 0; i < EVENT_MAX; i += (random() % EVENT_MAX/5))
	{
		manager.Listen(random() % EVENT_MAX, test);
		srandom(now.Diff().Usecond() + random());
	}
	test.SetId( *(int*)p );

	while(1)
	{
		const TimeValue t = now.Diff();

		if( t.Usecond() > 1)
		{
			Message m;
			m.id = pthread_self();
			m.event = random() % EVENT_MAX;
			for(int i = 0; i < sizeof(m.x.buf); i++)
			{
				m.x.buf[i] = 'A' + i % 26;
			}
			m.x.buf[sizeof(m.x.buf)-1] = 0;
			//srandom(t.Usecond());
			manager.SendMessage(m);
			now.Update();
		}
		test.Dispatch();
		usleep(150);
	}
}
int main(void)
{
	TimeValue time;
	MessageQueueManager &manager = MessageQueueManager::GetInstance();

	for(int i = 0; i < THREAD_MAX; i++)
	{
		pthread_t t = pthread_t(-1);
		pthread_create(&t, 0, thread, &i);
	}
	while(1)
	{
		manager.Dispatch();
		usleep(100);
		if(time.Diff().Second() > 1)
		{
			printf("%s", manager.GetInfo().data());
			fflush(stdout);
			time.Update();
		}
	}
}
