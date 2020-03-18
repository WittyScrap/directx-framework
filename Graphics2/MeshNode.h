#pragma once
#include "SceneNode.h"
#include "DirectXCore.h"
#include <vector>

/**
 * Simple structure to represent Vertex data.
 *
 */
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TextureCoordinate;
};

/**
 * Simple structure to hold constant buffer
 * data to be transported to the shader program.
 *
 */
struct CBUFFER
{
	XMMATRIX    CompleteTransformation;
	XMMATRIX	WorldTransformation;
	XMVECTOR    LightVector;
	XMFLOAT4    LightColour;
	XMFLOAT4    AmbientColour;
};

/**
 * Represents a node that can retain any type
 * of geometry.
 *
 */
class MeshNode : public SceneNode
{
public:
    /* --- Constructors --- */
                 MeshNode()                     : SceneNode(L"Mesh Node") {}
                 MeshNode(const wstring& name)  : SceneNode(name)         {}

    virtual      ~MeshNode()                                              {}

    virtual bool Initialise()                                   override;
    virtual void Update(FXMMATRIX& currentWorldTransformation)  override;
    virtual void Render()                                       override;
    virtual void Shutdown()                                     override;

            void SetShader(const wstring& fileName);
            void SetTexture(const wstring& textureName);

protected:
            void BuildBuffers();
            void BuildShader();
            void BuildVertexLayout();
            void BuildConstantBuffer();
            void BuildTexture();

    static  ComPtr<ID3D11Device>                GetDevice();
    static  ComPtr<ID3D11DeviceContext>         GetDeviceContext();

    virtual vector<Vertex>                      MeshVertices()          = 0;
    virtual vector<UINT>                        MeshIndices()           = 0;

private:
            ComPtr<ID3D11Buffer>                _vertexBuffer;
            ComPtr<ID3D11Buffer>                _indexBuffer;

            ComPtr<ID3DBlob>                    _vertBytes = nullptr;
            ComPtr<ID3DBlob>                    _fragBytes = nullptr;

            ComPtr<ID3D11VertexShader>          _vert;
            ComPtr<ID3D11PixelShader>           _frag;
            ComPtr<ID3D11InputLayout>           _layout;
            ComPtr<ID3D11Buffer>                _constantBuffer;

            ComPtr<ID3D11ShaderResourceView>    _texture;

            wstring                             _shaderName;
            wstring                             _textureName;

            UINT                                _indicesCount;
};

