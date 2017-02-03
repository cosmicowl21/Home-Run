#include "Player.h"

Player::Player(void)
{

	state = Player::State::Standing;
	facingRight = true;
	grounded = true;
	stateTime = 0;
	velocity = Point(0, 0);
	position = Point();

}

Player::~Player(void)
{
}

