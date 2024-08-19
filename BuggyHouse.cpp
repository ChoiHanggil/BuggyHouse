#include "BuggyHouse.h"

HRESULT BuggyHouse::Init(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Init(hInstance, title, width, height);
	ThrowIfFailed(hr);

	mspBackground = std::make_unique<Actor>(this, L"Images/back1_1024.png");

	float sx{}, sy{};
	RECT rct;
	GetClientRect(mHwnd, &rct);

	for (int i = 0; i < 40; i++)
	{
		sx = (float)(rand() % (rct.right - rct.left));
		sy = (float)(rand() % (rct.bottom - rct.top));

		mBugs.push_back(std::make_shared<Actor>(this, L"Images/bug.png", sx, sy));

	}
	return S_OK;
}


void BuggyHouse::Release()
{
	for (auto& bug : mBugs)
	{
		bug.reset();
	}
	mBugs.clear();

	mspBackground.reset();

	D2DFramework::Release();
}

void BuggyHouse::Render()
{
	mspRenderTarget->BeginDraw();

	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	mspBackground->Draw();
	//한 반향으로 지속이동(1초 or 10픽셀)
	//	방향 전환 시도

	//정해진 위치 (화면 가장자리)에서 출몰하도록 
	for (auto& bug : mBugs)
	{

		bug->Draw();
	}

	HRESULT hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
