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
   //map�� �ش� ���ϸ�(Ű)�� �ִ���?
   //�ִٸ� �ش� ��Ʈ��(����)�� ��ȯ
   // ���ٸ� �̹��� �ε� �� �ʿ� �߰�

    //auto result = myMap.insert(<"1.png", nullptr>);
    //if (result.second)
    //{
    //    //ó���߰�
    //}
    //else
    //{
    //    //�̹� ����
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

    // ���ڴ� ����
    hr = pWICFactory->CreateDecoderFromFilename(
        filename.c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        bitmapDecoder.GetAddressOf());
    ThrowIfFailed(hr);

    // ������ ȹ��
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
    ThrowIfFailed(bitmapDecoder->GetFrame(0, frame.GetAddressOf()));

    // ���� ������
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

    // ��ȯ�� �����Ϳ��� ��Ʈ�� ����
    hr = mpRendertarget->CreateBitmapFromWicBitmap(
        converter.Get(),
        ppBitmap
    );
    ThrowIfFailed(hr);

    return S_OK;
}
