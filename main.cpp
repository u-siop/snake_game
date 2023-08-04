#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque) {
	for (unsigned int i = 0; i < deque.size(); i++) {
		if (Vector2Equals(deque[i], element)) {
			return true;
		}
	}
	return false;
}

bool eventTriggered(double interval) {
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval) {
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake {
public:
	deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
	Vector2 direction = { 1, 0 };
	bool addSegment = false;
	bool directionSegment = false;

	void Draw() {
		for (unsigned i = 0; i < body.size(); i++) {
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	void Update() {
		body.push_front(Vector2Add(body[0], direction));
		directionSegment = false;
		if (addSegment == true) {
			addSegment = false;
		}
		else {
			body.pop_back();
		}
	}

	void Reset() {
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		direction = { 1,0 };
	}
};

class Food {

public:
	Vector2 position;
	Texture2D texture;

	Food(deque<Vector2> snakeBody) {
		Image image = LoadImage("food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food() {
		UnloadTexture(texture);
	}

	void Draw() {
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x,y };
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody)) {
			position = GenerateRandomCell();
		}
		return position;
	}
};

class Game {
public :
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;
	Sound eatSound;
	Sound wallSound;

	Game() {
		InitAudioDevice();
		eatSound = LoadSound("eat.mp3");
		wallSound = LoadSound("wall.mp3");
	}

	~Game() {
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}

	void Draw() {
		food.Draw();
		snake.Draw();
	}

	void Update() {
		if (running) {
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}

	void GetInputKey() {
		if (snake.directionSegment == false) {
			if (IsKeyPressed(KEY_UP) && snake.direction.y != 1) {
				snake.directionSegment = true;
				snake.direction = { 0, -1 };
			}
			if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1) {
				snake.directionSegment = true;
				snake.direction = { 0, 1 };
			}
			if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1) {
				snake.directionSegment = true;
				snake.direction = { -1, 0 };
			}
			if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1) {
				snake.directionSegment = true;
				snake.direction = { 1, 0 };
			}
		}
	}

	void CheckCollisionWithFood() {
		if (Vector2Equals(snake.body[0], food.position)) {
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
			PlaySound(eatSound);
		}
	}

	void CheckCollisionWithEdges() {
		if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1) {
			GameOver();
		}
	}

	void CheckCollisionWithTail() {
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if(ElementInDeque(snake.body[0], headlessBody)){
			GameOver();
		}
	}

	void GameOver() {
		running = false;
		score = 0;
		PlaySound(wallSound);
	}

	void GameRestart() {
		running = true;
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		PlaySound(eatSound);
	}
};

int main(){
	cout << "Starting the game... " << endl;
	InitWindow(2*offset + cellSize * cellCount, 2*offset + cellSize * cellCount, "Retro Snake");
	SetTargetFPS(60);

	Game game = Game();
	
	while (WindowShouldClose() == false) {
		BeginDrawing();

		if (eventTriggered(0.2)) {
			game.Update();
		}
		game.GetInputKey();
		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
		DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
		DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
		game.Draw();
		if (!game.running) {
			DrawText("GAME OVER", 150, 200, 100, darkGreen);
			DrawText("IF YOU WANT TO CONTINUE", 80, 600, 50, darkGreen);
			DrawText("PRESS SPACE", 260, 680, 50, darkGreen);
			if (IsKeyPressed(KEY_SPACE)) {
				game.GameRestart();
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}