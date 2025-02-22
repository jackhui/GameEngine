#pragma once
#include <windows.h>
#include <sstream>

#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_A 0x41
#define VK_D 0x44
#define VK_W 0x53
#define VK_S 0x57

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Start(HINSTANCE hInst);
	static void SetCursorPosition(long x, long y);

	static void MouseEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void KeyboardEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	static long cursor_pos[2];
	static bool rotationMode;
};

