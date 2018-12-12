#ifndef __EVENT_EVENTQUEUE_H__
#define __EVENT_EVENTQUEUE_H__
#include <map>
#include "Mutex.h"
#include "Event.h"
using namespace std;

class EventQueue : public Mutex
{
	protected:
		long long count[EVENT_MAX];
		map<unsigned, class Event> queue;
	public:
		EventQueue(void);
		void Dispatch(Message&);
		const xstring GetInfo(void);
		void Listen(unsigned, EventListener&);
		typedef map<unsigned, class Event>::iterator iterator;
};

#endif//__EVENT_QUEUE_H__
