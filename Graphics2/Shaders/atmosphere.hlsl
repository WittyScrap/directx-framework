#define PI		3.14159265359f

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

Texture2D atmoLUT : register(t0);
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

	// Store away squared values for easier access
	float innerRadius2 = fInnerRadius * fInnerRadius;
	float outerRadius2 = fOuterRadius * fOuterRadius;

	// Determine camera location in local space to extract its height from the top of the atmosphere
	float3 cameraPos = cameraPosition.xyz - v3PlanetPosition;
	float cameraHeight = length(cameraPos);
	float cameraHeight2 = cameraHeight * cameraHeight;

	// Calculate a ray from the camera's location to the "far point", AKA the vertex that is currently being rendered.
	// Because the atmosphere's sphere is inverted, only the "far" points will be rendered at any given point in time.
	float3 atmosphereFarPoint = mul(worldTransform, float4(vin.Position, 1.0f)).xyz;
	float3 relativeFarPoint = mul(worldTransform, float4(vin.Position, 1.0f)).xyz - v3PlanetPosition;
	float3 ray = relativeFarPoint - cameraPos;
	float farDistance = length(ray);
	ray /= farDistance;

	float3 farPointToPlanetCenter = normalize(v3PlanetPosition - atmosphereFarPoint);
	float3 cameraAtmosphereRay = normalize(cameraPosition.xyz - atmosphereFarPoint);

	// Determine near point
	float B = 2.0 * dot(cameraPos, ray);
	float C = cameraHeight2 - (fOuterRadius * fOuterRadius);
	float det = max(0.0, B * B - 4.0 * C);
	float near = 0.5 * (-B - sqrt(det));

	// Calculate a gradient to determine the amount by which the camera has entered
	// the atmosphere.
	float atmosphereVertexHeight = length(atmosphereFarPoint - v3PlanetPosition);
	float atmosphereThickness = fOuterRadius - fInnerRadius;
	float cameraInsetAmount = saturate(cameraHeight - atmosphereVertexHeight);
	float cameraAtmosphereGradient = saturate((cameraHeight - fInnerRadius) / atmosphereThickness);

	// Calculate the maximum distance that a light ray could travel across the atmosphere when
	// in space and when on the surface of the planet.
	float maximumTravelDistanceGround = sqrt(outerRadius2 - innerRadius2);
	float maximumTravelDistanceSpace = maximumTravelDistanceGround * 2;
	float currentMaximumDistance = lerp(maximumTravelDistanceGround, maximumTravelDistanceSpace, cameraAtmosphereGradient); // The current maximum distance depends on how close to the ground the camera is.

	// Calculate the total distance the light ray had to travel to get to the camera.
	float lightTravelDistance = length(cameraPosition.xyz - atmosphereFarPoint) - max(near, 0);
	float atmosphereFalloff = lightTravelDistance / currentMaximumDistance; // The falloff gradient

	// Determine the normal of the "near" point in the atmosphere to calculate light value
	float3 atmosphereNear = cameraPos + ray * near * cameraInsetAmount;
	float3 atmosphereNormal = normalize(atmosphereNear - v3PlanetPosition);

	// Finally, calculate the light amount using the previously determined normal.
	// Add a bias of 0.25 because atmosphere scattering would allow light to illuminate parts of the
	// atmosphere slightly beyond the sphere's diameter.
	float lightAngle = saturate((dot(atmosphereNormal, -lightVector.xyz) + .25f) * 2);

	// Create LUT map coordinates
	vout.TexCoord = float2(lightAngle, atmosphereFalloff);

	return vout;
}

float4 PS(VertexOut input) : SV_Target
{
	return atmoLUT.Sample(samplerState, input.TexCoord) * input.TexCoord.x * input.TexCoord.y;
}