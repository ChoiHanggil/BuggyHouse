#pragma once
#include<d2d1.h>
#include<wincodec.h>
#include<wrl/client.h>
#include "ComException.h"

class D2DFramework
{
public:
	virtual HRESULT Init(HINSTANCE hInstacne, LPCWSTR title = L"D2DFramework", UINT width = 1024, UINT height = 768);
	
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

public:
	ID2D1Factory* GetD2DFactory() { return mspD2DFactory.Get(); }
	ID2D1HwndRenderTarget* GetRenderTarget() { return mspRenderTarget.Get(); }

protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

	HRESULT InitWindow(HINSTANCE hInstacne, LPCWSTR title, UINT width, UINT height);
	HRESULT InitD2D();

	virtual HRESULT CreateDeviceResources();

protected:
	HWND mHwnd;
	

private:
	//컴파일은 cpp에서만 가능 / 헤더에서는 안됨
	const wchar_t* CLASS_NAME{ L"Direct2DSampleClass" };
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
