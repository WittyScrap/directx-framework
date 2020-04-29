#pragma once
#include "DirectXCore.h"
#include "DirectXFramework.h"
#include <vector>


/**
 *  Represents a 2D texture of any size.
 */
class Texture2D
{
public:

                            Texture2D(const int& width, const int& height);
                            Texture2D(const int& width, const int& height, const wstring& filePath);

    virtual                ~Texture2D() { }

    virtual bool            Load(const wstring& filePath);
    virtual void            Clear();

            void            SetPixel(const size_t x, const size_t y, FLOAT value);
            const FLOAT&    GetPixel(const size_t x, const size_t y) const;

            const UINT&     GetWidth() const;
            const UINT&     GetHeight() const;


private:
            const UINT      _width;
            const UINT      _height;

            vector<FLOAT>   _textureData;
};

