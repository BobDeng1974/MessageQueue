#ifndef __EVENT_EVENT_H__
#define __EVENT_EVENT_H__
#include <map>
#include "Mutex.h"
#include "Message.h"
#include "EventListener.h"
using namespace std;

class Event : Mutex
{
protected:
	map<EventListener*,bool> listeners;
public:
	const xstring GetInfo(void);
	void Listen(EventListener&);
	void Dispatch(const Message&);
	typedef map<EventListener*,bool>::iterator iterator;
};

#endif//__EVENT_EVENT_H__
