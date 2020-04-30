#include "SceneNode.h"
#include <WinUser.h>
#include "DirectXFramework.h"

const int SceneNode::GetKey(const int& keyCode)
{
	return GetAsyncKeyState(keyCode) < 0;
}

const int SceneNode::GetKeyDown(const int& keyCode)
{
	SHORT key = GetAsyncKeyState(keyCode);
	return (key & 1) && key < 0;
}

const float SceneNode::GetMouseHorizontal()
{
	return _mouseHorizontalAxis;
}

const float SceneNode::GetMouseVertical()
{
	return _mouseVerticalAxis;
}

void SceneNode::ResetMouse()
{
	if (_mouseLocked)
	{
		if (GetKey(VK_ESCAPE))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}

		POINT mouseLocation;
		RECT windowRect;

		if (!GetCursorPos(&mouseLocation) || !GetWindowRect(FRAMEWORK->GetHWnd(), &windowRect))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}

		// Convert mouse location from screen space
		// to window space.
		mouseLocation.x -= windowRect.left;
		mouseLocation.y -= windowRect.top;

		// Normalize mouse location.
		float windowWidth = (float)windowRect.right - (float)windowRect.left;
		float windowHeight = (float)windowRect.bottom - (float)windowRect.top;

		float normalMouseX = (float)mouseLocation.x / windowWidth;
		float normalMouseY = (float)mouseLocation.y / windowHeight;

		// Convert mouse location from 0 - 1 range to -1 - +1 range
		float clipMouseX = normalMouseX * 2 - 1; // These values now represent the
		float clipMouseY = normalMouseY * 2 - 1; // horizontal and vertical mouse axis.

		_mouseHorizontalAxis = clipMouseX;
		_mouseVerticalAxis = clipMouseY;

		if (!SetCursorPos((windowRect.right + windowRect.left) / 2, (windowRect.top + windowRect.bottom) / 2))
		{
			SetMouseVisible(true);
			_mouseLocked = false;
			return;
		}
	}
	else
	{
		_mouseHorizontalAxis = 0;
		_mouseVerticalAxis = 0;
	}
}
