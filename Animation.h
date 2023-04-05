#pragma once
#include "raylib.h"
#include <iostream>

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

class Animation
{
private:
	Texture2D m_animateSet;
	Rectangle m_frameRec;
	int m_currentFrame = 0;
	int m_frameSpeed = 10;
	Vector2 m_position = Vector2{0, 0};
	int m_frameCounter = 0;
	float m_rotation = 0;
	int m_frameCount = 0;

public:
	Animation();
	Animation(Texture2D tex, Rectangle frameRec, int frameSpeed, Vector2 pos, float rotation, int frameCount);
	void update();
	void draw();
	void unload();
	void resetFrame();

	void setAnimateSet(Texture2D anim);
	void setPos(Vector2 pos);
	void setFrameRec(Rectangle rec);
	void setFrameSpeed(int speed);
	void setRotation(float rotation);
	void setFrameCount(int count);

	Vector2 getPos();
	float getRotation();
	std::pair<int, int> getSize();
	Rectangle getFrameRec();
};

