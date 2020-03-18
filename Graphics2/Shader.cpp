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

ID3D11VertexShader* const Shader::GetVertexShader() const
{
	return _vert.Get();
}

ID3D11PixelShader* const Shader::GetFragmentShader() const
{
	return _frag.Get();
}

ID3D11InputLayout* const Shader::GetInputLayout() const
{
	return _layout.Get();
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

const Shader Shader::Load(const wstring& file)
{
	return Shader(file);
}

bool Shader::CompileVertex(DWORD flags)
{
	ComPtr<ID3DBlob> m = nullptr;

	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();

	HRESULT result = D3DCompileFromFile (
		_file.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		flags, 0,
		_vertBytes.GetAddressOf(),
		m.GetAddressOf()
	);

	if (m.Get() != nullptr)
	{
		// If there were any compilation messages, display them
		MessageBoxA(0, (char*)m->GetBufferPointer(), 0, 0);

		return false;
	}

	ThrowIfFailed(result);
	ThrowIfFailed(
		device->CreateVertexShader(
			_vertBytes->GetBufferPointer(),
			_vertBytes->GetBufferSize(),
			NULL,
			_vert.GetAddressOf()
		)
	);

	return true;
}

bool Shader::CompileFragment(DWORD flags)
{
	ComPtr<ID3DBlob> m = nullptr;

	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();
	const auto& device_context = DirectXFramework::GetDXFramework()->GetDeviceContext();

	HRESULT result = D3DCompileFromFile (
		_file.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		flags, 0,
		_fragBytes.GetAddressOf(),
		m.GetAddressOf()
	);

	if (m.Get() != nullptr)
	{
		// TODO: log error...
		return false;
	}

	ThrowIfFailed(result);
	ThrowIfFailed(
		device->CreatePixelShader(
			_fragBytes->GetBufferPointer(),
			_fragBytes->GetBufferSize(),
			NULL,
			_frag.GetAddressOf()
		)
	);

	return true;
}

void Shader::BuildVertexLayout()
{
	const auto& device = DirectXFramework::GetDXFramework()->GetDevice();

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), _vertBytes->GetBufferPointer(), _vertBytes->GetBufferSize(), _layout.GetAddressOf()));
}
