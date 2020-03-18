#pragma once
#include "Shader.h"

/**
 * The Material class will hold and manage a reference to a Shader,
 * as well as any Texture or property that the shader may use.
 *
 */
class Material
{
public:
                          Material()                                                        {}
                          Material(wstring source) : _shader(make_shared<Shader>(source))   { _shader->CompileOnce(); }

    virtual               ~Material()                                                       {}

            void          SetShader(Shader& source);
	        void          SetShader(const wstring& source);
	        void          SetTexture(const wstring& textureName);

            const Shader& GetShader() const;

            void          Activate();
            void          Update(CBUFFER* cbuf);

            ComPtr<ID3D11ShaderResourceView> GetTexture() const;

private:
	shared_ptr<Shader>                  _shader = nullptr;

	ComPtr<ID3D11ShaderResourceView>    _texture;
	wstring                             _textureName;
};

