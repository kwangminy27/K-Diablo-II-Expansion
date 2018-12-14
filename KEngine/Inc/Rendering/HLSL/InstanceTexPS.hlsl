#include "type.hlsl"

PS_OUTPUT_SINGLE_TARGET InstanceTexPS(VS_OUTPUT_POSITION_TEX_INSTANCE _input)
{
	PS_OUTPUT_SINGLE_TARGET output = (PS_OUTPUT_SINGLE_TARGET)0;

	output.target = g_texture.Sample(g_sampler, _input.uv) * g_diffuse;

	float4 color = output.target;

	if (color.a <= 0.05f)
		discard;

	if (_input.option.x == 1.f)
		output.target = float4(0.5f, color.g, color.b, 1.f);

	return output;
}
