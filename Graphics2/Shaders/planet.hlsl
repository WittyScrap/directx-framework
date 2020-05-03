
cbuffer ConstantBuffer 
{
	// General (base) data
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

	// Planet-specific data
	float  planetRadius;
	float  planetPeaks;
	float  planetResolution;
};

Texture2D Texture;
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
	float3 PositionLS: TEXCOORD1;
	float4 PositionWS: TEXCOORD2;
	float3 NormalLS : TEXCOORD3;
	float4 NormalWS : TEXCOORD4;
	float2 TexCoord	 : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.Position = mul(completeTransform, float4(vin.Position, 1.0f));
	vout.PositionLS = vin.Position;
	vout.PositionWS = mul(worldTransform, float4(vin.Position, 1.0f));
	vout.NormalLS = vin.Normal;
	vout.NormalWS = float4(mul((float3x3)worldTransform, vin.Normal), 1.0f);
	vout.TexCoord = vin.TexCoord;
    
    return vout;
}

float4 PS(VertexOut input) : SV_Target
{
	float4 viewDirection = normalize(cameraPosition - input.PositionWS);
	float4 directionToLight = normalize(-lightVector);

	// Calculate diffuse lighting
	float4 adjustedNormal = normalize(input.NormalWS);
	float NdotL = max(0, dot(adjustedNormal, directionToLight));
	float4 diffuse = saturate(lightColor * NdotL * diffCoefficient);

	// Calculate specular component
	float4 R = 2 * NdotL * adjustedNormal - directionToLight;
	float RdotV = max(0, dot(R, viewDirection));
	float4 specular = saturate(lightColor * pow(RdotV, shininess) * specCoefficient);

	// Calculate ambient lighting
	float4 ambientLight = ambientColor * diffCoefficient * .25f;

	// Combine all components
	float4 light = saturate(ambientLight + diffuse/* + specular*/);

	float len = length(input.PositionLS);
	len -= planetRadius;
	len /= planetPeaks;

	float slope = dot(normalize(input.PositionLS), input.NormalLS) * .65f;
	float desert = saturate(-log10(abs(dot(normalize(input.PositionLS), float3(0, 1, 0))))) * .5f;
	float ice = saturate(-log10(1 - abs(dot(normalize(input.PositionLS), float3(0, 1, 0)))));

	float4 color = lerp(.25f * light, light * float4(.75f, 1.f, .65f, 1.f), slope);
		   color = lerp(color, light, len);
		   color = lerp(color, light * float4(1.f, .75f, .1f, 1.f), desert);
		   color = lerp(color, light, ice);

	color.a = saturate(opacity);

	return color * Texture.Sample(ss, input.TexCoord);
}


