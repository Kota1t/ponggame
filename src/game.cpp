#include "game.h"

#include <cstdlib>
const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mPaddleDir(0),
      mPaddleDir2(0) {}

bool Game::Initialize() {
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("SDLを初期化できません: %s", SDL_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow =
        SDL_CreateWindow("Game Programming in C++ (Chapter 1)",  // タイトル
                         100,                                    // 左上のX座標
                         100,                                    // 左上のY座標
                         1024,  // ウィンドウの幅
                         768,   // ウィンドウの高さ
                         0      // フラグ
        );

    if (!mWindow) {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
        return false;
    }

    //// Create SDL renderer
    mRenderer = SDL_CreateRenderer(
        mWindow,  // 作成するレンダラーの描画対象となるウィンドウ
        -1,       // 通常は-1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    //
    mBalls.clear();
    Ball ball1;
    ball1.pos = {1024.0f / 2.0f, 768.0f / 2.0f};
    ball1.vel = {-200.0f, 235.0f};
    mBalls.emplace_back(ball1);

    Ball ball2;
    ball2.pos = {1024.0f / 2.0f, 768.0f / 2.0f};
    ball2.vel = {200.0f, 235.0f};
    mBalls.emplace_back(ball2);

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mPaddlePos2.x = 1024.0f - 10.0f;
    mPaddlePos2.y = 768.0f / 2.0f;
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    // キューにイベントがある限り処理を続ける
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // sdlの終了イベントが発生した場合
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    // キーボードの状態を取得
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // ESCキーが押された場合、ゲームを終了する
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // Update paddle direction based on W/S keys
    mPaddleDir = 0;
    mPaddleDir2 = 0;
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
    if (state[SDL_SCANCODE_I]) {
        mPaddleDir2 -= 1;
    }
    if (state[SDL_SCANCODE_K]) {
        mPaddleDir2 += 1;
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();

    // 左パドルの移動
    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        } else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }
    // 右パドルの移動
    if (mPaddleDir2 != 0) {
        mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
        if (mPaddlePos2.y < (paddleH / 2.0f + thickness)) {
            mPaddlePos2.y = paddleH / 2.0f + thickness;
        } else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness)) {
            mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    // 各ボールの更新
    for (int i = mBalls.size() - 1; i >= 0; i--) {
        Ball& ball = mBalls[i];
        ball.pos.x += ball.vel.x * deltaTime;
        ball.pos.y += ball.vel.y * deltaTime;

        // 左パドルとの衝突判定
        float diff1 = fabs(mPaddlePos.y - ball.pos.y);
        if (diff1 <= paddleH / 2.0f && ball.pos.x <= (25.0f + thickness) &&
            ball.pos.x > 0.0f) {
            ball.vel.x *= -1.0f;
        }

        // 右パドルとの衝突判定
        float diff2 = fabs(mPaddlePos2.y - ball.pos.y);
        if (diff2 <= paddleH / 2.0f &&
            ball.pos.x >= (1024.0f - 25.0f - thickness) &&
            ball.pos.x < 1024.0f) {
            ball.vel.x *= -1.0f;
        }
        // 上下壁での反射
        if (ball.pos.y <= thickness && ball.vel.y < 0.0f) {
            ball.vel.y *= -1;
        } else if (ball.pos.y >= (768.0f - thickness) && ball.vel.y > 0.0f) {
            ball.vel.y *= -1;
        }
        // ボールが画面外に出た場合は削除
        if (ball.pos.x < 0.0f || ball.pos.x > 1024.0f) {
            mBalls.erase(mBalls.begin() + i);
        }
        if (mBalls.empty()) {
            mIsRunning = false;
        }
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect wall{0, 0, 1024, thickness};
    SDL_RenderFillRect(mRenderer, &wall);

    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);

    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 768;
    SDL_RenderFillRect(mRenderer, &wall);

    wall.x = 0;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 768;
    SDL_RenderFillRect(mRenderer, &wall);

    // 左側のパドル
    SDL_Rect leftPaddle{static_cast<int>(mPaddlePos.x),
                        static_cast<int>(mPaddlePos.y - paddleH / 2), thickness,
                        static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &leftPaddle);

    // 右側のパドル（右端に合わせるためx座標は mPaddlePos2.x - thickness）
    SDL_Rect rightPaddle{static_cast<int>(mPaddlePos2.x - thickness),
                         static_cast<int>(mPaddlePos2.y - paddleH / 2),
                         thickness, static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &rightPaddle);

    // ボールの描画
    for (const auto& ball : mBalls) {
        SDL_Rect ballRect{static_cast<int>(ball.pos.x - thickness / 2),
                          static_cast<int>(ball.pos.y - thickness / 2),
                          thickness, thickness};
        SDL_RenderFillRect(mRenderer, &ballRect);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Game game;
    bool success = game.Initialize();
    if (success) {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
