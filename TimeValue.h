#ifndef __TIMEVALUE_H__
#define __TIMEVALUE_H__
#include <string>
#include <time.h>
#include <sys/time.h>
using namespace std;

class TimeValue
{
protected:
	struct timeval value;
public:
	TimeValue(void)
	{
		this->Update();
	}
	bool Update(void)
	{
		return gettimeofday(&value, 0)==0;
	}
public:
	const time_t Day(void)const
	{
		return value.tv_sec / 60 / 60 / 24;
	}
	const time_t Hour(void)const
	{
		return value.tv_sec / 60 / 60;
	}
	const time_t Min(void)const
	{
		return value.tv_sec / 60;
	}
	const time_t& Second(void)const
	{
		return value.tv_sec;
	}
	const time_t Msecond(void)const
	{
		return value.tv_usec / 1000;
	}
	const time_t Usecond(void)const
	{
		return value.tv_usec;
	}
	const long long Mdiff(void)const
	{
		long long sec = sec = value.tv_sec;
		long long usec = usec = value.tv_usec;
		return sec * 1000LL + usec / 1000LL;
	}
	const long long Udiff(void)const
	{
		long long sec = sec = value.tv_sec;
		long long usec = usec = value.tv_usec;
		return usec + sec * 1000000LL;
	}
	const TimeValue Diff(void)const
	{
		TimeValue now;
		return (now - *this);
	}
	const TimeValue operator-(const TimeValue& t)const
	{
		TimeValue r;

		r.value.tv_sec = value.tv_sec - t.value.tv_sec;
		r.value.tv_usec = value.tv_usec - t.value.tv_usec;

		if(r.value.tv_usec < 0)
		{
			r.value.tv_sec -= 1;
			r.value.tv_usec += 1000000;
		}
		return r;
	}
	const string ToString(const char *format)const
	{
		char buf[128];
		struct tm tm;

		gmtime_r(&value.tv_sec, &tm);
		strftime(buf, sizeof(buf), format, &tm);

		return buf;
	}
};

#endif//__TIMEVALUE_H__
