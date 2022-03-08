#pragma once
#include <memory>
#include <optional>
#include "CasliWin.h"
#include <Graphics.h>

class RENDERER_API Window
{
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
	Graphics *pGfx;
	HWND ghMainWnd = NULL;
	HDC ghdcMainWnd = NULL;
	HBITMAP gdibMap = NULL;
	HBITMAP goldMap = NULL;
	unsigned char *gFbo = nullptr;
	int gInputKeys[512] = { 0 };
};