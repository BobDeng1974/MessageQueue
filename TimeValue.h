#ifndef __TIMEVALUE_H__
#define __TIMEVALUE_H__
#include <sys/time.h>

class TimeValue
{
protected:
	struct timeval value;
public:
	TimeValue(void)
	{
		Update();
	}
	const time_t Second(void)const
	{
		return value.tv_sec;
	}
	const time_t Usecond(void)const
	{
		return value.tv_usec;
	}
	bool Update(void)
	{
		return gettimeofday(&value, 0)==0;
	}
	TimeValue& operator-=(const TimeValue& t)
	{
		value.tv_sec -= t.value.tv_sec;
		value.tv_usec -= t.value.tv_usec;
		if(value.tv_usec < 0)
		{
			value.tv_sec -= 1;
			value.tv_usec += 1000000;
		}
		return *this;
	}
};

#endif//__TIMEVALUE_H__
