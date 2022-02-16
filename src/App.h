#pragma once
#include "./Platform/Window.h"

class App
{
public:
	App();
	// master frame / message loop
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
};