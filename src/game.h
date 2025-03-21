#pragma once
#include <cmath>
#include <vector>

#include "SDL2/SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2 {
    float x;
    float y;
};
struct Ball {
    Vector2 pos;
    Vector2 vel;
};

// Game class
class Game {
   public:
    Game();
    // 　ゲームの初期化
    bool Initialize();
    // ゲームのメインループ
    void RunLoop();
    // ゲームのシャットダウン
    void Shutdown();

   private:
    // ゲームループのためのヘルパー関数
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // SDLが作るウィンドウ
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;
    // ゲームの続行を指示するフラグ
    bool mIsRunning;

    // Pong specific
    // Direction of paddle
    int mPaddleDir;
    int mPaddleDir2;

    // Position of paddle
    Vector2 mPaddlePos;
    Vector2 mPaddlePos2;
    std::vector<Ball> mBalls;
};
