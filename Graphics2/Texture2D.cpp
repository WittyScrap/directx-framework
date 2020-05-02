#include "Texture2D.h"
#include <fstream>

Texture2D::Texture2D(const int& width, const int& height)
	: _width{ static_cast<UINT>(width) }, _height{ static_cast<UINT>(height) } 
{ }

Texture2D::Texture2D(const int& width, const int& height, const wstring& filePath)
	: _width{ static_cast<UINT>(width) }, _height{ static_cast<UINT>(height) }
{
	Load(filePath);
}

bool Texture2D::Load(const wstring& filePath)
{
	_textureData.clear();

	unsigned int mapSize = _width * _height;
	USHORT* rawFileValues = new USHORT[mapSize];

	std::ifstream inputMap;
	inputMap.open(filePath.c_str(), std::ios_base::binary);

	if (!inputMap)
	{
		return false;
	}

	inputMap.read((char*)rawFileValues, (long long)mapSize * 2);
	inputMap.close();

	// Normalise BYTE values to the range 0.0f - 1.0f;
	for (unsigned int i = 0; i < mapSize; i++)
	{
		_textureData.push_back((float)rawFileValues[i] / 65536);
	}

	delete[] rawFileValues;
	return true;
}

void Texture2D::Clear()
{
	_textureData.clear();
}

void Texture2D::SetPixel(const size_t x, const size_t y, FLOAT value)
{
	_textureData[x * _width + y] = value;
}

const FLOAT& Texture2D::GetPixel(const size_t x, const size_t y) const
{
	return _textureData[x * _width + y];
}

const UINT& Texture2D::GetWidth() const
{
	return _width;
}

const UINT& Texture2D::GetHeight() const
{
	return _height;
}
