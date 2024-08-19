#include<Windows.h>
#include "BuggyHouse.h"


BuggyHouse myFramework;

int WINAPI WinMain(
	_In_ HINSTANCE hInstacne,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int ret{};
	try
	{
		myFramework.Init(hInstacne);

		ret = myFramework.GameLoop();
		// 4. 해제
		myFramework.Release();
	}
	catch (const com_exception& e)
	{
		OutputDebugStringA(e.what());
		
	}

	return ret;
}

//Unreal
// Actor (스프라이트)
//	WIC 이미지 로딩
//	ID2D1Bitmap
//	x,y
//	투명도
// 
// 
// 
//unity
// Object
//	PlacableObject
//		Character
//		BackGround
//		Effect
//	Logic
//	Manager
//