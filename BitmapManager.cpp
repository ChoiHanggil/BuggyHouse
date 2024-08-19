#include "BitmapManager.h"
#include "ComException.h"

#pragma comment(lib, "WindowsCodecs.lib")

using namespace std;
using namespace Microsoft::WRL;


HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
    if (pRT == nullptr)
    {
        return E_FAIL;
    }
    mpRendertarget = pRT;

    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(mspWICFactory.GetAddressOf()));
    ThrowIfFailed(hr);

    return hr;

}

void BitmapManager::Release()
{
    for (auto& bitmap : mBitmapResources)
    {
        bitmap.second.Reset();
    }
    mBitmapResources.clear();
    mspWICFactory.Reset();

}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename)
{
   //map에 해당 파일명(키)이 있느냐?
   //있다면 해당 비트맵(벨류)를 반환
   // 없다면 이미지 로딩 후 맵에 추가

    //auto result = myMap.insert(<"1.png", nullptr>);
    //if (result.second)
    //{
    //    //처음추가
    //}
    //else
    //{
    //    //이미 존재
    //    return result.first->second;
    //}

    if (mspWICFactory == nullptr)
    {
        return nullptr;
    }

    auto result = mBitmapResources.insert(pair<wstring, ComPtr<ID2D1Bitmap>>(filename, nullptr));
    if (result.second)
    {
        auto spBitmap = ComPtr<ID2D1Bitmap>();
        LoadWICBitmap(filename, spBitmap.GetAddressOf());
        result.first->second = spBitmap;
    }
  
    return result.first->second.Get();
}

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
    HRESULT hr{};

    auto pWICFactory{ mspWICFactory.Get() };
    if (pWICFactory == nullptr)
    {
        return E_FAIL;
    }

    // 디코더 생성
    hr = pWICFactory->CreateDecoderFromFilename(
        filename.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        bitmapDecoder.GetAddressOf());
    ThrowIfFailed(hr);

    // 프레임 획득
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    ThrowIfFailed(bitmapDecoder->GetFrame(0, frame.GetAddressOf()));

    // 포멧 컨버터
    Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
    ThrowIfFailed(pWICFactory->CreateFormatConverter(converter.GetAddressOf()));

    hr = converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone,
        nullptr,
        0,
        WICBitmapPaletteTypeCustom);
    ThrowIfFailed(hr);

    // 변환된 데이터에서 비트맵 생성
    hr = mpRendertarget->CreateBitmapFromWicBitmap(
        converter.Get(),
        ppBitmap
    );
    ThrowIfFailed(hr);

    return S_OK;
}
