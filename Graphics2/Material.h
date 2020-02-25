#pragma once
#include "Core.h"
#include "Shader.h"

/**
  Represents a generic material that can hold a shader,
  compile it, and apply it to a mesh for rendering.

 */
class Material
{
public:
    /* Constructors */
    Material();
    Material(const Material& copy);
    Material(const Shader& shader);

    virtual ~Material();

    void SetShader(const Shader& shader);

    void SetInt(const wstring& propertyName, const int& value);
    void SetFloat(const wstring& propertyName, const float& value);
    void SetTexture(const wstring& propertyName, const int& value);

    void Activate();

private:
    Shader _shader;
};

