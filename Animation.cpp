#include "Animation.h"

Animation::Animation()
{
    m_animateSet = LoadTexture("");
    m_frameRec = Rectangle{ 0.0f, 0.0f, 0, 0 };
}

Animation::Animation(Texture2D tex, Rectangle frameRec, int frameSpeed, Vector2 pos, float rotation, int frameCount)
{
	m_animateSet = tex;
    m_frameRec = frameRec;
	m_currentFrame = 0;
	m_frameSpeed = frameSpeed;
	m_position = pos;
	m_frameCounter = 0;
    m_rotation = rotation;
    m_frameCount = frameCount;
}

void Animation::update()
{
    ++m_frameCounter;

    if (m_frameCounter >= (GetFPS() / m_frameSpeed))
    {
        m_frameCounter = 0;
        ++m_currentFrame;

        if (m_currentFrame >= m_frameCount) 
            m_currentFrame = 0;

        m_frameRec.x = (float)m_currentFrame * (float)m_animateSet.width / m_frameCount;
    }

    if (m_frameSpeed > MAX_FRAME_SPEED) m_frameSpeed = MAX_FRAME_SPEED;
    else if (m_frameSpeed < MIN_FRAME_SPEED) m_frameSpeed = MIN_FRAME_SPEED;
}

void Animation::draw()
{
    Rectangle destRec = { 
        m_position.x ,
        m_position.y, 
        m_frameRec.width, 
        m_frameRec.height 
    };
    Vector2 origin = { 
        m_frameRec.width,
        m_frameRec.height
    };
    DrawTexturePro(m_animateSet, m_frameRec, destRec, origin, m_rotation, WHITE);
}

void Animation::resetFrame()
{
	m_currentFrame = 0;
	m_frameCounter = 0;
}

void Animation::unload()
{
    UnloadTexture(m_animateSet);
}

void Animation::setAnimateSet(Texture2D anim)
{
    m_animateSet = anim;
}

void Animation::setPos(Vector2 pos)
{
    m_position = pos;
}

void Animation::setFrameRec(Rectangle rec)
{
    m_frameRec = rec;
}

void Animation::setFrameSpeed(int speed)
{
    m_frameSpeed = speed;
}

void Animation::setRotation(float rotation)
{
    m_rotation = rotation;
}

void Animation::setFrameCount(int count)
{
    m_frameCount = count;
}

Vector2 Animation::getPos()
{
	return m_position;
}

float Animation::getRotation()
{
    return m_rotation;
}

Rectangle Animation::getFrameRec()
{
    return m_frameRec;
}

std::pair<int, int> Animation::getSize()
{
    return std::pair<int,int>(m_animateSet.width, m_animateSet.height);
}
