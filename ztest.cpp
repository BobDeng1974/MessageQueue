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
		bool eventmap[EVENT_MAX];
		MessageQueueManager &manager;
	public:
		Test(int n):id(n),manager(MessageQueueManager::GetInstance())
		{
			for(int i = 0; i < EVENT_MAX; i++)
			{
				eventmap[i] = false;
			}
		}
		const xstring GetInfo(void)
		{
			xstring s;
			Lock();
			s.format("\n\tListener(%p).thread(%lX).run(%s).total(%lld).count(%d).miss(%lld).listenning(%s){%d}",
					this, tid, time.ToString("%Y-%m-%d %H:%M:%S").data(), total, count, missing, GetEvents().data(), EventCount());
			Unlock();
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
		void ProcMessage(void)
		{
			xstring s;
			int dispatch = MESSAGEQUEUE_DISPATCH;

			Lock();
			if(count > MESSAGEQUEUE_MAX/10)
			{
				dispatch = count / 10 + missing / 10;
			}
			while(!messageQueue.empty())
			{
				Message &message = messageQueue.front();
				eventmap[message.event] = true;
				messageQueue.pop_front();
				this->count--;
				if(--dispatch < 1)
					break;
				for(int i = 0; i < 1000; i++);
			}
			Unlock();
			tid = pthread_self();
			time.Update();
		}
};

void *thread(void *p)
{
	Test test(*(int*)p);
	TimeValue now;
	MessageQueueManager &manager = MessageQueueManager::GetInstance();

	for(int i = 0; i < EVENT_MAX; i += (random() % EVENT_MAX/5))
	{
		manager.Listen(random() % EVENT_MAX, test);
		srandom(now.Diff().Usecond());
	}

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
			srandom(t.Usecond());
			manager.SendMessage(m);
			now.Update();
		}
		test.ProcMessage();
		usleep(200);
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
