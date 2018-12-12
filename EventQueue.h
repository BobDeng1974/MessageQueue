#ifndef __EVENT_EVENTQUEUE_H__
#define __EVENT_EVENTQUEUE_H__
#include <map>
#include "Mutex.h"
#include "Event.h"
using namespace std;

class EventQueue : public Mutex
{
	protected:
		map<int,Event> queue;
		long long count[EVENT_MAX];
	public:
		EventQueue(void);
		void Dispatch(Message&);
		const xstring GetInfo(void);
		void Listen(int, EventListener&);
		typedef map<int,Event>::iterator iterator;
};

#endif//__EVENT_QUEUE_H__
