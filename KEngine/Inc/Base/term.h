#pragma once

namespace K
{
	class Actor;
	class Component;

	using APTR = std::shared_ptr<Actor>;
	using CPTR = std::unique_ptr<Component, std::function<void(Component*)>>;

	template <typename T> constexpr auto APTR_CAST(APTR const& _p) { return std::static_pointer_cast<T>(_p); }
	template <typename T> constexpr auto CPTR_CAST(CPTR const& _p) { return static_cast<T*>(_p.get()); }

	//////////////////// Path Key ////////////////////
	constexpr auto ROOT_PATH = "RootPath";
	constexpr auto TEXTURE_PATH = "TexturePath";
	constexpr auto AUDIO_PATH = "AudioPath";
	constexpr auto VIDEO_PATH = "VideoPath";
	constexpr auto SHADER_PATH = "ShaderPath";
	constexpr auto DATA_PATH = "DataPath";

	//////////////////// Mesh Key ////////////////////
	constexpr auto TEX_RECT = "TexRect";
	constexpr auto COLLIDER_RECT = "ColliderRect";
	constexpr auto COLLIDER_CIRCLE = "ColliderCircle";
	constexpr auto INSTANCE_TEX_RECT = "InstanceTexRect";

	//////////////////// Sampler Key ////////////////////
	constexpr auto LINEAR_SAMPLER = "LinearSampler";

	//////////////////// Shader Key ////////////////////
	constexpr auto BASIC_SHADER = "BasicShader";
	constexpr auto BASIC_TEX_SHADER = "BasicTexShader";
	constexpr auto BASIC_ANIMATION_2D_SHADER = "BasicAnimation2DShader";
	constexpr auto COLLIDER_SHADER = "ColliderShader";
	constexpr auto INSTANCE_TEX_SHADER = "InstanceTexShader";

	//////////////////// RenderState Key ////////////////////
	constexpr auto ALPHA_BLEND = "AlphaBlend";
	constexpr auto DEPTH_DISABLE = "DepthDisable";
	constexpr auto RGB_ADD_BLEND = "RGBAddBlend";

	//////////////////// Common Key ////////////////////
	constexpr auto SOLUTION_NAME = "K Diablo II Expansion";
	constexpr auto DEFAULT = "Default";
	constexpr auto UI = "UI";

	//////////////////// Actor Key ////////////////////
	constexpr auto DEFAULT_CAMERA = "DefaultCamera";
	constexpr auto UI_CAMERA = "UICamera";
	constexpr auto TILE = "Tile";
	constexpr auto TILE_MAP = "TileMap";

	//////////////////// Component Key ////////////////////
	constexpr auto TRANSFORM = "Transform";
	constexpr auto CAMERA = "Camera";
	constexpr auto TEXT = "Text";
	constexpr auto MATERIAL = "Material";
	constexpr auto RENDERER = "Renderer";
	constexpr auto ANIMATION_2D = "Animation2D";
	constexpr auto COLLIDER = "Collider";
	constexpr auto NAVIGATOR = "Navigator";

	//////////////////// Constant ////////////////////
	const auto MTU_SIZE = 1024;
	const auto USER_SIZE = 100;
	const auto MAX_MESSAGE_SIZE = 64;

	//////////////////// Protocol ////////////////////
	const auto CS_LOGIN = 1;
	const auto CS_CHAT = 2;

	const auto SC_LOGIN = 1;
	const auto SC_CHAT = 2;
	const auto SC_REPLICATION_CREATE = 3;
	const auto SC_REPLICATION_UPDATE = 4;
	const auto SC_REPLICATION_DESTROY = 5;

#pragma pack(push, 1)
	struct CS_PACKET_LOGIN
	{
		uint8_t size;
		uint8_t type;
	};

	struct SC_PACKET_LOGIN
	{
		uint8_t size;
		uint8_t type;
		uint8_t id;
	};

	struct CS_PACKET_CHAT
	{
		uint8_t size;
		uint8_t type;
		wchar_t message[64];
	};

	struct SC_PACKET_CHAT
	{
		uint8_t size;
		uint8_t type;
		uint8_t id;
		wchar_t message[MAX_MESSAGE_SIZE];
	};

	struct SC_PACKET_REPLICATION
	{
		uint8_t size;
		uint8_t type;
	};
#pragma pack(pop)

	////////////////////////////////////////////////////////////

	struct VertexColor
	{
		Vector3 position;
		Vector4 color;
	};

	struct VertexTex
	{
		Vector3 position;
		Vector2 uv;
	};

	struct MatrixTex
	{
		Vector2 LT;
		Vector2 RB;
		Matrix world;
		Vector4 option;
	};

	struct CSO_DESC
	{
		SHADER_TYPE type;
		std::wstring file_name;
	};

	struct ANIMATION_2D_FRAME_DESC
	{
		Vector2 LT;
		Vector2 RB;
	};

	struct ANIMATION_2D_CLIP_DESC
	{
		std::string clip_tag;
		std::string texture_tag;
		ANIMATION_2D_TYPE type;
		ANIMATION_OPTION option;
		float width;
		float height;
		float completion_time;
		std::vector<ANIMATION_2D_FRAME_DESC> frame_vector;
	};

	struct TransformConstantBuffer
	{
		Matrix world;
		Matrix view;
		Matrix projection;
		Matrix WVP;
	};

	struct MaterialConstantBuffer
	{
		Vector4 diffuse;
	};

	struct Animation2DConstantBuffer
	{
		Vector2 LT;
		Vector2 RB;
	};

	struct OptionConstantBuffer
	{
		int option;
	};
}
