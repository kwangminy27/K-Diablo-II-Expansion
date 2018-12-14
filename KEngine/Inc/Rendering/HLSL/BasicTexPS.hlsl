#include "type.hlsl"

PS_OUTPUT_SINGLE_TARGET BasicTexPS(VS_OUTPUT_POSITION_TEX _input)
{
	PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

	output.target = g_texture.Sample(g_sampler, _input.uv) * g_diffuse;

	float3 color = output.target.rgb;

	if (color.r == 1.f && color.g == 0.f && color.b == 1.f)
		discard;

	return output;
}