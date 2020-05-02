#pragma once
#include "DirectXCore.h"

/**
 * Basic structure to hold constant buffer
 * data to be transported to the shader program.
 *
 */
struct ConstantBuffer
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
    inline  ComPtr<ID3D11Buffer>        GetConstantBuffer() const           { return _constantBuffer; }
    
    template<typename TBufferType>
    inline  TBufferType*                GetLayoutPointer()                  { return (TBufferType*)(_bufferData.get()); }
    inline  ConstantBuffer*             GetLayoutPointer()                  { return (ConstantBuffer*)(_bufferData.get()); }

    template<typename TBufferType>
    inline  void                        CreateBufferData()                  { _bufferData = (shared_ptr<ConstantBuffer>)(make_shared<TBufferType>()); _bufferSize = sizeof(TBufferType); ApplyBufferData(); }
    
protected:
    inline  void                        ApplyBufferData();

private:
    shared_ptr<ConstantBuffer>          _bufferData;
    ComPtr<ID3D11Buffer>                _constantBuffer;

    UINT                                _bufferSize;
};

inline void ConstantBufferObject::ApplyBufferData()
{
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = _bufferSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    ThrowIfFailed(DirectXFramework::GetDXFramework()->GetDevice()->CreateBuffer(&bufferDesc, NULL, _constantBuffer.GetAddressOf()));
}

// Shorthand for class name
typedef ConstantBufferObject CBO;