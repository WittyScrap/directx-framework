
cbuffer ConstantBuffer : register(b0)
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
};

Texture2D Texture : register(t0);
SamplerState ss;

struct VertexIn
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VertexOut
{
	float4 Position  : SV_POSITION;
	float2 TexCoord	 : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.Position = mul(completeTransform, float4(vin.Position, 1.0f));
	vout.TexCoord = vin.TexCoord;
    
    return vout;
}

float4 PS(VertexOut input) : SV_Target
{
	return Texture.Sample(ss, input.TexCoord);
}


