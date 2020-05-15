#pragma once
#include "SceneNode.h"
#include <list>

using namespace std;
typedef SceneNode	Super;

class SceneGraph : public SceneNode
{
public:
	SceneGraph() : SceneNode(L"Root") {};
	SceneGraph(wstring name) : SceneNode(name) {};
	~SceneGraph(void) {};

	virtual bool Initialise(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Shutdown(void);

	void Add(SceneNodePointer node);
	void Remove(SceneNodePointer node);
	SceneNodePointer Find(wstring name);

	template<typename TNode>
	static shared_ptr<TNode> Create(wstring name);

	template<typename TNode>
	static shared_ptr<TNode> Create();

private:
	list<SceneNodePointer> _children;

};

typedef shared_ptr<SceneGraph>			 SceneGraphPointer;

template<typename TNode>
inline shared_ptr<TNode> SceneGraph::Create(wstring name)
{
	return make_shared<TNode>(name);
}

template<typename TNode>
inline shared_ptr<TNode> SceneGraph::Create()
{
	return make_shared<TNode>();
}
