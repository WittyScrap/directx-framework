#pragma once
#include <vector>
#include "Framework.h"
#include "DirectXCore.h"
#include "WICTextureLoader.h"

struct Vertex;

class Graphics2 : public Framework
{
public:
	Graphics2();

	bool Initialise();
	void Update();
	void Render();
	void Shutdown();
	void OnResize(WPARAM wParam);

private:
	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11DeviceContext>		_deviceContext;
	ComPtr<IDXGISwapChain>			_swapChain;
	ComPtr<ID3D11Texture2D>			_depthStencilBuffer;
	ComPtr<ID3D11RenderTargetView>	_renderTargetView;
	ComPtr<ID3D11DepthStencilView>	_depthStencilView;

	ComPtr<ID3D11Buffer>			_vertexBuffer;
	ComPtr<ID3D11Buffer>			_indexBuffer;

	ComPtr<ID3DBlob>				_vertexShaderByteCode = nullptr;
	ComPtr<ID3DBlob>				_pixelShaderByteCode = nullptr;
	ComPtr<ID3D11VertexShader>		_vertexShader;
	ComPtr<ID3D11PixelShader>		_pixelShader;
	ComPtr<ID3D11InputLayout>		_layout;
	ComPtr<ID3D11Buffer>			_constantBuffer;

	ComPtr<ID3D11ShaderResourceView> _texture;
	ComPtr<ID3D11ShaderResourceView> _texture2;


	D3D11_VIEWPORT					_screenViewport;

	// Our vectors and matrices.  Note that we cannot use
	// XMVECTOR and XMMATRIX for class variables since they need
	// to be aligned on 16-byte boundaries and the compiler cannot
	// guarantee this for class variables

	XMFLOAT4						_eyePosition;
	XMFLOAT4						_focalPointPosition;
	XMFLOAT4						_upVector;

	XMFLOAT4X4						_worldTransformation1;
	XMFLOAT4X4						_worldTransformation2;
	XMFLOAT4X4						_viewTransformation;
	XMFLOAT4X4						_projectionTransformation;

	float							_rotationAngle;

	bool GetDeviceAndSwapChain();
	void CreateShape();
	void BuildGeometryBuffers(Vertex vertices[], UINT indices[], ComPtr<ID3D11Buffer> vertexTarget, ComPtr<ID3D11Buffer> indexTarget);
	void BuildShaders();
	void BuildVertexLayout();
	void BuildConstantBuffer();
	void BuildTexture();
};

