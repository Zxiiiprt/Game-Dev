#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MARGIN 10

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

#define BALL_SIZE 15
#define BALL_X (WINDOW_WIDTH / 2 - BALL_SIZE / 2)
#define BALL_Y (WINDOW_HEIGHT / 2 - BALL_SIZE / 2)
#define BALL_VELOCITY 250

#define NET_WIDTH (WINDOW_WIDTH - 2 * MARGIN)
#define NET_HEIGHT 2.5
#define NET_X (WINDOW_WIDTH / 2 - NET_WIDTH / 2)
#define NET_Y (WINDOW_HEIGHT / 2 - NET_HEIGHT / 2)

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define PADDLE_X (WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2)
#define PADDLE1_Y (WINDOW_HEIGHT - PADDLE_HEIGHT - MARGIN)
#define PADDLE2_Y MARGIN
#define PADDLE_VELOCITY 300

#endif
