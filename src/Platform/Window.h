#pragma once
#include <memory>
#include <optional>
#include "CasliWin.h"
#include <Graphics.h>
#include <Platform/Keyboard.h>
#include <Platform/Mouse.h>

class Window
{
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	void EnableCursor();
	void DisableCursor();
	bool CursorEnabled() const;
	static std::optional<int> ProcessMessages();
	Graphics& Gfx();
public:
	Keyboard kbd;
	Mouse mouse;
private:
	void ConfineCursor();

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	void FreeCursor();
	void ShowCursor();
	void HideCursor();
private:
	bool cursorEnabled = true;
	int width;
	int height;
	Graphics *pGfx;
	
	HWND ghMainWnd = NULL;
	HDC ghdcMainWnd = NULL;
	HBITMAP gdibMap = NULL;
	HBITMAP goldMap = NULL;
	unsigned char *gFbo = nullptr;
	std::vector<BYTE> rawBuffer;
	int gInputKeys[512] = { 0 };
};