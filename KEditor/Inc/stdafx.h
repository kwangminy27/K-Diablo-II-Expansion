
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

// C++ Standard Library
#include <filesystem>

// KEngine
#include <KEngine.h>
#include <core.h>
#include <device_manager.h>
#include <path_manager.h>
#include <Resource/resource_manager.h>
#include <Rendering/rendering_manager.h>
#include <time_manager.h>
#include <input_manager.h>
#include <World/world_manager.h>
#include <World/level.h>
#include <World/layer.h>
#include <Object/object_manager.h>
#include <Object/Actor/actor.h>
#include <Object/Actor/camera_actor.h>
#include <Object/Actor/tile_map_actor.h>
#include <Object/Component/component.h>
#include <Object/Component/transform.h>
#include <Object/Component/text.h>
#include <Object/Component/material.h>
#include <Object/Component/renderer.h>
#include <Object/Component/animation_2d.h>
#include <Object/Component/Collider/collider.h>
#include <Object/Component/Collider/collider_point.h>
#include <Object/Component/Collider/collider_circle.h>
#include <Object/Component/Collider/collider_aabb.h>
#include <Object/Component/Collider/collider_oobb.h>
#include <registry_manager.h>

// Base
#include "Base/enum.h"
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
