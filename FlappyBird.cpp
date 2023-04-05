#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "raylib.h"
#include "Animation.h"
#include "Player.h"
#include "Pipe.h"
#include "Stolb.h"
#include "GameFunctions.h"
#include "Audio.h"

enum WindowScreen {
    MENU,
    GET_READY,
    GAME,
    GAME_OVER,
};

int main(void)
{
    const int screenWidth = 288 * 1.9;
    const int screenHeight = 512 * 1.9;

    InitWindow(screenWidth, screenHeight, "Flappy Bird");

    SetTargetFPS(60);
    //E:/Programming/C++/OpenGL/RayLib Projects/FlappyBird/x64/Debug
    std::string resPath = "./res";
    const int gravity = 10;
    WindowScreen currScreen = GET_READY;
    InitAudioDevice();              // Initialize audio device
    AttachAudioMixedProcessor(ProcessAudio);

    //Sprites
    std::vector<Texture2D> birdSprites = {
        LoadTexture((resPath + "/sprites/red_bird.png").c_str()),
        LoadTexture((resPath + "/sprites/yellow_bird.png").c_str()),
        LoadTexture((resPath + "/sprites/blue_bird.png").c_str()),
    };
    std::vector<Texture2D> backgrounds = {
        LoadTexture((resPath + "/sprites/background_day.png").c_str()),
        LoadTexture((resPath + "/sprites/background_night.png").c_str()),
    };
    std::vector<Texture2D> numSprites = {
        LoadTexture((resPath + "/sprites/0.png").c_str()),
        LoadTexture((resPath + "/sprites/1.png").c_str()),
        LoadTexture((resPath + "/sprites/2.png").c_str()),
        LoadTexture((resPath + "/sprites/3.png").c_str()),
        LoadTexture((resPath + "/sprites/4.png").c_str()),
        LoadTexture((resPath + "/sprites/5.png").c_str()),
        LoadTexture((resPath + "/sprites/6.png").c_str()),
        LoadTexture((resPath + "/sprites/7.png").c_str()),
        LoadTexture((resPath + "/sprites/8.png").c_str()),
        LoadTexture((resPath + "/sprites/9.png").c_str()),
    };
    std::vector<Texture2D> messages = {
        LoadTexture((resPath + "/sprites/get_ready.png").c_str()),
        LoadTexture((resPath + "/sprites/game_over.png").c_str()),
    };
    std::vector<Texture2D> pipeSprites = {
        LoadTexture((resPath + "/sprites/pipe_up_green.png").c_str()),
        LoadTexture((resPath + "/sprites/pipe_up_red.png").c_str()),
        LoadTexture((resPath + "/sprites/pipe_down_green.png").c_str()),
        LoadTexture((resPath + "/sprites/pipe_down_red.png").c_str()),
    };
    Sound scoreSound = LoadSound((resPath + "/audio/score.ogg").c_str());
    Sound upSound = LoadSound((resPath + "/audio/up.ogg").c_str());
    Sound flapSound = LoadSound((resPath + "/audio/down.ogg").c_str());
    Sound dieSound = LoadSound((resPath + "/audio/die.ogg").c_str());
    Sound hitSound = LoadSound((resPath + "/audio/hit.ogg").c_str());
   
    //Init All Values
    
    Texture2D background = backgrounds[random(0, backgrounds.size())];
    Texture2D birdSprite = birdSprites[random(0, birdSprites.size())];
    int pipeVecPos = random(0, 2);
    Texture2D pipeUpSprite = pipeSprites[pipeVecPos];
    Texture2D pipeDownSprite = pipeSprites[pipeVecPos + 2];
    Animation birdAnimation = Animation(
        birdSprite,
        Rectangle{ 0.0f,0.0f,float(birdSprite.width / 3), float(birdSprite.height) },
        8,
        { screenWidth / 2, screenHeight / 2 },
        0,3
    );
    Player bird = Player(
        Vector2{ screenWidth / 3, screenHeight/2 - birdAnimation.getFrameRec().height/2},
        birdAnimation, 30
    );
    Stolb pipe = Stolb(pipeUpSprite,pipeDownSprite);
    std::vector<int> scores;

    while (!WindowShouldClose())
    {
        //Update
        bird.update();
        updateAudioDevice();

        BeginDrawing();
        ClearBackground(GREEN);
        
        switch (currScreen) {
        case GET_READY: {
            float backgroundScale = std::max(
                screenWidth / float(background.width),
                float(screenHeight / background.height)
            );
            float getReadyScale = 1.5f;
            Vector2 getReadyPos = {
                screenWidth / 2 - messages[0].width / 2 * getReadyScale,
                screenHeight / 2 - messages[0].height / 2 * getReadyScale,
            };
            DrawTextureEx(background, Vector2{ 0, 0 }, 0, backgroundScale, WHITE);
            DrawTextureEx(messages[0], getReadyPos, 0, getReadyScale, WHITE);

            if (IsKeyPressed(KEY_SPACE))
                currScreen = GAME;
        }
            break;

        case GAME: {
            //Background Drawing
            float backgroundScale = std::max(
                screenWidth / float(background.width),
                float(screenHeight / background.height)
            );
            DrawTextureEx(background, Vector2{ 0, 0 }, 0, backgroundScale, WHITE);

            //Bird Drawing
            bird.draw(false);
            PlaySound(flapSound);

            if (bird.getUpFrameCounter() == 0) {
                bird.setIsUp(IsKeyPressed(KEY_SPACE));
            }
            
            if (bird.getUpFrameCounter() < bird.getUpFrameMax() and bird.isUp()) {
                PlaySound(upSound);
                bird.setPos_y(bird.getPos_y() - bird.getUpSpeed());      //UP Vel
                bird.setUpFrameCounter(bird.getUpFrameCounter() + 1);
                bird.resetFallSpeed();
                if (bird.getRotation() >= -10.0f)
                    bird.setRotation(bird.getRotation() - 45.0f / bird.getUpFrameMax());
            }
            else {
                bird.setIsUp(false);
                bird.setUpFrameCounter(0);
                bird.setPos_y(bird.getPos_y() + bird.getFallSpeed());
                bird.setFallingSpeed(bird.getFallSpeed() + 0.1f);  //DOWN Vel
                if(bird.getRotation() <= 10.0f)
                    bird.setRotation(bird.getRotation() + 45.0f / bird.getUpFrameMax());
            }
            
            //Pipes Drawing
            pipe.draw(false);

            //Collision Checking and Calculating Point msg
            Rectangle birdHitbox = bird.getHitbox();
            auto upsHitbox = pipe.getUpHitbox();
            auto downsHitbox = pipe.getDownHitbox();
            int size = std::min(upsHitbox.size(), downsHitbox.size());

            for (int i = 0; i < size; ++i) {
                bool UpCollision = CheckCollisionRecs(birdHitbox, upsHitbox[i]);
                bool DownCollision = CheckCollisionRecs(birdHitbox, downsHitbox[i]);

                if (UpCollision or DownCollision or bird.getPos_y() <= 0 or bird.getPos_y() >= screenHeight) {
                    PlaySound(hitSound);
                    PlaySound(dieSound);
                    currScreen = GAME_OVER;
                    scores.push_back(bird.getPoint());
                }

                if (upsHitbox[i].x - 1 <= bird.getPos_x() and upsHitbox[i].x + 1 >= bird.getPos_x()) {
                    bird.setPointCount(bird.getPoint() + 1);
                    PlaySound(scoreSound);
                }
            }

            //Point Drawing
            int point = bird.getPoint();
            int zhuzdik = point / 100;
            int ondik = (point % 100) / 10;
            int birlik = point % 10;
            DrawTexture(numSprites[zhuzdik], 0, 0, WHITE);
            DrawTexture(numSprites[ondik], 25, 0, WHITE);
            DrawTexture(numSprites[birlik], 50, 0, WHITE);

            //FPS Drawing
            DrawText( ("FPS : " + std::to_string(GetFPS())).c_str(), 100, 0, 35, WHITE);
        }
            break;
        
        case GAME_OVER: {
            //Background Drawing
            float backgroundScale = std::max(
                screenWidth / float(background.width),
                float(screenHeight / background.height)
            );
            DrawTextureEx(background, Vector2{ 0, 0 }, 0, backgroundScale, WHITE);
            Vector2 GameOverPos = {
                screenWidth / 2 - messages[1].width / 2,
                screenHeight / 2 - messages[1].height / 2,
            };
            DrawTextureEx(messages[1], GameOverPos, 0, 1, WHITE);
            DrawText(("Score : " + std::to_string(bird.getPoint())).c_str(), GameOverPos.x, GameOverPos.y + 50, 35, BLACK);
            int highScoreElem = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
            DrawText(("High Score : " + std::to_string(scores[highScoreElem])).c_str(), GameOverPos.x, GameOverPos.y + 100, 35, BLACK);
            DrawText("Press R to replay", GameOverPos.x, GameOverPos.y + 150, 35, BLACK);

            if (IsKeyPressed(KEY_R)) {
                bird.reset();
                pipe.reset();
                background = backgrounds[random(0, backgrounds.size())];
                birdSprite = birdSprites[random(0, birdSprites.size())];
                pipeVecPos = random(0, 2);
                pipeUpSprite = pipeSprites[pipeVecPos];
                pipeDownSprite = pipeSprites[pipeVecPos + 2];
                birdAnimation.setAnimateSet(birdSprite);
                bird.setAnimation(birdAnimation);
                pipe = Stolb(pipeUpSprite, pipeDownSprite);
                currScreen = GET_READY;
            }
        }
            break;
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
