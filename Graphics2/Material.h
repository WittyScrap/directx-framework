#pragma once
#include "Shader.h"
#include "Texture.h"
#include "ConstantBufferObject.h"
#include <map>

#define IDLIST(t) map<int, t>

struct MeshObjectData
{
    XMMATRIX completeTransformation;
    XMMATRIX worldTransformation;
};

enum class Transparency
{
    Opaque,
    Transparent
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
                                            _name{ name }, _shader(source), _albedo{ 1, 1, 1, 1 }, _specular{ 1, 1, 1, 1 }, _shininess{ 0 }, _opacity{ 1 }  { if (_shader) _shader->CompileOnce(); }

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

            void                            SetTransparencyMode(Transparency value);
            const Transparency&             GetTransparencyMode() const                 { return _transparency; }

private:
    wstring                             _name{ L"Material" };
	shared_ptr<Shader>                  _shader = nullptr;

	IDLIST(shared_ptr<Texture>)         _textures;
    shared_ptr<CBO>                     _constantBuffer;

    XMFLOAT4                            _albedo;
    XMFLOAT4                            _specular;
    FLOAT                               _shininess;
    FLOAT                               _opacity;

    Transparency                        _transparency{ Transparency::Opaque };
    ComPtr<ID3D11BlendState>            _blendStateObject{ NULL };

    static Material*                    _activeMaterial;
};

#undef IDLIST