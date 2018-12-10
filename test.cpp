#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "TimeValue.h"
#include "MQManager.h"
using namespace Event;

MQManager mq;
int event = 0;
int countx = 0;

TimeValue tt;

class Test : public EventListener
{
	protected:
		int id;
		list<Message> queue;	
	public:
		Test(int n):id(n)
		{
		}
		void RecvMessage(const Message& m)
		{
			Lock();
			queue.push_back(m);
			Unlock();
		}
		const xstring GetInfo(void)
		{
			xstring s;
			Lock();
			s.format("Test[%p].mcount(%d),", this, queue.size());
			Unlock();
			return s;
		}
		void ProcMessage()
		{

			Lock();
			while(!queue.empty())
			{
				Message &m = queue.front();
				const TimeValue td = tt.Diff();

				if(td.Second() > 0)
				{
					xstring s;
					const TimeValue& t = mq.GetTime().Diff();

					m.buffer[sizeof(m.buffer) - 1] = 0;
					s.format("mq.count(%d).runtime(%d).div(%d)", mq.Count(), t.Second(), mq.Count()/t.Second());
					printf("%d.[%lx].m(%lx, %d).%s.%s\n", id, pthread_self(), m.id, m.event, m.buffer, s.data());
					tt.Update();
				}
				queue.pop_front();
			}
			Unlock();
		}

};

int event_max = 100;
void *thread(void *p)
{
	Test test(*(int*)p);
	TimeValue now;

	for(int i = 0; i < event_max; i += random() % 10)
	{
		mq.Listen(random() % event_max, test);
		srandom(now.Diff().Usecond());
	}

	while(1)
	{
		const TimeValue t = now.Diff();

		if( t.Usecond() > 1)
		{
			Message m;
			m.id = pthread_self();
			m.event = random()%event_max;
			for(int i = 0; i < sizeof(m.buffer); i++)
			{
				m.buffer[i] = 'A' + i % 26;
			}
			srandom(t.Usecond());
			mq.SendMessage(m);
			now.Update();
		}
		test.ProcMessage();
		usleep(1);
	}
}
int thread_max = 20;
int main(void)
{

	for(int i = 0; i < thread_max; i++)
	{
		pthread_t tid = 0;
		pthread_create(&tid, 0, thread, &i);
	}
	while(1)
	{
		mq.Dispatch();
		usleep(1);
	}
}
