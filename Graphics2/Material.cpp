#include "Material.h"
#include "WICTextureLoader.h"
#include "DirectXFramework.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "CameraNode.h"

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
		if (_activeMaterial != nullptr)
		{
			_activeMaterial->Reset();
		}

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

void Material::Reset()
{
	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();
	const auto& defaultTexture = RESOURCES->GetDefaultTexture();

	// Clear any assigned texture in shared resources by replacing it with default texture
	for (const auto& texture : _textures)
	{
		deviceContext->PSSetShaderResources(texture.first, 1, defaultTexture->Get().GetAddressOf());
	}
}

void Material::UpdateConstantBuffers(const MeshObjectData& meshData)
{
	ComPtr<ID3D11DeviceContext> deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	// Get lights
	shared_ptr<DirectionalLight> directional = FRAMEWORK->GetLight<DirectionalLight>();
	shared_ptr<AmbientLight> ambient = FRAMEWORK->GetLight<AmbientLight>();

	// Camera information
	DirectXFramework* framework = DirectXFramework::GetDXFramework();
	const CameraNode* mainCamera = CameraNode::GetMain();

	ConstantBuffer* baseData = GetConstantBuffer()->GetLayoutPointer<ConstantBuffer>(0);

	baseData->CompleteTransformation	= meshData.completeTransformation;
	baseData->WorldTransformation		= meshData.worldTransformation;
	baseData->CameraPosition			= mainCamera->GetWorldPosition().ToDX();
	baseData->LightVector				= XMVector4Normalize(directional->GetDirection());
	baseData->LightColor				= directional->GetColor();
	baseData->AmbientColor				= ambient->GetColor();
	baseData->DiffuseCoefficient		= GetAlbedo();
	baseData->SpecularCoefficient		= GetSpecularColor();
	baseData->Shininess					= GetShininess();

	for (int cb = 0; cb < _constantBuffer->Size(); ++cb)
	{
		// Update the constant buffer 
		deviceContext->VSSetConstantBuffers(cb, 1, _constantBuffer->GetConstantBuffer(cb).GetAddressOf()); // Pass constant buffer to VS (Vertex Shader)
		deviceContext->PSSetConstantBuffers(cb, 1, _constantBuffer->GetConstantBuffer(cb).GetAddressOf()); // Pass constant buffer to PS (Pixel Shader)

		deviceContext->UpdateSubresource(_constantBuffer->GetConstantBuffer(cb).Get(), 0, 0, _constantBuffer->GetLayoutPointer<void>(cb), 0, 0);
	}

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

shared_ptr<CBO>& Material::GetConstantBuffer()
{
	if (_constantBuffer == nullptr)
	{
		_constantBuffer = make_shared<CBO>();
	}

	return _constantBuffer;
}
