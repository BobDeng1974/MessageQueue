#ifndef __EVENT_MESSAGEQUEUE_H__
#define __EVENT_MESSAGEQUEUE_H__
#include <list>
#include "Mutex.h"
#include "EventQueue.h"
using namespace std;

namespace Event
{
	class MessageQueue : public Mutex 
	{
		protected:
			size_t count;
			list<Message> queue;
		public:
			MessageQueue():count(0)
			{
			}
			const xstring GetInfo(void)
			{
				int size = 0;
				Lock();
				size = queue.size();
				Unlock();
				return xstring("count(%d),size(%d)", count, size);
			}
			void Push(const Message& m)
			{
				Lock();
				count++;
				queue.push_back(m);	
				Unlock();
			}
			void Dispatch(EventQueue& equeue)
			{
				Lock();
				while(!queue.empty())
				{
					equeue.Dispatch( queue.front() );
					this->queue.pop_front();
				}
				Unlock();
			}
			typedef list<Message>::iterator iterator;
	};
};

#endif//__EVENT_QUEUE_H__
