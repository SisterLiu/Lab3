//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 CameraPos;
	float3 LightPos;
}

SamplerState samplerLinear : register(s0);
Texture2D woodDiffuse : register(t0);

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Color : COLOR;
	float3 Nor : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float2 Color : TEXTURE, float3 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = Color;

	float kc = 0.5;
	float kd = 0.5;
	float ks = 0.8;
	float3 pointPos = Pos.xyz;
	float3 cColor = float3(1.0, 1.0, 0.0);
	float3 cI, sI, dI;
	cI = cColor;
	sI = cColor;
	for(int i = 0; i < 2; i++)
	{
		sI *= normalize(CameraPos - pointPos)*reflect(normalize(pointPos - LightPos), Normal);
	}
	dI = cColor * (normalize(pointPos - LightPos)*Normal);
	//output.Nor = kc*cI;
	output.Nor = kc*cI + ks*sI + kd*dI;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	return float4(input.Nor,1.0);
//return woodDiffuse.Sample(samplerLinear, input.Color);
}
