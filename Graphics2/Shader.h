#pragma once
#include "Core.h"
#include "DirectXCore.h"

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
    Shader(const wstring& file);

    virtual ~Shader();

    void SetSource(const wstring& file);
    void Compile();
    void CompileOnce();

    ID3D11VertexShader* const GetVertexShader() const;
    ID3D11PixelShader* const GetFragmentShader() const;
    ID3D11InputLayout* const GetInputLayout() const;

    const Shader& operator=(const Shader& rhs);
    static const Shader Load(const wstring& file);

protected:
    bool CompileVertex(DWORD flags);
    bool CompileFragment(DWORD flags);

    void BuildVertexLayout();

private:
    bool b_isCompiled;
    wstring _file;

    ComPtr<ID3DBlob> _vertBytes;
    ComPtr<ID3DBlob> _fragBytes;

    ComPtr<ID3D11VertexShader> _vert;
    ComPtr<ID3D11PixelShader> _frag;
    ComPtr<ID3D11InputLayout> _layout;
};

typedef shared_ptr<Shader> ShaderPtr;