#ifndef __EVENT_MESSAGEQUEUE_H__
#define __EVENT_MESSAGEQUEUE_H__
#include <list>
#include <cmath>
#include "Mutex.h"
#include "TimeValue.h"
#include "EventQueue.h"
using namespace std;

namespace Event
{
	class MessageQueue : public Mutex 
	{
		protected:
			size_t count;
			long long total;
			long long missing;
			TimeValue time;
			list<Message> queue;
		public:
			MessageQueue():count(0),total(0),missing(0)
			{
			}
			const size_t Count(void)const
			{
				return count;
			}
			const long long Total(void)const
			{
				return total;
			}
			const long long Missing(void)const
			{
				return missing;
			}
			const xstring GetInfo(void)
			{
				time_t sec = time.Diff().Second();
				long long dispatch = (total - count);

				return xstring("\nMessageQueue(%p).count(%d)"
								".recieve(%lld / %d = %lld/s)"
								".dispatch(%lld / %d = %lld/s)"
								".missing(%lld / %d = %lld/s)",
								this, count,
								total, sec, total/sec,
								dispatch, sec, dispatch/sec,
								missing, sec, missing/sec);
			}
			void Push(const Message& m)
			{
				Lock();
				if(count < MESSAGEQUEUE_MAX)
				{
					queue.push_back(m);	
					total = total + 1LL;
					count++;
				}
				else
				{
					missing = missing +1LL;
				}
				Unlock();
			}
			void Dispatch(EventQueue& equeue)
			{
				int i = 0;
				int dispatch = MESSAGEQUEUE_DISPATCH;

				Lock();
				if(count > MESSAGEQUEUE_MAX/3)
				{
					dispatch = count / 10 + missing / 10;
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
			typedef list<Message>::iterator iterator;
	};
};

#endif//__EVENT_QUEUE_H__
