#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__
#include <list>
#include <cmath>
#include "Mutex.h"
#include "Message.h"
#include "TimeValue.h"

#ifndef EVENT_MAX
#define EVENT_MAX 100
#endif//EVENT_MAX

#ifndef MESSAGEQUEUE_MAX
#define MESSAGEQUEUE_MAX 20000
#endif//MESSAGEQUEUE_MAX

#ifndef MESSAGEQUEUE_DISPATCH
#define MESSAGEQUEUE_DISPATCH 20
#endif//MESSAGEQUEUE_DISPATCH

class EventListener : Mutex
{
protected:
	signed			id;
	size_t			count;
	size_t			ecount[EVENT_MAX];
	long long		total;
	long long		miss;
	string			name;
	pthread_t		tid;
	TimeValue		time;
	list<Message>	messageQueue;
public:
	EventListener(void);
	void SetId(int);
	void Listen(int);
	void Dispatch(void);
	void SetName(const xstring&);
	void SendMessage(const Message&);
	void RecvMessage(const Message&);
	const size_t EventCount(void)const;
	const xstring EventInfo(void)const;
	virtual const xstring GetInfo(void);
protected:
	virtual void ProccessMessage(const Message& m)=0;
};

#endif//__EVENT_LISTENER_H__
