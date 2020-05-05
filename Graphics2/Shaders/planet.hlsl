
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
	float  planetOuterRadius;
	float  planetResolution;
	float3 planetPosition;
};

Texture2D ground : register(t0);
Texture2D cliff : register(t1);
Texture2D sand : register(t2);
Texture2D snow : register(t3);

Texture2D atmo : register(t4);

SamplerState ss;

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
	float3 PositionLS	: TEXCOORD1;
	float4 PositionWS	: TEXCOORD2;
	float3 NormalLS		: TEXCOORD3;
	float4 NormalWS		: TEXCOORD4;
	float3 NormalSphere : TEXCOORD5;
	float  Fog			: TEXCOORD6;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	vout.Position = mul(completeTransform, float4(vin.Position, 1.0f));
	vout.PositionLS = vin.Position;
	vout.PositionWS = mul(worldTransform, float4(vin.Position, 1.0f));
	vout.NormalLS = vin.Normal;
	vout.NormalWS = float4(mul((float3x3)worldTransform, vin.Normal), 1.0f);
	vout.NormalSphere = normalize(vout.PositionWS.xyz - planetPosition);
	vout.TexCoord = vin.TexCoord;

	float outerRadius2 = planetOuterRadius * planetOuterRadius;
	float innerRadius2 = planetRadius * planetRadius;

	float3 cameraPos = cameraPosition.xyz - planetPosition;
	float cameraHeight = length(cameraPos);
	float cameraHeight2 = cameraHeight * cameraHeight;

	float3 atmosphereFar = mul(worldTransform, float4(vin.Position, 1.0f)).xyz - planetPosition;
	float3 ray = atmosphereFar - cameraPos;
	float farDistance = length(ray);
	ray /= farDistance;

	float B = 2.0 * dot(cameraPos, ray);
	float C = cameraHeight2 - (planetOuterRadius * planetRadius);
	float det = max(0.0, B * B - 4.0 * C);
	float near = 0.5 * (-B - sqrt(det));

	float lightRayTravelDistance = farDistance - near;
	float maximumTravelDistance = sqrt(outerRadius2 - innerRadius2);

	vout.Fog = (lightRayTravelDistance / maximumTravelDistance) * dot(vout.NormalSphere, -lightVector.xyz);
    
    return vout;
}

float4 PS(VertexOut input) : SV_Target
{
	float4 viewDirection = normalize(cameraPosition - input.PositionWS);
	float4 directionToLight = normalize(-lightVector);

	// Calculate diffuse lighting
	float4 adjustedNormal = normalize(input.NormalWS);
	float NdotL = saturate(dot(adjustedNormal, directionToLight));
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
	len = saturate((len - .5) * 2.f);

	float slope = (1 - dot(normalize(input.PositionLS), input.NormalLS)) * 6.f;
	float desert = 1 - saturate((abs(dot(normalize(input.PositionLS), float3(0, 1, 0))) - .05f) * 4.f);
	float ice = saturate((abs(dot(normalize(input.PositionLS), float3(0, 1, 0))) - .5f) * 4.f);

	float3 cameraPos = cameraPosition.xyz - planetPosition;
	float cameraHeight = length(cameraPos);
	cameraHeight -= planetRadius;
	cameraHeight /= (planetOuterRadius - planetRadius);
	cameraHeight = 1 - saturate(cameraHeight);

	float2 uv = (input.TexCoord * 8) % 1.f;
	float2 uvDetail = (input.TexCoord * 64) % 1.f;

	float4 flatGround = ground.Sample(ss, uv);
	float4 cliffRocks = cliff.Sample(ss, uv);
	float4 desertSand = sand.Sample(ss, uv);
	float4 HiPeakSnow = snow.Sample(ss, uv) * 2;

	float4 flatDetail   = ground.Sample(ss, uvDetail);
	float4 cliffDetail  = cliff.Sample(ss, uvDetail);
	float4 desertDetail = sand.Sample(ss, uvDetail);
	float4 snowDetail   = snow.Sample(ss, uvDetail) * 2;

	flatGround = lerp(flatGround, flatDetail, cameraHeight);
	cliffRocks = lerp(cliffRocks, cliffDetail, cameraHeight);
	desertSand = lerp(desertSand, desertDetail, cameraHeight);
	HiPeakSnow = lerp(HiPeakSnow, snowDetail, cameraHeight);

	float4 mountainDetection = lerp(flatGround, HiPeakSnow, len);
	float4 desertDetection = lerp(mountainDetection, desertSand, desert * .5f);
	float4 polesDetection = lerp(desertDetection, HiPeakSnow, ice);
	float4 cliffDetection = lerp(polesDetection, cliffRocks, slope);

	float4 color = light * cliffDetection;
	float sphereLightFactor = saturate(dot(input.NormalSphere, -lightVector.xyz) + .15f);

	float4 foggyColor = atmo.Sample(ss, float2(sphereLightFactor, 1.f));
	color = lerp(color, foggyColor, input.Fog);

	return color;
}


