#include "raylib.h"
#include <vector>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Meu Primeiro Jogo com Raylib");
    SetTargetFPS(60);

    // Jogador
    Rectangle player = { 100, screenHeight - 70, 40, 40 };
    float velocityY = 0.0f;          // NOVO
    float jumpImpulse = -600.0f;
    float gravity = 1500.0f;
    bool isOnGround = true;

    // Obstáculos
    std::vector<Rectangle> obstacles;
    float obstacleSpeed = 300.0f;
    float spawnTimer = 0.0f;
    float spawnInterval = 1.5f;

    int score = 0;
    bool gameOver = false;

    while (!WindowShouldClose()) {
        if (!gameOver) {
            float dt = GetFrameTime();

            // --- Pulo ---
            if (IsKeyPressed(KEY_SPACE) && isOnGround) {
                velocityY = jumpImpulse;
                isOnGround = false;
            }

            // --- Física ---
            velocityY += gravity * dt;
            player.y += velocityY * dt;

            // --- Chão ---
            if (player.y >= screenHeight - 70) {
                player.y = screenHeight - 70;
                velocityY = 0.0f;
                isOnGround = true;
            }

            // --- Obstáculos ---
            spawnTimer += dt;
            if (spawnTimer >= spawnInterval) {
                Rectangle newObstacle = {
                    (float)screenWidth, screenHeight - 70,
                    (float)GetRandomValue(20, 50), 40
                };
                obstacles.push_back(newObstacle);
                spawnTimer = 0.0f;
                spawnInterval = (float)GetRandomValue(10, 20) / 10.0f;
            }

            for (int i = 0; i < (int)obstacles.size(); i++) {
                obstacles[i].x -= obstacleSpeed * dt;

                if (obstacles[i].x + obstacles[i].width < 0) {
                    obstacles.erase(obstacles.begin() + i);
                    score += 10;
                    i--;
                } else if (CheckCollisionRecs(player, obstacles[i])) {
                    gameOver = true;
                }
            }
        } else {
            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                score = 0;
                obstacles.clear();
                player.y = screenHeight - 70;
                velocityY = 0.0f;        // ← também zera aqui ao reiniciar
                isOnGround = true;
            }
        }

        // --- Draw ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(player, BLUE);
        for (auto& obs : obstacles) DrawRectangleRec(obs, RED);

        DrawText(TextFormat("Score: %i", score), 10, 10, 20, DARKGRAY);

        if (gameOver) {
            DrawText("GAME OVER! Pressione R para reiniciar",
                     screenWidth/2 - 200, screenHeight/2, 20, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}