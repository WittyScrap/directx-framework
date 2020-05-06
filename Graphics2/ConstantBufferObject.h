#pragma once
#include "DirectXCore.h"
#define CBUFFER __declspec(align(16)) struct

/**
 * Basic structure to hold constant buffer
 * data to be transported to the shader program.
 *
 */
CBUFFER ConstantBuffer
{
    XMMATRIX    CompleteTransformation;
    XMMATRIX	WorldTransformation;
    XMFLOAT4	CameraPosition;
    XMVECTOR    LightVector;
    XMFLOAT4    LightColor;
    XMFLOAT4    AmbientColor;
    XMFLOAT4    DiffuseCoefficient;
    XMFLOAT4	SpecularCoefficient;
    float		Shininess;
    float		Opacity;
    float       Padding[2];
};


/**
 * Wrapper class for describing a generic constant buffer.
 *
 */
class ConstantBufferObject
{
public:
                                        ConstantBufferObject();
    virtual                            ~ConstantBufferObject();

    template<typename TBufferType>
    inline  int                         CreateBuffer();

    template<typename TBufferType>
    inline  TBufferType*                GetLayoutPointer(const int& id);
    inline  ComPtr<ID3D11Buffer>        GetConstantBuffer(const int& id);
    
    inline  size_t                      Size() { return _bufferData.size(); }

protected:
    inline  void                        ApplyBufferData(const int& id);

private:
    vector<void*>                       _bufferData;
    vector<ComPtr<ID3D11Buffer>>        _constantBuffers;
    vector<UINT>                        _bufferSize;
};

inline void ConstantBufferObject::ApplyBufferData(const int& id)
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = _bufferSize[id];
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    ThrowIfFailed(DirectXFramework::GetDXFramework()->GetDevice()->CreateBuffer(&bufferDesc, NULL, _constantBuffers[id].GetAddressOf()));
}

// Shorthand for class name
typedef ConstantBufferObject CBO;

template<typename TBufferType>
inline int ConstantBufferObject::CreateBuffer()
{
    int bufferIndex = static_cast<int>(_bufferData.size());
    _bufferData.push_back(new TBufferType());
    _bufferSize.push_back(sizeof(TBufferType));
    _constantBuffers.push_back(ComPtr<ID3D11Buffer>());

    ApplyBufferData(bufferIndex);

    return bufferIndex;
}

template<typename TBufferType>
inline TBufferType* ConstantBufferObject::GetLayoutPointer(const int& id)
{
    return (TBufferType*)_bufferData[id];
}

inline ConstantBufferObject::ConstantBufferObject()
{
    CreateBuffer<ConstantBuffer>(); // Allocate default buffer
}

inline ConstantBufferObject::~ConstantBufferObject()
{
    // Deallocate all buffer layout objects and clear vectors
    for (size_t i = 0; i < _bufferData.size(); ++i)
    {
        delete _bufferData[i];
    }

    _bufferData.clear();
    _constantBuffers.clear();
    _bufferSize.clear();
}

inline ComPtr<ID3D11Buffer> ConstantBufferObject::GetConstantBuffer(const int& id)
{
    return _constantBuffers[id];
}
