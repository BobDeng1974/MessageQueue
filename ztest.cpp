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
			int dispatch = MESSAGEQUEUE_DISPATCH;

			Lock();
			if(count > MESSAGEQUEUE_MAX/10)
			{
				dispatch = count / 10 + missing / 10;
			}
			while(!messageQueue.empty())
			{
				Message &message = messageQueue.front();
				const TimeValue& t = manager.GetTime().Diff();

				if(tt.Diff().Second() > 1 && t.Second() > 0)
				{
					tt.Update();
					printf("listener[%d]"
							".total(%lld)"
							".miss(%lld)"
							".count(%ld)"
							".thread(%lx,%lx)___"
							"manager"
							".total(%lld)"
							".miss(%lld)"
							".count(%d)"
							".speed(%lld)"
							".time(%d)___"
							"message"
							".event(%d)"
							".buf(%d)\n%s\n\n",
							this->id,
							this->total,
							this->missing,
							this->count,
							pthread_self(), 
							message.id, 
							manager.Total(), 
							manager.Missing(), 
							manager.Count(), 
							manager.Total()/t.Second(),
							t.Second(),
							message.event, 
							strlen(message.x.buf),
							message.x.buf);
				}
				messageQueue.pop_front();
				count--;
				if(--dispatch < 1)
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
		usleep(1000);
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
		usleep(100);
	}
}
