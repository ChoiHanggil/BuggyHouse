#pragma once

#include "D2DFramework.h"

class Actor
{
public:
	Actor() = delete;
	//Framework->mspWICFactory->Codec, Converter
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);
	virtual ~Actor();
	//IBitmap
	//x,y
	//opacity

	virtual void Draw();
public:
	void SetPosition(float x, float y) { mX = x, mY = y; };
	void SetPosition(D2D_VECTOR_2F& pos) { mX = pos.x, mY = pos.y; }
	D2D_VECTOR_2F GetPosition() const { return D2D_VECTOR_2F{ mX, mY }; }

protected:
	D2DFramework* mpFramework; //일반포인터 소유권X, 참조만 사용 use a
	ID2D1Bitmap* mpBitmap; // have a 
	
	float mX;
	float mY;
	float mOpacity;

private:
	void Draw(float x, float y, float opacity = 1.0f);
};

