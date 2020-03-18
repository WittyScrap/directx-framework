#include "SceneGraph.h"

bool SceneGraph::Initialise(void)
{
	for (SceneNodePointer& child : _children)
	{
		if (!child->Initialise())
		{
			return false;
		}
	}

	return true;
}

void SceneGraph::Update(FXMMATRIX& currentWorldTransformation)
{
	Super::Update(currentWorldTransformation);

	for (SceneNodePointer& child : _children)
	{
		child->Update(currentWorldTransformation);
	}
}

void SceneGraph::Render()
{
	for (SceneNodePointer& child : _children)
	{
		child->Render();
	}
}

void SceneGraph::Shutdown()
{
	for (SceneNodePointer& child : _children)
	{
		child->Shutdown();
	}
}

void SceneGraph::Add(SceneNodePointer node)
{
	_children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node)
{
	std::list<SceneNodePointer>::iterator child = std::find(_children.begin(), _children.end(), node);

	if (child != _children.end())
	{
		_children.erase(child);
	}
	else
	{
		for (SceneNodePointer& child : _children)
		{
			child->Remove(node);
		}
	}
}

SceneNodePointer SceneGraph::Find(wstring name)
{
	auto& found = Super::Find(name);

	if (found != nullptr)
	{
		return found;
	}

	for (SceneNodePointer& child : _children)
	{
		auto& found = child->Find(name);

		if (found != nullptr)
		{
			return found;
		}
	}

	return nullptr;
}