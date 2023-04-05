#pragma once
#include <vector>
#include <iostream>
#include "raylib.h"

const int screenWidth = 288 * 1.9;
const int screenHeight = 512 * 1.9;

class Stolb
{
    std::vector<Rectangle> Ups;
    std::vector<Rectangle> Downs;

    int m_count = 100;
    float m_startPos = screenWidth;
    float m_S = 400;
    float stolbWidth = 100;
    float UpDownS = 300;
    float m_speed = 5;
    int m_upMin = screenHeight / 5;
    int m_upMax = screenHeight / 5 * 3;
    Color stolbColor = WHITE;
    Texture2D m_pipeUpSprite;
    Texture2D m_pipeDownSprite;
    
public:
    Stolb(Texture2D up, Texture2D down) {
        m_pipeUpSprite = up;
        m_pipeDownSprite = down;
        init();
    }
    
    void init(){
        Ups = std::vector<Rectangle>();
        Downs = std::vector<Rectangle>();

        for (int i = 0; i < m_count; ++i) {
            Rectangle r;
            if (i != 0) {
                float prevX = Ups.back().x;
                r = {
                    prevX + m_S,               //x
                    0,                            //y
                    stolbWidth,                   //width
                    float(rand() % (m_upMax - m_upMin) + m_upMin) //height
                };
            }
            else {
                r = {
                    m_startPos ,               //x
                    0,                            //y
                    stolbWidth,                   //width
                    float(rand() % (m_upMax - m_upMin) + m_upMin) //height
                };
            }
            Ups.push_back(r);
        }
        for (int i = 0; i < m_count; ++i) {
            Rectangle r = {
                Ups[i].x ,                                //x
                Ups[i].y + Ups[i].height + UpDownS,       //y
                stolbWidth,                               //width
                screenHeight - (Ups[i].height + UpDownS)  //height
            };
            Downs.push_back(r);
        }
    }
    
    void draw(bool isHitboxed) {
        for (int i = 0; i < m_count; ++i) {
            DrawTextureEx(m_pipeUpSprite, Vector2{ Ups[i].x,(Ups[i].height - m_pipeUpSprite.height)}, 0, 1, WHITE);
            Ups[i].x -= m_speed;

            DrawTextureEx(m_pipeDownSprite, Vector2{ Downs[i].x, Downs[i].y }, 0, 1, WHITE);   
            Downs[i].x -= m_speed;

            //Draw Hitbox
            if (isHitboxed) {
                DrawRectangleRec(Ups[i], Color{ 1, 0, 0, 200 });
                DrawRectangleRec(Downs[i], Color{ 1, 0, 0, 200 });
            }
        }
    }
    
    void reset() {
        init();
    }

    auto getUpHitbox() {
        return Ups;
    }
    auto getDownHitbox() {
        return Downs;
    }
};

