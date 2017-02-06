#pragma once
#include "cocos2d.h"
using namespace cocos2d;

// KEYBOARD INPUTS	
const EventKeyboard::KeyCode ENTER = EventKeyboard::KeyCode::KEY_KP_ENTER;
const EventKeyboard::KeyCode UP_ARROW = EventKeyboard::KeyCode::KEY_UP_ARROW;
const EventKeyboard::KeyCode DOWN_ARROW = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;

const float SCALE_FACTOR = 2.0f;
const float PLAYER_MAX_VELOCITY = 5.0f;
const float PLAYER_JUMP_VELOCITY = 40.0f;
const float DAMPING = 0.87f;