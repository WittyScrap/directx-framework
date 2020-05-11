#pragma once
#include <vector>
#include "Framework.h"
#include "DirectXCore.h"
#include "SceneGraph.h"
#include "ResourceManager.h"
#include "ILight.h"
#include <chrono>

using namespace std::chrono;


class DirectXFramework : public Framework
{
public:
	DirectXFramework();
	DirectXFramework(unsigned int width, unsigned int height);

	virtual void CreateSceneGraph();
	virtual void UpdateSceneGraph();

	bool Initialise();
	void Update();
	void Render();
	void OnResize(WPARAM wParam);
	void Shutdown();

	static DirectXFramework*			GetDXFramework();

	inline SceneGraphPointer			GetSceneGraph() { return _sceneGraph; }
	inline ComPtr<ID3D11Device>			GetDevice() { return _device; }
	inline ComPtr<ID3D11DeviceContext>	GetDeviceContext() { return _deviceContext; }
	inline shared_ptr<ResourceManager>	GetResourceManager() { return _resourceManager; }

	void								EnableDepthTesting();
	void								DisableDepthTesting();
	void								DisableDepthBuffer();

	inline const float&					GetDeltaTime() { return _deltaTime; }

	void								SetBackgroundColour(XMFLOAT4 backgroundColour);

										template<typename _TLight>
	inline shared_ptr<_TLight>			AddLight() { shared_ptr<_TLight> l = make_shared<_TLight>(); _sceneLights.push_back(l); return l; }

										template<typename _TLight>
	inline shared_ptr<_TLight>			GetLight() { 
											const auto& it = find_if(_sceneLights.begin(), _sceneLights.end(), [](const shared_ptr<ILight>& l) { return dynamic_cast<_TLight*>(l.get()); });

											if (it != _sceneLights.end())
											{
												return dynamic_pointer_cast<_TLight>(*it);
											}

											return nullptr;
										}

private:
	ComPtr<ID3D11Device>				_device;
	ComPtr<ID3D11DeviceContext>			_deviceContext;
	ComPtr<IDXGISwapChain>				_swapChain;
	ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView>		_renderTargetView;
	ComPtr<ID3D11DepthStencilView>		_depthStencilView;
	ComPtr<ID3D11DepthStencilState>		_depthStencilActiveState;
	ComPtr<ID3D11DepthStencilState>		_depthStencilInactiveState;
	ComPtr<ID3D11DepthStencilState>		_depthStencilDisabledState;

	D3D11_VIEWPORT						_screenViewport;

	// Our vectors and matrices.  Note that we cannot use
	// XMVECTOR and XMMATRIX for class variables since they need
	// to be aligned on 16-byte boundaries and the compiler cannot
	// guarantee this for class variables

	SceneGraphPointer					_sceneGraph;
	shared_ptr<ResourceManager>			_resourceManager;
	vector<shared_ptr<ILight>>			_sceneLights;

	float							    _backgroundColour[4];
	float								_deltaTime{ 0 };
	time_point<steady_clock>			_previousFrameTime{ high_resolution_clock::now() };

	bool GetDeviceAndSwapChain();
};

#define FRAMEWORK		DirectXFramework::GetDXFramework()
#define DEVICE			FRAMEWORK->GetDevice()
#define DEVICE_CONTEXT	FRAMEWORK->GetDeviceContext()
#define RESOURCES		FRAMEWORK->GetResourceManager()
#define SCENE			FRAMEWORK->GetSceneGraph()
#define ISNULL(x)		(x == nullptr)