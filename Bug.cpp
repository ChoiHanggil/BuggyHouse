#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Images/bug.png")
{
	RECT rct;
	GetClientRect(mpFramework->GetWindowHandle(), &rct);

	mX = (float)(rand() % rct.right);
	mY = (float)(rand() % rct.bottom);

	mRotation = 0.0f;
	mSteps = 0.0f;
}

Bug::~Bug()
{

}

void Bug::Draw()
{
	//if (mSteps++ > 10.0f)
	//{
	//	mSteps = 0.0f;
	//	int dir = static_cast<int>(mDirection);
	//	int dir_count = static_cast<int>(Direction::COUNT);
	//	dir += (1 - rand() % 3);
	//	dir = (dir + dir_count) % dir_count;
	//	mDirection = static_cast<Direction>(dir);
	//}
	//switch(mDirection)
	//{
	//	case Direction::UP:
	//		mY--;
	//		break;
	//	case Direction::UP_RIGHT:
	//		mY--;
	//		mX++;
	//		break;
	//	case Direction::RIGHT:
	//		mX++;
	//		break;
	//	case Direction::DOWN_RIGHT:
	//		mY++;
	//		mX++;
	//		break;
	//	case Direction::DOWN:
	//		mY++;
	//		break;
	//	case Direction::DOWN_LEFT:
	//		mY++;
	//		mX--;
	//		break;
	//	case Direction::LEFT:
	//		mX--;
	//		break;
	//	case Direction::UP_LEFT:
	//		mY--;
	//		mX--;
	//		break;
	//}
	//Actor::Draw();

	if (mSteps++ > 30.0f)
	{
		mSteps = 0.0f;
		mRotation += (1 - rand() % 3) * 45.0f;
	}

	auto rotatedDir = UPVECTOR* D2D1::Matrix3x2F::Rotation(mRotation);
	mX += rotatedDir.x;
	mY += rotatedDir.y;

	//Transform
	auto size = mpBitmap->GetPixelSize();
	D2D1_RECT_F rect{
		0.0f, 0.0f,
		0.0f + size.width, 0.0f + size.height
	};

	//Translate
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX,mY);

	//Rotation
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation, D2D_POINT_2F{ size.width * 0.5f, size.height*0.5f });
	auto matTransfrom = matRotation * matTranslate;

	//Transform
	auto pRT = mpFramework->GetRenderTarget();
	pRT->SetTransform(matTransfrom);

	pRT->DrawBitmap(mpBitmap, rect, mOpacity);
}

bool Bug::IsClicked(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect{
		mX,mY,
		mX + size.width, mY+size.height
	};
	if (pt.x >= rect.left && pt.x <= rect.right &&
		pt.y >= rect.top && pt.y <= rect.bottom)
	{
		return true;
	}
	return false;
}
