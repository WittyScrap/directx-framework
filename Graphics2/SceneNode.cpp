#include "SceneNode.h"

const int SceneNode::GetKey(const int& keyCode)
{
	return GetAsyncKeyState(keyCode) < 0;
}

const int SceneNode::GetKeyDown(const int& keyCode)
{
	SHORT key = GetAsyncKeyState(keyCode);
	return (key & 1) && key < 0;
}
