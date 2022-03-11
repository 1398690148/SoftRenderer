#include "CasliTimer.h"

using namespace std::chrono;

CasliTimer::CasliTimer() noexcept
{
	last = steady_clock::now();
}

float CasliTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float CasliTimer::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}
