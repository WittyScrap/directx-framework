#pragma once
#include "Core.h"

/**
  Represents a shader object that can hold a shader's
  source code, compile it, and apply it to a material
  for rendering.
 */
class Shader
{
public:
    /* Constructors */
    Shader();
    Shader(const Shader& copy);
    Shader(const wstring& fragment, const wstring& vertex);

    virtual ~Shader();

    void Compile();

    const wstring& GetVertexShader() const;
    const wstring& GetFragmentShader() const;

    void SetVertexShader(const wstring& sourceFile);
    void SetFragmentShader(const wstring& sourceFile);

private:
    wstring _vert;
    wstring _frag;
};

