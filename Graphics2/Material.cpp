#include "Material.h"
#include "WICTextureLoader.h"
#include "DirectXFramework.h"

Material* Material::_activeMaterial{ nullptr };

void Material::SetShader(shared_ptr<Shader> source)
{
	if (_shader.get() == source.get())
	{
		return;
	}

	_shader = source;
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

void Material::SetTexture(const int& id, const shared_ptr<Texture>& texture)
{
	if (_textures.find(id) != _textures.end())
	{
		_textures[id] = texture;
	}
	else
	{
		_textures.emplace(id, texture);
	}
}

shared_ptr<Shader> Material::GetShader() const
{
	return _shader;
}

bool Material::Activate()
{
	if (_activeMaterial != this)
	{
		if (!_shader)
		{
			return false;
		}

		_shader->CompileOnce();

		ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

		deviceContext->VSSetShader(_shader->GetVertexShader().Get(), 0, 0);
		deviceContext->PSSetShader(_shader->GetFragmentShader().Get(), 0, 0);
		deviceContext->IASetInputLayout(_shader->GetInputLayout().Get());

		_activeMaterial = this;
	}

	return true;
}

void Material::Update(CBUFFER* cbuf)
{
	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	// Update the constant buffer 
	deviceContext->VSSetConstantBuffers(0, 1, _shader->GetConstantBuffer().GetAddressOf()); // Pass constant buffer to VS (Vertex Shader)
	deviceContext->PSSetConstantBuffers(0, 1, _shader->GetConstantBuffer().GetAddressOf()); // Pass constant buffer to PS (Pixel Shader)

	deviceContext->UpdateSubresource(_shader->GetConstantBuffer().Get(), 0, 0, cbuf, 0, 0);

	// Set the texture to be used by the pixel shader
	for (const auto& texture : _textures)
	{
		deviceContext->PSSetShaderResources(texture.first, 1, texture.second->Get().GetAddressOf());
	}
}

shared_ptr<Texture>& Material::GetTexture(const int& id)
{
	if (!_textures[id] || !_textures[id]->IsLoaded())
	{
		_textures[id] = RESOURCES->GetDefaultTexture();
	}

	return _textures[id];
}
