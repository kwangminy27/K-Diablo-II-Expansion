#pragma once

// Windows API
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

// C++ Standard Library
#include <functional>
#include <iostream>
#include <unordered_map>

// KCommon
#include <KCommon.h>
#include <singleton.h>

// Base
#include "Base/macro.h"
#include "Base/term.h"
