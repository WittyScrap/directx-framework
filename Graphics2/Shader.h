#pragma once
#include "Core.h"
#include "DirectXCore.h"

/**
 * Simple structure to represent Vertex data.
 *
 */
struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Normal;
    XMFLOAT2 TextureCoordinate;
};

/**
 * Represents a shader object that can hold a shader's
 * source code, compile it, and apply it to a material
 * for rendering.
 *
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
    const wstring& GetSource() const;

    void Compile();
    void CompileOnce();

    ComPtr<ID3D11VertexShader> GetVertexShader() const;
    ComPtr<ID3D11PixelShader>  GetFragmentShader() const;
    ComPtr<ID3D11InputLayout>  GetInputLayout() const;

    const Shader& operator=(const Shader& rhs);
    static shared_ptr<Shader> Load(const wstring& file);

protected:
    bool CompileVertex(DWORD flags);
    bool CompileFragment(DWORD flags);

    void BuildVertexLayout();

private:
    bool b_isCompiled;
    wstring _file;

    ComPtr<ID3DBlob> _vertBytes = nullptr;
    ComPtr<ID3DBlob> _fragBytes = nullptr;

	ComPtr<ID3D11VertexShader>          _vert;
	ComPtr<ID3D11PixelShader>           _frag;
	ComPtr<ID3D11InputLayout>           _layout;
};