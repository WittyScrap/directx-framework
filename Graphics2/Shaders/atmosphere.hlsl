//
// Shader source: GPU Gems (NVIDIA)
//

cbuffer ConstantBuffer 
{
	matrix completeTransform;	// The complete transformation
	matrix worldTransform;		// The world transformation matrix
	float4 cameraPosition;		// The world position of the camera
	float4 lightVector;			// The directional light's vector
	float4 lightColor;			// The directional light's colour
	float4 ambientColor;		// The ambient light's colour
	float4 diffCoefficient;		// The coefficient to be used for diffuse coloring
	float4 specCoefficient;		// The coefficient to be used for specular coloring
	float  shininess;			// How shiny this material should be
	float  opacity;				// The opacity of this material.
	float2 padding;				// Padding to be applied for lighting calculations

	// Atmosphere-specific data
	float3 v3PlanetPosition;	// The position of the planet in world space
	float  fOuterRadius;		// The outer (atmosphere) radius
	float  fInnerRadius;		// The inner (planetary) radius
};

Texture2D atmoLUT;
SamplerState samplerState;

struct VertexIn
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
	float4 Position		: SV_POSITION;
	float2 TexCoord		: TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.Position = mul(completeTransform, float4(vin.Position, 1.0f));

	float3 cameraPos = cameraPosition.xyz - v3PlanetPosition;
	float cameraHeight = length(cameraPos);
	float cameraHeight2 = cameraHeight * cameraHeight;

	float3 atmosphereFar = mul(worldTransform, float4(vin.Position, 1.0f)).xyz - v3PlanetPosition;
	float3 ray = atmosphereFar - cameraPos;
	float farDistance = length(ray);
	ray /= farDistance;

	float3 atmosphereFarPoint = mul(worldTransform, float4(vin.Position, 1.0f)).xyz;
	float3 farPointToPlanetCenter = normalize(v3PlanetPosition - atmosphereFarPoint);
	float3 cameraAtmosphereRay = normalize(cameraPosition.xyz - atmosphereFarPoint);
	float atmosphereFallof = dot(cameraAtmosphereRay, farPointToPlanetCenter);

	float B = 2.0 * dot(cameraPos, ray);
	float C = cameraHeight2 - (fOuterRadius * fOuterRadius);
	float det = max(0.0, B * B - 4.0 * C);
	float near = 0.5 * (-B - sqrt(det));

	float3 atmosphereNear = cameraPos + ray * near;
	float3 atmosphereNormal = normalize(atmosphereNear - v3PlanetPosition);

	float rayAngle = dot(ray, -lightVector.xyz);
	float lightAngle = saturate((dot(atmosphereNormal, -lightVector.xyz) + .5f) * 2);

	vout.TexCoord = float2(lightAngle, atmosphereFallof);

	return vout;
}

float4 PS(VertexOut input) : SV_Target
{
	return atmoLUT.Sample(samplerState, input.TexCoord) * input.TexCoord.x * input.TexCoord.y;
}