
#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class Player : public Object
{

public:
	//FIELDS
	// state for each type of movement
	enum State 
	{
		Standing, Walking, Jumping
	};

	State state;

	// boolean to say if were on the ground or if were facing to the right
	// oppisite is false
	bool facingRight;
	bool grounded;
	float stateTime;

	Point position;
	Point velocity;

	Player(void);
	virtual ~Player(void);
};
