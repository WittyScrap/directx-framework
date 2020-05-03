#pragma once
#include "WICTextureLoader.h"
#include "DirectXFramework.h"


class Texture
{
public:
														Texture() : _textureObject{ nullptr }, _name{ L"" }		{ _loaded = false; }
														Texture(const wstring& source) : _name{ source }		{ Load(_name); }
	virtual											   ~Texture()												{}

	inline  void										Load(const wstring& file);
	inline	const ComPtr<ID3D11ShaderResourceView>&		Get() const												{ return _textureObject; }

	inline  bool										IsLoaded()												{ return _loaded; }
	inline  const wstring&								GetName()												{ return _name; }

private:
	ComPtr<ID3D11ShaderResourceView>			_textureObject = nullptr;
	wstring										_name;
	bool										_loaded;
};


void Texture::Load(const wstring& file)
{
	if (file == L"")
	{
		return;
	}

	if (FAILED(CreateWICTextureFromFile(
		DirectXFramework::GetDXFramework()->GetDevice().Get(),
		DirectXFramework::GetDXFramework()->GetDeviceContext().Get(),
		file.c_str(),
		nullptr,
		_textureObject.GetAddressOf()
	)))
	{
		ErrorDialog(L"Invalid texture", L"Could not load texture: " + file, std::exception("Invalid texture file"));

		_loaded = false;
		_textureObject = nullptr;
	}
	else
	{
		_loaded = true;
	}
}