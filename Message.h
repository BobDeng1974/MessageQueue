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
			char buffer[512];
			xstring data;
			pthread_t id;
			map<xstring,xstring> xx;
		public:
			Message():id(-1UL),event(-1)
			{
				memset(buffer, 'x', sizeof(buffer));
			}
	};
};

#endif//__EVENT_MESSAGE_H__
