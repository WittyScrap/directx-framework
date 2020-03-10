#include "Material.h"

Material::Material()
{ }

Material::Material(const Material& copy) : _shader(copy._shader)
{ }

Material::Material(const Shader& shader) : _shader(shader)
{ }

Material::~Material()
{ }

void Material::SetShader(const Shader& shader)
{
	_shader = shader;
}

void Material::SetInt(const wstring& propertyName, const int& value)
{
	// TODO: Properties...
}

void Material::SetFloat(const wstring& propertyName, const float& value)
{
	// TODO: Properties...
}

void Material::SetTexture(const wstring& propertyName, const int& value)
{
	// TODO: Properties...
}

void Material::Activate()
{
	const auto& deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

	_shader.CompileOnce();

	deviceContext->VSSetShader(_shader.GetVertexShader(), 0, 0);
	deviceContext->PSSetShader(_shader.GetFragmentShader(), 0, 0);
}