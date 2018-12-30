#pragma once

// Windows API
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// C++ Standard Library
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <random>
#include <unordered_map>
#include <utility>

// DirectX
#include <wrl/client.h>
#include <dxgidebug.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

// DirectXMath
#include <DirectXColors.h>

// DirectXTK
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <Audio.h>

// DirectShow
#include <dshow.h>

#pragma comment(lib, "strmiids")

// KCommon
#include <KCommon.h>
#include <singleton.h>
#include <tag.h>

// KPhysics
#include <KPhysics.h>
#include <type.h>

// KNetwork
#include <KNetwork.h>
#include <Socket/socket_manager.h>
#include <Socket/socket_address.h>
#include <Socket/tcp_socket.h>
#include <Socket/udp_socket.h>
#include <Stream/memory_stream.h>

// Base
#include "Base/enum.h"
#include "Base/macro.h"
#include "Base/term.h"

namespace K
{
	static void ThrowIfFailed(HRESULT _result)
	{
		if (FAILED(_result))
			throw std::exception{};
	}
}
