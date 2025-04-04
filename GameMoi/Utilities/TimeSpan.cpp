#include "TimeSpan.h"

TimeSpan::TimeSpan(void)
{
}

TimeSpan::~TimeSpan(void)
{
}

TimeSpan::TimeSpan(UINT64 ticks)
{
	this->_ticks = ticks;
}

float TimeSpan::getMiliSeconds()
{
	return ((float)_ticks) / TicksPerMilisecond;
}

UINT64 TimeSpan::getTicks()
{
	return _ticks;
}

TimeSpan TimeSpan::operator+(TimeSpan ts) const
{
	return TimeSpan(this->_ticks + ts._ticks);
}

TimeSpan TimeSpan::operator+(UINT64 ts) const
{
	return TimeSpan(this->_ticks + ts);
}