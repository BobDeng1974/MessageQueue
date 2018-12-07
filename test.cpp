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
			xstring s;

		   if( td.Diff() > 1000000)
		   {
			  s.format("mq.count(%d).timediff(%d).div(%d)", mq.Count(), mq.DiffTime(), mq.Count()/(mq.DiffTime()/1000000));
		   }
			Lock();
			while(!queue.empty())
			{
				Message &m = queue.front();
				if(td.Diff() > 1000000 && s.length() > 10)
				{
					printf("[%lx].m(%lx, %d).%s.%s\n", pthread_self(), m.id, m.event, m.data.data(), s.data());
					td.Update();
				}
				queue.pop_front();
			}
			Unlock();
		}

};

int event_max = 100;
void *thread(void *p)
{
	Test test;
	TimeValue last;

	for(int i = 0; i < event_max; i += random() % 10)
	{
		mq.Listen(random() % event_max, test);
		srandom(last.Usecond());
	}

	while(1)
	{
		TimeValue now;
		if( (now -= last).Usecond() > 1)
		{
			Message m;
			m.id = pthread_self();
			m.event = random()%event_max;
			m.data = "jjjjjjjjjjjjjjjjjjjjjjjjjjlajflajfkajfljasdlfjalskdfjafjkajflajsdlfkjaslkdfjjkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk88888";
			TimeValue now;
			srandom(now.Usecond());
			mq.SendMessage(m);
			last.Update();
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
		pthread_create(&tid, 0, thread, 0);
	}
	while(1)
	{
		mq.Dispatch();
		usleep(1);
	}
}
