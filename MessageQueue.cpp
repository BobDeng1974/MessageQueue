#include "MessageQueue.h"

MessageQueue::MessageQueue():count(0),total(0),miss(0)
{
	memset(event, 0, sizeof(event));
}
const xstring MessageQueue::GetInfo(void)
{
	xstring s;
	int lineoff = 0;
	time_t sec = time.Diff().Second();
	long long dispatch = (total - count);

	s += xstring("\nMessageQueue(%p).count(%d)", this, count);
	s += xstring(".recieve(%lld / %d = %lld/s)", total, sec, total/sec);
	s += xstring(".dispatch(%lld / %d = %lld/s)", dispatch, sec, dispatch/sec);
	s += xstring(".miss(%lld / %d = %lld/s)",  miss, sec, miss/sec);
	for(int i = 0; i < EVENT_MAX; i++)
	{
		int x = (i/10 + i*10) % EVENT_MAX;
		if(event[x])
		{
			if( (lineoff++ % 10) == 0 )
			{
				s += "\n\t";
			}
			s += xstring("[%2d->%-8lld], ", x, event[x]);
		}
	}
	return s;
}
void MessageQueue::Push(const Message& m)
{
	Lock();
	if(count < MESSAGEQUEUE_MAX)
	{
		event[ (m.event % EVENT_MAX) ]++;
		queue.push_back(m);	
		total = total + 1LL;
		count++;
	}
	else
	{
		miss = miss +1LL;
	}
	Unlock();
}
void MessageQueue::Dispatch(EventQueue& equeue)
{
	int i = 0;
	int dispatch = MESSAGEQUEUE_DISPATCH;

	Lock();
	if(count > MESSAGEQUEUE_MAX/3)
	{
		dispatch = count / 10 + miss / 10;
	}
	while(!queue.empty())
	{
		equeue.Dispatch( queue.front() );
		this->queue.pop_front();count--;
		if(i++ > dispatch)
		{
			break;
		}
	}
	Unlock();
}
