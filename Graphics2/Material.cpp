#include "Material.h"
#include "WICTextureLoader.h"
#include "DirectXFramework.h"

void Material::SetShader(Shader& source)
{
	if (!_shader)
	{
		_shader = make_shared<Shader>();
	}
	else
	{
		_shader.reset<Shader>(&source);
	}

	_shader->CompileOnce();
}

void Material::SetShader(const wstring& source)
{
	if (_shader)
	{
		_shader->SetSource(source);
	}
	else
	{
		_shader = make_shared<Shader>(source);
	}

	_shader->CompileOnce();
}

void Material::SetTexture(const wstring& textureName)
{
	if (textureName != _textureName)
	{
		ThrowIfFailed(
			CreateWICTextureFromFile(
				DirectXFramework::GetDXFramework()->GetDevice().Get(),
				DirectXFramework::GetDXFramework()->GetDeviceContext().Get(),
				textureName.c_str(),
				nullptr,
				_texture.GetAddressOf()
			)
		);

		_textureName = textureName;
	}
}

const Shader& Material::GetShader() const
{
	return *_shader;
}

bool Material::Activate()
{
	if (!_shader)
	{
		return false;
	}

	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	deviceContext->VSSetShader(_shader->GetVertexShader().Get(), 0, 0);
	deviceContext->PSSetShader(_shader->GetFragmentShader().Get(), 0, 0);
	deviceContext->IASetInputLayout(_shader->GetInputLayout().Get());

	return true;
}

void Material::Update(CBUFFER* cbuf)
{
	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	// Update the constant buffer 
	deviceContext->VSSetConstantBuffers(0, 1, _shader->GetConstantBuffer().GetAddressOf());
	deviceContext->UpdateSubresource(_shader->GetConstantBuffer().Get(), 0, 0, cbuf, 0, 0);

	// Set the texture to be used by the pixel shader
	deviceContext->PSSetShaderResources(0, 1, _texture.GetAddressOf());
}

ComPtr<ID3D11ShaderResourceView> Material::GetTexture() const
{
	return _texture;
}
