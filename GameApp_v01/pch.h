// pch.h: 미리 컴파일된 헤더 파일입니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <comdef.h>	// 유틸리티, 매크로, 데이터 타입 정의

// Cpp 관련 헤더
#include <iostream>
using namespace std;

// 내가 만든 헤더
#include "Tool/Helper.h"


// D2D
#include "d2d1helper.h"		// 나중에 simplemath 에 있는 걸로 사용하자

// D3D
#include <d3d11.h>

#include <directxtk/simplemath.h>							// 수학 연산을 단순화하는 클래스들을 제공
// using namespace DirectX::SimpleMath;

#pragma comment(lib, "d3d11.lib")							// Direct3D 11 라이브러리
#pragma comment(lib, "d3dcompiler.lib")						// Direct3D 컴파일러 라이브러리(Direct3D 셰이더 코드(HLSL)를 컴파일하는 데 사용)


//// DirectXTex : 텍스처 이미지 파일을 로드하고 저장하며, 변환하는 데 사용
//#ifdef DEBUG
//#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")	// DirectXTex 라이브러리의 디버그 버전
//#else
//#pragma comment(lib, "DirectXTex\\DirectXTex.lib")			// DirectXTex 라이브러리의 릴리즈 버전
//#endif // DEBUG

#endif //PCH_H
