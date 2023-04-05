#pragma once
#include "raylib.h"
#include "Animation.h"

const int startFallSpeed = 7;
const int startUpSpeed = 17;
const int maxUpFrameCount = 10;

class Player {
private:
    Vector2 m_position;
    Rectangle m_hitbox;
    Animation m_animation;
    float m_rotation;

    float m_fallingSpeed = startFallSpeed;
    int m_upSpeed = startUpSpeed;
    int m_pointCount = 0;
    int m_upFrameCounter = 0;
    bool m_isUp = true;
    int m_upFrameMax = maxUpFrameCount;

    Vector2 m_startPos;

public:
    Player(Vector2 pos, Animation anim, float rotation) {
        m_position = pos;
        m_hitbox = Rectangle{pos.x, pos.y, anim.getFrameRec().width, anim.getFrameRec().height};
        m_animation = anim;
        m_rotation = rotation;
        m_startPos = pos;
    }
    
    float getFallSpeed() { return m_fallingSpeed; }
    int getUpSpeed() { return m_upSpeed; }
    int getPoint() { return m_pointCount; }
    int getUpFrameCounter() { return m_upFrameCounter; }
    bool isUp() { return m_isUp; }
    int getUpFrameMax() { return m_upFrameMax; }
    float getPos_x() { return m_position.x; }
    float getPos_y() { return m_position.y; }
    Vector2 getPos() { return m_position; }
    float getRotation() { return m_rotation; }
    auto getHitbox() { return m_hitbox; }

    void resetFallSpeed() { m_fallingSpeed = startFallSpeed; }
    void resetUpSpeed() { m_upSpeed = startUpSpeed; }
    void resetUpFrameCount() { m_upFrameMax = maxUpFrameCount; }

    void update() {
        m_animation.setRotation(m_rotation);
        m_animation.setPos(m_position);
        m_animation.update();
    }

    void draw(bool isHitboxed) {
        m_animation.draw();
        Vector2 origin = {
            (float)m_animation.getFrameRec().width,
            (float)m_animation.getFrameRec().height
        };

        //Draw Hitbox
        if(isHitboxed)
            DrawRectanglePro(m_hitbox, origin, m_rotation, Color{1, 0, 0, 200});
    }
    
    void reset() {
        m_position = m_startPos;
        m_hitbox = Rectangle{
            m_startPos.x, m_startPos.y, 
            m_animation.getFrameRec().width, m_animation.getFrameRec().height
        };
        m_rotation = 0;
        m_fallingSpeed = startFallSpeed;
        m_upSpeed = startUpSpeed;
        m_pointCount = 0;
        m_upFrameCounter = 0;
        m_isUp = true;
        m_upFrameMax = maxUpFrameCount;
    }

    void setPos_x(float n) {
        m_position.x = n; 
        m_hitbox.x = n;
    }
    
    void setPos_y(float n) {
        m_position.y = n; 
        m_hitbox.y = n;
    }

    void setRotation(float rot) {
        m_rotation = rot;
    }
    
    void setFallingSpeed(float s) {
        m_fallingSpeed = s;
    }
    
    void setUpSpeed(int s) {
        m_upSpeed = s;
    }
    
    void setPointCount(int n) {
        m_pointCount = n;
    }
    
    void setUpFrameCounter(int c) {
        m_upFrameCounter = c;
    }
    
    void setIsUp(bool e) {
        m_isUp = e;
    }
    
    void setUpFrameMax(int n) {
        m_upFrameMax = n;
    }

    void setAnimation(Animation anim) {
        m_animation = anim; 
    }
    
    ~Player() {
        m_animation.unload();
    }
};

