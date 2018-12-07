#ifndef __EVENT_TIMEDIFF_H__
#define __EVENT_TIMEDIFF_H__
#include "Mutex.h"
#include "TimeValue.h"

namespace Event
{
	class TimeDiff : public Mutex
	{
		protected:
			TimeValue value;
		public:
			void Update(void)
			{
				Lock();
				value.Update();
				Unlock();
			}
			const time_t Diff(void)const
			{
				TimeValue now;

				now -= value;

				return now.Second() * 1000 * 1000 + now.Usecond();
			}
	};
};

#endif//__EVENT_TIMEDIFF_H__
