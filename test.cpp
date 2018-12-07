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

class TimeDiff: public Mutex
{
	private:
		TimeValue last;
		TimeValue now;
	public:
		size_t Diff(void)
		{
			time_t diff;

			Lock();
			now.Update();
			now -= last;
			diff = now.Second() * 1000000 + now.Usecond();
			Unlock();

			return diff;
		}
		void Update(void)
		{
			Lock();
			last.Update();
			Unlock();
		}
};

TimeDiff td;

class Test : public EventListener
{
	protected:
		list<Message> queue;	
	public:
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
				if(td.Diff() > 1000000 )
				{
					printf("[%lx].m(%lx, %d).%s\n", pthread_self(), m.id, m.event, m.data.data());
					td.Update();
				}
				queue.pop_front();
			}
			Unlock();
		}

};

void *thread(void *p)
{
	Test test;
	int e = event++;
	int count = 0;
	TimeValue last;

	mq.Listen(e%5, test);

	while(1)
	{
		TimeValue now;
		if( (now -= last).Usecond() > 1)
		{
			Message m;
			m.id = pthread_self();
			m.event = random()%5;
			TimeValue now;
			xstring s = mq.GetInfo();
			if(count++ % 100 == 0)
				m.data.format("time(%lu,%lu).mq(%s)", now.Second(), now.Usecond(), s.data());
			srandom(now.Usecond());
			mq.SendMessage(m);
			last.Update();
		}
		test.ProcMessage();
		usleep(1);
	}
}
int main(void)
{

	for(int i = 0; i < 10; i++)
	{
		pthread_t tid = 0;
		pthread_create(&tid, 0, thread, 0);
	}
	while(1)
	{
		mq.Dispatch();
		usleep(1);
	}
}
