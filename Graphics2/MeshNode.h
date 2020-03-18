#pragma once
#include "SceneNode.h"
#include "DirectXCore.h"
#include "Shader.h"
#include <vector>

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
            void BuildTexture();

    static  ComPtr<ID3D11Device>                GetDevice();
    static  ComPtr<ID3D11DeviceContext>         GetDeviceContext();

    virtual vector<Vertex>                      MeshVertices()          = 0;
    virtual vector<UINT>                        MeshIndices()           = 0;

private:
            ComPtr<ID3D11Buffer>                _vertexBuffer;
            ComPtr<ID3D11Buffer>                _indexBuffer;

            shared_ptr<Shader>                  _shader = nullptr;

            ComPtr<ID3D11ShaderResourceView>    _texture;
            wstring                             _textureName;

            UINT                                _indicesCount;
};

