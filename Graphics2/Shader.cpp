#include "Shader.h"
#include "DirectXFramework.h"


Shader::Shader() : _file(L""), b_isCompiled(false)
{ }

Shader::Shader(const Shader& copy) : _file(copy._file), b_isCompiled(false)
{
	if (copy.b_isCompiled)
	{
		Compile();
	}
}

Shader::Shader(const wstring& file) : _file(file), b_isCompiled(false)
{ }

Shader::~Shader()
{ }

void Shader::SetSource(const wstring& file)
{
	_file = file;
}

const wstring& Shader::GetSource() const
{
	return _file;
}

void Shader::Compile()
{
	DWORD shaderCompileFlags = 0;

#ifdef _DEBUG
	shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (!CompileVertex(shaderCompileFlags) || !CompileFragment(shaderCompileFlags))
	{
		MessageBoxA(0, "Could not compile source shader, refer to log for details.", "Shader error", 0);
		return;
	}
	else
	{
		BuildVertexLayout();
	}

	b_isCompiled = true;
}

void Shader::CompileOnce()
{
	if (!b_isCompiled)
	{
		Compile();
	}
}

ComPtr<ID3D11VertexShader> Shader::GetVertexShader() const
{
	return _vert;
}

ComPtr <ID3D11PixelShader> Shader::GetFragmentShader() const
{
	return _frag;
}

ComPtr <ID3D11InputLayout> Shader::GetInputLayout() const
{
	return _layout;
}

const Shader& Shader::operator=(const Shader& rhs)
{
	_file = rhs._file;
	if (b_isCompiled)
	{
		Compile();
	}

	return *this;
}

shared_ptr<Shader> Shader::Load(const wstring& file)
{
	return make_shared<Shader>(file);
}

bool Shader::CompileVertex(DWORD flags)
{
	ComPtr<ID3DBlob> m = nullptr;

	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();

	//Compile vertex shader
	HRESULT hr = D3DCompileFromFile
	(
		_file.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		flags, 0,
		_vertBytes.GetAddressOf(),
		m.GetAddressOf()
	);

	if (m.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)m->GetBufferPointer(), 0, 0);
	}

	// Even if there are no compiler messages, check to make sure there were no other errors.
	ThrowIfFailed(hr);
	ThrowIfFailed(device->CreateVertexShader(_vertBytes->GetBufferPointer(), _vertBytes->GetBufferSize(), NULL, _vert.GetAddressOf()));

	return true;
}

bool Shader::CompileFragment(DWORD flags)
{
	ComPtr<ID3DBlob> m = nullptr;

	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();

	// Compile fragment shader
	HRESULT hr = D3DCompileFromFile
	(
		_file.c_str(),
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		flags, 0,
		_fragBytes.GetAddressOf(),
		m.GetAddressOf()
	);

	if (m.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)m->GetBufferPointer(), 0, 0);
	}

	ThrowIfFailed(hr);
	ThrowIfFailed(device->CreatePixelShader(_fragBytes->GetBufferPointer(), _fragBytes->GetBufferSize(), NULL, _frag.GetAddressOf()));

	return true;
}

void Shader::BuildVertexLayout()
{
	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();

	// Create the vertex input layout. This tells DirectX the format
	// of each of the vertices we are sending to it.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertBytes->GetBufferPointer(), _vertBytes->GetBufferSize(), _layout.GetAddressOf()));
}
