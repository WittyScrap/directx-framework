#pragma once
#include "ILight.h"

/**
 * Ambient light.
 *
 */
class AmbientLight : public ILight
{
public:
                                    AmbientLight() : _col{ 1, 1, 1, 1 }                                                     {}

                                    AmbientLight(const XMFLOAT4& col) : _col{ col }                                         {}
                                    AmbientLight(const AmbientLight& d) : _col{ d._col }                                    {}

    virtual                         ~AmbientLight()                                                                         {}
    
    XMFLOAT4&                       GetColor() override                                                                     { return _col; }
    void                            SetColor(const XMFLOAT4& c) override                                                    { _col = c; }

private:
    XMFLOAT4                        _col;
};

