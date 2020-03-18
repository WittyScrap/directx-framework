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

void Material::Activate()
{
	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	deviceContext->VSSetShader(_shader->GetVertexShader().Get(), 0, 0);
	deviceContext->PSSetShader(_shader->GetFragmentShader().Get(), 0, 0);
	deviceContext->IASetInputLayout(_shader->GetInputLayout().Get());
}

ComPtr<ID3D11ShaderResourceView> Material::GetTexture() const
{
	return _texture;
}
