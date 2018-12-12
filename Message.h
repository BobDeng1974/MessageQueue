#ifndef __EVENT_MESSAGE_H__
#define __EVENT_MESSAGE_H__
#include <map>
#include <pthread.h>
#include "xstring.h"

typedef union
{
	char scvalue;
	short ssvalue;
	signed int sivalue;
	signed long int slvalue;
	signed long long int sllvalue;

	unsigned char ucvalue;
	unsigned short usvalue;
	unsigned int uvalue;
	unsigned long ulvalue;
	unsigned long long ullvalue;

	float fvalue;
	double dvalue;

	char buf[256];

	struct{
		char name[32];
		int value;
	}nvalue;
}Messagex;

class Message
{
	public:
		unsigned event;
		pthread_t from;
		Messagex x;
	public:
		Message():from(-1UL),event(-1)
		{
		}
};

#endif//__EVENT_MESSAGE_H__
