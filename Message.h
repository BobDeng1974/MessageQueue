#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__
#include <map>
#include <pthread.h>
#include "xstring.h"

namespace Event
{
	class Message
	{
		public:
			int event;
			xstring data;
			pthread_t id;
		public:
			Message():id(-1UL),event(-1)
			{
			}
	};
};

#endif//__EVENT_MESSAGE_H__
