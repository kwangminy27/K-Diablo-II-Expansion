#include "type.hlsl"

VS_OUTPUT_POSITION_TEX_INSTANCE InstanceTexVS(VS_INPUT_POSITION_TEX_INSTANCE _input)
{
	VS_OUTPUT_POSITION_TEX_INSTANCE output = (VS_OUTPUT_POSITION_TEX_INSTANCE)0;

	output.position = mul(mul(mul(float4(_input.position, 1.f), _input.world), g_view), g_projection);

	output.uv.x = _input.position.x == -0.5f ? _input.LT.x : _input.RB.x;
	output.uv.y = _input.position.y == -0.5f ? _input.RB.y : _input.LT.y;

	output.option = _input.option;

	return output;
}
