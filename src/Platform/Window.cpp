#include "Window.h"

Window::Window(int width, int height, const char *windowName) : width(width), height(height)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "WindowClassName";
	RegisterClass(&wc);

	// 调整窗口大小和位置
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, GetWindowLong(ghMainWnd, GWL_STYLE), 0);

	ghMainWnd = CreateWindow("WindowClassName", windowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), this);

	if (ghMainWnd == NULL)
	{
		throw;
	}

	HDC tmpHdc = GetDC(ghMainWnd);
	ghdcMainWnd = CreateCompatibleDC(tmpHdc);
	ReleaseDC(ghMainWnd, tmpHdc);

	// 创建DIB
	LPVOID ptr;
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),
		width, -height, 1, 32, BI_RGB,
		width * height * 4, 0, 0, 0, 0 } };
	gdibMap = CreateDIBSection(ghdcMainWnd, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
	if (gdibMap == NULL)
	{
		throw;
	}
	goldMap = (HBITMAP)SelectObject(ghdcMainWnd, gdibMap);

	// 创建并绑定帧缓存
	memset(ptr, 0, width * height * 4);
	memset(gInputKeys, 0, sizeof(int) * 512);

	int wx = rect.right - rect.left;
	int wy = rect.bottom - rect.top;
	int sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	int sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;

	SetWindowPos(ghMainWnd, NULL, sx, sy, wx, wy,
		(SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(ghMainWnd);

	ShowWindow(ghMainWnd, SW_SHOW);
	pGfx = new Graphics(width, height, ghMainWnd, ghdcMainWnd, ptr);

	// register mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		throw ;
	}
}

Window::~Window()
{
	
}

Graphics & Window::Gfx()
{
	if (!pGfx)
	{
		throw;
	}
	return *pGfx;
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(ghMainWnd, title.c_str()) == 0)
	{
		throw ;
	}
}

void Window::FreeCursor()
{
	ClipCursor(nullptr);
}

void Window::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableCursor()
{
	cursorEnabled = true;
	ShowCursor();
	FreeCursor();
}

void Window::DisableCursor()
{
	cursorEnabled = false;
	HideCursor();
	ConfineCursor();
}

void Window::ConfineCursor()
{
	RECT rect;
	GetClientRect(ghMainWnd, &rect);
	MapWindowPoints(ghMainWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

bool Window::CursorEnabled() const
{
	return cursorEnabled;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	if (message == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window *>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, message, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	// we don't want the DefProc to handle this message because
	// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	// clear keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_ACTIVATE:
		// confine/free cursor on window to foreground/background if cursor disabled
		if (!cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;
	case WM_INPUT:
	{
		if (!mouse.RawEnabled())
		{
			break;
		}
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}
		rawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			// bail msg processing if error
			break;
		}
		// process the raw input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
	// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	/*********** END KEYBOARD MESSAGES ***********/
	/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!cursorEnabled)
		{
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
			mouse.OnMouseMove(pt.x, pt.y);
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
	}	
	break;
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	//case WM_RBUTTONDOWN:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	mouse.OnRightPressed(pt.x, pt.y);
	//	break;
	//}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	//case WM_RBUTTONUP:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	mouse.OnRightReleased(pt.x, pt.y);
	//	// release mouse if outside of window
	//	if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
	//	{
	//		ReleaseCapture();
	//		mouse.OnMouseLeave();
	//	}
	//	break;
	//}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** END MOUSE MESSAGES **************/

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
