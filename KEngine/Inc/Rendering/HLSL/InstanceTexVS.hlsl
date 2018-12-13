#include "type.hlsl"

VS_OUTPUT_POSITION_TEX InstanceTexVS(VS_INPUT_POSITION_TEX_INSTANCE _input)
{
	VS_OUTPUT_POSITION_TEX output = (VS_OUTPUT_POSITION_TEX)0;

	output.position = mul(mul(mul(float4(_input.position, 1.f), _input.world), g_view), g_projection);
	output.uv = _input.uv;

	return output;
}
