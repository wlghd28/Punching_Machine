// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "IO.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.
#include<Shlwapi.h>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "UxTheme.lib")

#define CAMERA_DPI 2540
#define CAMERA_WIDTH 1280
#define CAMERA_HEIGHT 1024
#define CAMERA_CENTERLINE_THICKNESS 6

#ifdef _DEBUG
#endif


// VC6
#if defined(_MSC_VER) && _MSC_VER == 1200

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif

#include <Specstrings.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// VS2005
#else

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#endif

#endif //PCH_H
