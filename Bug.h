#pragma once
#include "Actor.h"
class Bug : public Actor
{
	const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f }; //¥‹¿ß∫§≈Õ

	//enum class Direction
	//{
	//	UP,
	//	UP_RIGHT,
	//	RIGHT,
	//	DOWN_RIGHT,
	//	DOWN,
	//	DOWN_LEFT,
	//	LEFT,
	//	UP_LEFT,

	//	COUNT
	//};

	

public:
	Bug(D2DFramework* pFramework);
	virtual ~Bug();
		virtual void Draw() override;

		bool IsClicked(POINT& pt);
private:
	//Direction mDirection;
	float mRotation;
	float mSteps;
};

