#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{ 38,185,154,255 };
Color Dark_Green = Color{ 20,160,133,255 };
Color Light_Green = Color{ 129,204,184,255 };
Color Yellow = Color{ 243,213,91,255 };

int player_Score = 0;
int CPU_Score = 0;
float playerScoreTime = 0;
float CPUScoreTime = 0;
const float SCORE_DISPLAY_TIME = 1.0f;

class Ball {
public:
    float x, y;
    int Speed_x, Speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update() {
        x += Speed_x;
        y += Speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            Speed_y *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            CPU_Score++;
            CPUScoreTime = GetTime();
            ResetBall();
        }

        if (x - radius <= 0) {
            player_Score++;
            playerScoreTime = GetTime();
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_Choices[2] = { -1, 1 };
        Speed_x *= speed_Choices[GetRandomValue(0, 1)];
        Speed_y *= speed_Choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + Height >= GetScreenHeight()) {
            y = GetScreenHeight() - Height;
        }
    }

public:
    float x, y;
    float width, Height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, Height }, 0.8, 0, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CPUPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + Height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + Height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle Player;
CPUPaddle cpu;

int main() {
    cout << "Starting The Game" << endl;

    const int width = 1280;
    const int height = 800;

    InitWindow(width, height, "My Pong Game");
    SetTargetFPS(60);

    // Setting Ball Variables
    ball.radius = 20;
    ball.x = width / 2;
    ball.y = height / 2;
    ball.Speed_x = 7;
    ball.Speed_y = 7;

    // Setting PlayerPaddle Variables
    Player.width = 25;
    Player.Height = 120;
    Player.x = width - Player.width - 10;
    Player.y = height / 2 - Player.Height / 2;
    Player.speed = 6;

    // Setting CPU Paddle Variables
    cpu.Height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = height / 2 - cpu.Height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false) {
        BeginDrawing();

        // Updating
        ball.Update();
        Player.Update();
        cpu.Update(ball.y);

        // Checking For Collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ Player.x, Player.y, Player.width, Player.Height })) {
            ball.Speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.Height })) {
            ball.Speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(width / 2, 0, width / 2, height, Green);
        DrawCircle(width / 2, height / 2, 150, Light_Green);
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        ball.Draw();
        cpu.Draw();
        Player.Draw();
        DrawText(TextFormat("%i", CPU_Score), width / 4, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_Score), 3 * width / 4, 20, 80, WHITE);

        // Display Score Messages
        if (GetTime() - playerScoreTime < SCORE_DISPLAY_TIME && playerScoreTime != 0) {
            const char* text = "Player Scored!";
            int fontSize = 40;
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
            DrawText(text,
                (width - textSize.x) / 2,
                (height - textSize.y) / 2,
                fontSize,
                WHITE);
        }

        if (GetTime() - CPUScoreTime < SCORE_DISPLAY_TIME && CPUScoreTime != 0) {
            const char* text = "CPU Scored!";
            int fontSize = 40;
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
            DrawText(text,
                (width - textSize.x) / 2,
                (height - textSize.y) / 2,
                fontSize,
                WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}