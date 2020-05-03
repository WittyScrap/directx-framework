#pragma once
#include "SimplexNoise.h"
#include "DirectXCore.h"

enum class NoiseDirection
{
    ND_Outwards = 1,
    ND_Inwards = -1
};

enum class NoiseBlendMode
{
    BM_Additive,
    BM_Multiply
};

/**
 * Wrapper class for noise generation.
 *
 */
class BasicNoise
{
public:
                                    BasicNoise()                                   {}
    virtual                        ~BasicNoise()                                   {}

    virtual FLOAT                   GetNoiseValue(FLOAT x, FLOAT y, FLOAT z) const;

    inline  FLOAT                   GetNoiseStartX() const                          { return _noiseOffsetX; }
    inline  FLOAT                   GetNoiseStartY() const                          { return _noiseOffsetY; }
    inline  FLOAT                   GetNoiseStartZ() const                          { return _noiseOffsetZ; }

    inline  void                    SetNoiseOffsetX(const FLOAT& value)             { _noiseOffsetX = value; }
    inline  void                    SetNoiseOffsetY(const FLOAT& value)             { _noiseOffsetY = value; }
    inline  void                    SetNoiseOffsetZ(const FLOAT& value)             { _noiseOffsetZ = value; }

    inline  void                    SetNoiseOctaves(const size_t& value)            { _octaves = value; }
    inline  void                    SetNoise(SimplexNoise& noise)                   { _noise = noise; }
    inline  void                    SetNoiseScale(const FLOAT& scale)               { _noiseScale = scale; }

    inline  FLOAT                   GetPeakHeight() const                           { return _peakHeight; }
    inline  void                    SetPeakHeight(FLOAT value)                      { _peakHeight = value; }

    inline  NoiseDirection          GetNoiseDirection() const                       { return _direction; }
    inline  void                    SetNoiseDirection(const NoiseDirection dir)     { _direction = dir; }

    inline  NoiseBlendMode          GetNoiseBlendMode() const                       { return _blendMode; }
    inline  void                    SetNoiseBlendMode(const NoiseBlendMode mode)    { _blendMode = mode; }

private:
    SimplexNoise                    _noise;

    FLOAT                           _noiseOffsetX{ 0 };
    FLOAT                           _noiseOffsetY{ 0 };
    FLOAT                           _noiseOffsetZ{ 0 };

    FLOAT                           _noiseScale{ 1 };
    SIZE_T                          _octaves{ 1 };

    FLOAT                           _peakHeight{ 5.f };

    NoiseDirection                  _direction{ NoiseDirection::ND_Outwards };
    NoiseBlendMode                  _blendMode{ NoiseBlendMode::BM_Additive };
};