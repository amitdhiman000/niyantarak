#include "Stoppable.h"
#include <iostream>

Stoppable::Stoppable()
: mFuture(mPromise.get_future())
{
}

Stoppable::~Stoppable()
{
	stop();
}

Stoppable::Stoppable(Stoppable&& other)
: mPromise(std::move(other.mPromise))
, mFuture(std::move(other.mFuture))
{
	std::cout << "Move Constructor is called" << std::endl;
}

Stoppable & Stoppable::operator=(Stoppable&& other)
{
	std::cout << "Move Assignment is called" << std::endl;
	mPromise = std::move(other.mPromise);
	mFuture = std::move(other.mFuture);
	return *this;
}

void Stoppable::operator()()
{
	return run();
}

void Stoppable::stop()
{
	mPromise.set_value();
}

bool Stoppable::stopRequested() const
{
	if (mFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
		return false;
	return true;
}
