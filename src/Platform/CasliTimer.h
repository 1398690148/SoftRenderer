#pragma once
#include <chrono>
#include <RendererAPI.h>

class RENDERER_API CasliTimer
{
public:
	CasliTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};