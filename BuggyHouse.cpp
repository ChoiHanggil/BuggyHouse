#include "BuggyHouse.h"
#include"Bug.h"
HRESULT BuggyHouse::Init(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Init(hInstance, title, width, height);
	ThrowIfFailed(hr);

	mspBackground = std::make_unique<Actor>(this, L"Images/back1_1024.png");


	for (int i = 0; i < 40; i++)
	{
		mBugs.push_back(std::make_shared<Bug>(this));
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
	mspRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	CheckBugs();

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

void BuggyHouse::CheckBugs()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(mHwnd, &pt);

		auto it = std::remove_if(mBugs.begin(), mBugs.end(),
			[&](auto& bug) {return bug.get()->IsClicked(pt); }
		);

		mBugs.erase(it, mBugs.end());
	}
}

//list
//remove : 실제로 지우는게 아니라 징운다고 뒤로 이동시키기만 함(컨테이너 정리)
// erase : 실제로 지우고 메모리 해제
// 
// erase-remove idiom
// 
// erase(remove());
// 
// myList.erase(
//	std::remove_if(myList.begin(), myList.end(), [](e){ruten e = ?}),
//	 myList.end()
// )
// 
//