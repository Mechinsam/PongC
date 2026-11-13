#include <stdio.h>
#include <stdint.h>
#include <MiniFB.h>
#include <stdlib.h>

const int WIDTH=800;
const int HEIGHT=600;
const int PLAYER_SPEED=600;
const int BALLSIZE=15;
const int PLAYER_X = 20;

const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 120;

const int MIN_BALL_SPEED = 300;
const int MAX_BALL_SPEED = 600;

typedef struct {
	int y;
	int score;
} Player;

typedef struct {
	int x;
	int y;

	int velX;
	int velY;
} Ball;

Player newPlayer()
{
	Player new;
	new.y = 100;
	new.score = 0;

	return new;
}

void setPixel(uint32_t* buffer, int x, int y)
{
	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0) {
		return;
	}

	buffer[y*WIDTH+x] = 0xFFFFFF;
}

void drawRect(uint32_t* buffer, int x, int y, int width, int height)
{
	for (int iy=y;iy<(y+height);iy++)
	{
		for (int ix=x;ix<(x+width);ix++)
		{
			setPixel(buffer, ix, iy);
		}
	}
}

void clearBuffer(uint32_t* buffer)
{
	for (size_t i=0;i<(WIDTH*HEIGHT);i++) {
		buffer[i] = 0x0;
	}
}

int main()
{
	struct mfb_window *window = mfb_open("Pong", 800, 600);
	if (window == NULL)
		return 0;

	uint32_t *buffer = malloc(800 * 600 * 4);
	const uint8_t* keys = mfb_get_key_buffer(window);

	Player p1 = newPlayer();
	Player p2 = newPlayer();

	Ball ball;
	ball.velX = 200;
	ball.velY = 100;
	ball.x = (WIDTH/2)-BALLSIZE;
	ball.y = (HEIGHT/2)-BALLSIZE;

	struct mfb_timer* clock = mfb_timer_create();
	double dt = 0.0;

	mfb_update_state state;
	do {
		// Game update
		if (keys[MFB_KB_KEY_W]) p1.y -= PLAYER_SPEED * dt;
		if (keys[MFB_KB_KEY_S]) p1.y += PLAYER_SPEED * dt;

		if (keys[MFB_KB_KEY_UP]) p2.y -= PLAYER_SPEED * dt;
		if (keys[MFB_KB_KEY_DOWN]) p2.y += (PLAYER_SPEED * dt);

		ball.x += ball.velX * dt;
		ball.y += ball.velY * dt;

		// ball coll
		if ((ball.x+BALLSIZE) > (WIDTH-PLAYER_X-PLAYER_WIDTH))
		{
			if ((ball.y+BALLSIZE) > p2.y && ball.y < (p2.y+PLAYER_HEIGHT))
			{
				ball.velX = -(rand() % (MAX_BALL_SPEED - MIN_BALL_SPEED+1)+MIN_BALL_SPEED);
				ball.velY = -(rand() % (MAX_BALL_SPEED - MIN_BALL_SPEED+1)+MIN_BALL_SPEED);
			}
			else {
				ball.x = (WIDTH/2)-BALLSIZE;
				ball.y = (HEIGHT/2)-BALLSIZE;

				ball.velX = -200;
				ball.velY = -100;

				p1.score++;
			}
		}
		if ((ball.x) < PLAYER_X+PLAYER_WIDTH)
		{
			if ((ball.y+BALLSIZE) > p1.y && ball.y < (p1.y+PLAYER_HEIGHT))
			{
				ball.velX = rand() % (MAX_BALL_SPEED - MIN_BALL_SPEED+1)+MIN_BALL_SPEED;
				ball.velY = rand() % (MAX_BALL_SPEED - MIN_BALL_SPEED+1)+MIN_BALL_SPEED;
			}
			else {
				ball.x = (WIDTH/2)-BALLSIZE;
				ball.y = (HEIGHT/2)-BALLSIZE;

				ball.velX = 200;
				ball.velY = 100;

				p2.score++;
			}
		}

		if (ball.y <= 0) {
			ball.velY = -(ball.velY);
		}
		if ((ball.y+BALLSIZE) >= HEIGHT) {
			ball.velY = -(ball.velY);
		}

		char title[128];
		snprintf(title, sizeof(title), "%d-%d", p1.score, p2.score);
		mfb_set_title(window, title);

		// Render
		clearBuffer(buffer);

		// p1
		drawRect(buffer, PLAYER_X, p1.y, PLAYER_WIDTH, PLAYER_HEIGHT);
		
		// p2
		drawRect(buffer, WIDTH-PLAYER_X-PLAYER_WIDTH, p2.y, PLAYER_WIDTH, PLAYER_HEIGHT);

		// ball
		drawRect(buffer, ball.x, ball.y, BALLSIZE, BALLSIZE);

		state = mfb_update_ex(window, buffer, 800, 600);

		if (state != MFB_STATE_OK)
			break;

		dt = mfb_timer_delta(clock);
		mfb_timer_reset(clock);

	} while(mfb_wait_sync(window));

	free(buffer);
	mfb_timer_destroy(clock);
	buffer = NULL;
	window = NULL;

	return 0;
}
