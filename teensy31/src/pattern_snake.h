#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

static const int snakeCount = 6;
static const byte SNAKE_LENGTH = 16;

enum Direction {
	UP, DOWN, LEFT, RIGHT
};

struct Pixel_s {
	uint8_t x;
	uint8_t y;
};

struct Snake {
	Pixel_s pixels[SNAKE_LENGTH];

	Direction direction;

	void newDirection() {
		switch (direction) {
		case UP:
		case DOWN:
			direction = random(0, 2) == 1 ? RIGHT : LEFT;
			break;

		case LEFT:
		case RIGHT:
			direction = random(0, 2) == 1 ? DOWN : UP;

		default:
			break;
		}
	}

	void shuffleDown() {
		for (byte i = SNAKE_LENGTH - 1; i > 0; i--) {
			pixels[i] = pixels[i - 1];
		}
	}

	void reset() {
		direction = UP;
		for (int i = 0; i < SNAKE_LENGTH; i++) {
			pixels[i].x = 0;
			pixels[i].y = 0;
		}
	}

	void move() {
		switch (direction) {
		case UP:
			pixels[0].y = (pixels[0].y + 1) % MATRIX_HEIGHT;
			break;
		case LEFT:
			pixels[0].x = (pixels[0].x + 1) % MATRIX_WIDTH;
			break;
		case DOWN:
			pixels[0].y = pixels[0].y == 0 ? MATRIX_HEIGHT - 1 : pixels[0].y - 1;
			break;
		case RIGHT:
			pixels[0].x = pixels[0].x == 0 ? MATRIX_WIDTH - 1 : pixels[0].x - 1;
			break;
		}
	}

	void draw(CRGB colors[SNAKE_LENGTH],CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT]) {
		for (byte i = 0; i < SNAKE_LENGTH; i++) {
			Background_Array[pixels[i].x][pixels[i].y] = colors[i] %= (255 - i * (255 / SNAKE_LENGTH));
		}
	}
};

static Snake snakes[snakeCount];
static CRGB colors[SNAKE_LENGTH];
static uint8_t initialHue;

void pattern_snake_setup() {
	for (int i = 0; i < snakeCount; i++) {
		Snake* snake = &snakes[i];
		snake->reset();
	}
}

void pattern_snake_draw(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], const struct CRGBPalette16 *palette) {
	DimAll(250,Background_Array);

	fill_palette(colors, SNAKE_LENGTH, initialHue++, 5, *palette, 255, LINEARBLEND);

	for (int i = 0; i < snakeCount; i++) {
		Snake* snake = &snakes[i];

		snake->shuffleDown();

		if (random(10) > 7) {
			snake->newDirection();
		}

		snake->move();
		snake->draw(colors, Background_Array);
	}

	return;
}


