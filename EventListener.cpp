#include "EventListener.h"
#include "MessageQueueManager.h"

EventListener::EventListener(void):id(0),count(0),total(0),miss(0)
{
	memset(ecount, 0, sizeof(ecount));
	tid = pthread_self();
}
void EventListener::SetId(int i)
{
	id = i;
}
void EventListener::Listen(int e)
{
	MessageQueueManager::GetInstance().Listen(e, *this);
}
void EventListener::Dispatch(void)
{
	int dispatch = 0;

	this->Lock();
	dispatch = count % MESSAGEQUEUE_DISPATCH;
	if(count > MESSAGEQUEUE_MAX/9)
	{
		dispatch += count / 3;
	}
	this->Unlock();

	for(int i = 0; i < dispatch; i++)
	{
		Message message;

		this->Lock();
		message = messageQueue.front();
		ecount[ (message.event % EVENT_MAX) ]++;
		this->messageQueue.pop_front();
		this->count = this->count - 1;
		this->Unlock();
		ProccessMessage(message);
	}
}
void EventListener::SetName(const xstring& nm)
{
	this->name = nm;
}
void EventListener::SendMessage(const Message& m)
{
	MessageQueueManager::GetInstance().SendMessage(m);
}
void EventListener::RecvMessage(const Message& m)
{
	this->Lock();
	if(count < MESSAGEQUEUE_MAX/3)
	{
		messageQueue.push_back(m);
		total = total + 1LL;
		count = count + 0x1;
	}
	else
	{
		miss = miss + 1LL;
	}
	this->Unlock();
}
const size_t EventListener::EventCount(void)const
{
	size_t x = 0;
	for(int i = 0; i < EVENT_MAX; i++)
	{
		if(ecount[i])x++;
	}
	return x;
}
const xstring EventListener::EventInfo(void)const
{
	xstring s;

	for(int i = 0; i < EVENT_MAX; i++)
	{
		if(ecount[i])
		{
			s += xstring("%d,", i);
		}
	}
	return s;
}
const xstring EventListener::GetInfo(void)
{
	xstring s;
	s.format("\n\t"
			"Listener(%s)"
			".thread(%lX)"
			".run(%s)"
			".total(%lld)"
			".count(%d)"
			".miss(%lld)"
			".events(%d):"
			"{%s}",
			name.data(), 
			tid, 
			time.ToString("%Y-%m-%d %H:%M:%S").data(), total, 
			count, 
			miss, 
			EventCount(),
			EventInfo().data());
	return s;
}
