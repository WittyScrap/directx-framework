#pragma once
#include "Shader.h"
#include "Texture.h"
#include "ConstantBufferObject.h"
#include <map>

#define IDLIST(t) map<int, t>

#define RENDER_PASS_OPAQUE      0
#define RENDER_PASS_TRANSPARENT 1

struct MeshObjectData
{
    XMMATRIX completeTransformation;
    XMMATRIX worldTransformation;
};

enum class Blend
{
    Zero                = D3D11_BLEND_ZERO,
    One                 = D3D11_BLEND_ONE,
    SrcAlpha            = D3D11_BLEND_SRC_ALPHA,
    DestAlpha           = D3D11_BLEND_DEST_ALPHA,
    OneMinusSrcAlpha    = D3D11_BLEND_INV_SRC_ALPHA,
    OneMinusDestAlpha   = D3D11_BLEND_INV_DEST_ALPHA
};

enum class Operation
{
    Add                 = D3D11_BLEND_OP_ADD,
    Subtract            = D3D11_BLEND_OP_SUBTRACT,
    Min                 = D3D11_BLEND_OP_MIN,
    Max                 = D3D11_BLEND_OP_MAX,
    InverseSubtract     = D3D11_BLEND_OP_REV_SUBTRACT
};

/**
 * The Material class will hold and manage a reference to a Shader,
 * as well as any Texture or property that the shader may use.
 *
 */
class Material
{
public:
                                            Material(wstring name, shared_ptr<Shader> source) :
                                                _name{ name },
                                                _shader(source),
                                                _albedo{ 1, 1, 1, 1 },
                                                _specular{ 1, 1, 1, 1 },
                                                _shininess{ 0 },
                                                _opacity{ 1 },
                                                _blendEnabled{ false },
                                                _sourceBlend{ Blend::Zero },
                                                _destinationBlend{ Blend::Zero },
                                                _blendOperation{ Operation::Add }                                  { if (_shader) _shader->CompileOnce(); }

                                            Material() : Material(L"Material", nullptr)                            {}
                                            Material(wstring name) : Material(name, nullptr)                       {}
                                            Material(shared_ptr<Shader> source) : Material(L"Material", source)    {}
                                            Material(const Material& m) : Material(m._name, m._shader)             {}

    virtual                                ~Material()                                                             {}

            void                            SetShader(shared_ptr<Shader> source);
	        void                            SetShader(const wstring& source);
            shared_ptr<Shader>              GetShader() const;


            bool                            Activate();
            void                            Reset();

            void                            UpdateConstantBuffers(const MeshObjectData& cbuf);

            void                            SetTexture(const int& id, const shared_ptr<Texture>& textureName);
            shared_ptr<Texture>&            GetTexture(const int& id);

            shared_ptr<CBO>&                GetConstantBuffer();

            void                            SetAlbedo(const XMFLOAT4& albedo)           { _albedo = albedo; }
            const XMFLOAT4&                 GetAlbedo() const                           { return _albedo;}

            void                            SetSpecularColor(const XMFLOAT4& specular)  { _specular = specular; }
            const XMFLOAT4&                 GetSpecularColor() const                    { return _specular; }

            void                            SetShininess(const FLOAT& shininess)        { _shininess = shininess; }
            const FLOAT&                    GetShininess() const                        { return _shininess; }

            void                            SetOpacity(const FLOAT& opacity)            { _opacity = opacity; }
            const FLOAT&                    GetOpacity() const                          { return _opacity; }

            void                            SetName(const wstring& name)                { _name = name; }
            const wstring&                  GetName() const                             { return _name; }

            void                            SetTransparencyEnabled(BOOL state)          { _blendEnabled = state; }
            const BOOL&                     GetTransparencyEnabled() const              { return _blendEnabled; }

            void                            SetTransparencyModes(Blend src, Blend dst, Operation op = Operation::Add);

            static void                     SetPass(uint8_t pass)                       { _pass = pass; }
            static uint8_t                  GetPass()                                   { return _pass; }

            bool                            CheckPass()                                 { return _pass == _blendEnabled; }

private:
    wstring                             _name{ L"Material" };
	shared_ptr<Shader>                  _shader = nullptr;

	IDLIST(shared_ptr<Texture>)         _textures;
    shared_ptr<CBO>                     _constantBuffer;

    XMFLOAT4                            _albedo;
    XMFLOAT4                            _specular;
    FLOAT                               _shininess;
    FLOAT                               _opacity;

    Blend                               _sourceBlend;
    Blend                               _destinationBlend;
    Operation                           _blendOperation;
    BOOL                                _blendEnabled;

    ComPtr<ID3D11BlendState>            _blendStateObject{ NULL };

    static Material*                    _activeMaterial;
    static uint8_t                      _pass;
};

#undef IDLIST