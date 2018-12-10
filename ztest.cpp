#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "MessageQueueManager.h"
using namespace Event;

int event = 0;
int countx = 0;
int event_max = 100;
TimeValue tt;
MessageQueueManager &manager = MessageQueueManager::GetInstance();

class Test : public EventListener
{
	protected:
		int id;
	public:
		Test(int n):id(n)
		{
		}
		const xstring GetInfo(void)
		{
			xstring s;
			Lock();
			s.format("Test[%p].total(%lld).missing(%lld).count(%d),", this, total, missing, count);
			Unlock();
			return s;
		}
		void ProcMessage()
		{
			int i = 0;
			int dispatch = MESSAGEQUEUE_DISPATCH;

			Lock();
			if(count > MESSAGEQUEUE_MAX/3)
			{
				dispatch = count / 10;
			}
			while(!messageQueue.empty())
			{
				Message &m = messageQueue.front();
				const TimeValue td = tt.Diff();

				if(td.Second() > 0)
				{
					xstring s;
					const TimeValue& t = manager.GetTime().Diff();

					m.buffer[sizeof(m.buffer) - 1] = 0;
					s.format("manager.total(%lld).missing(%lld).count(%d).runtime(%d).div(%lld)", 
							manager.Total(), manager.Missing(), manager.Count(), t.Second(), manager.Total()/t.Second());
					printf("%d.[%lx].m(%lx, %d).%s.%s\n", id, pthread_self(), m.id, m.event, m.buffer, s.data());
					tt.Update();
				}
				messageQueue.pop_front();count--;
				if(i++ > dispatch)
				{
					break;
				}
			}
			Unlock();
		}
};

void *thread(void *p)
{
	Test test(*(int*)p);
	TimeValue now;

	for(int i = 0; i < event_max; i += random() % 10)
	{
		manager.Listen(random() % event_max, test);
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
			manager.SendMessage(m);
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
		manager.Dispatch();
		usleep(1);
	}
}
