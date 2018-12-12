#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "MessageQueueManager.h"

#define EVENT_MAX	100
#define THREAD_MAX	20

class Test : public EventListener
{
	public:
		void ProccessMessage(const Message& message)
		{
			for(int i = 0; i < 1000; i++);
		}
};

void *thread(void *p)
{
	Test test;
	Message m;
	TimeValue now;

	for(int i = 0; i < EVENT_MAX; i += (random() % EVENT_MAX/5))
	{
		test.Listen(random() % EVENT_MAX);
		srandom(now.Diff().Usecond() + random());
	}

	m.from = pthread_self();
   	m.event = *(unsigned*)p;
	for(int i = 0; i < sizeof(m.x.buf); i++)
	{
		m.x.buf[i] = 'A' + (random() % 26);
	}
	m.x.buf[sizeof(m.x.buf)-1] = 0;

	test.SetId(m.event);

	while(1)
	{
		m.event = (random() % EVENT_MAX);
		if( now.Diff().Usecond() > m.event )
		{
			test.SendMessage(m);
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
