#include "constants.h" //Not in angled brackets since we're just grabbing this as a file from the same folder, not from a system directory
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Declarations
//////////////////////////////////////////////////////////////////////

// Global Variables of our System
static SDL_Window *window = NULL;     // Address of the SDL Window as a global variable
static SDL_Renderer *renderer = NULL; // Address of the SDL Renderer
static int game_is_running = FALSE;   // Flag to notify if game is running or not, quits the game loop if false
static int last_frame_time = 0;       // Keep track of the last frame we call (in milliseconds)
static int served = FALSE;            // Initially the ball is not served

// Declaring struct of our Game objects
typedef struct object
{
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
    char *name;
} object_t;

// Declare game objects
static object_t ball_g;
static object_t paddle1_g;
static object_t paddle2_g;
static object_t net_g;
static object_t otherBall_g;

// Declaring struct Players of our Game
typedef struct player
{
    int score;
    int number;
    // object_t PAD;
} player_t;

// Declare Players (2 player)
player_t player1;
player_t player2;

//////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////

// Deciding which direction the ball would move at start - CURRENTLY USED
int coin_flip(void)
{
    return rand() % 2 == 1 ? TRUE : FALSE;
}

// Creates the Players of our game - CURRENTLY USED
player_t create_player(void)
{
    player_t player = {
        player.score = 0,
        // player.number = num
        // player.PAD = setPaddle(&paddle, height)
    };
    return player;
}

// Create a ball object - CURRENTLY UNUSED
object_t create_ball(float size)
{
    printf("Setting other ball object to:\n");
    object_t b = {b.x = WINDOW_WIDTH / 2 - size,
                  b.y = WINDOW_HEIGHT / 2 - size,
                  b.width = size,
                  b.height = size,
                  b.vel_x = BALL_VELOCITY * (coin_flip() ? 1 : -1),
                  b.vel_y = BALL_VELOCITY * (coin_flip() ? 1 : -1),
                  b.name = "Other Ball"};
    return b;
}

/**Will come back to this idea sooner or later
object initPaddle(object *paddle, int y) {
    // Initialize paddle1 position
    paddle->width = PADDLE_WIDTH;
    paddle->height = PADDLE_HEIGHT;
    paddle->x = (WINDOW_WIDTH/2)-(paddle->width/2);		//center of the screen
    paddle->y = y;										//for the classic pong setting
    paddle->vel_x = 0;
    paddle->vel_y = 0;
    return paddle;
}**/

// Initialize our Game objects at the start of the game - CURRENTLY USED
void init_object(object_t *obj, float x, float y, float width, float height, float vx, float vy, char *name)
{
    obj->x = x;
    obj->y = y;
    obj->width = width;
    obj->height = height;
    obj->vel_x = vx;
    obj->vel_y = vy;
    obj->name = name;
}

//////////////////////////////////////////////////////////////////////
// WORK IN PROGRESS BELOW CHILL OUT
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// WORK IN PROGRESS ABOVE CHILL OUT
//////////////////////////////////////////////////////////////////////

// Update Player scores - CURRENTLY USED
void update_score(int player, int points)
{
    served = FALSE;
    if (player == 1)
    {
        // served = FALSE;
        player1.score += points;
        // served = FALSE;
    }

    if (player == 2)
    {
        // served = FALSE;
        player2.score += points;
        // served = FALSE;
    }

    char *scoreBoard = "PLAYER1 SCORE : %d				|				PLAYER2 SCORE : %d\n";
    int len = snprintf(NULL, 0, scoreBoard, player1.score, player2.score);
    char buffer[len + 1];
    snprintf(buffer, len + 1, scoreBoard, player1.score, player2.score);
    SDL_SetWindowTitle(window, buffer);
}

// Update the position of the Ball object - CURRENTLY USED
void update_ball(object_t *b, float time)
{
    if (!served)
    {
        init_object(b,
                    BALL_X,
                    BALL_Y,
                    BALL_SIZE,
                    BALL_SIZE,
                    (BALL_VELOCITY * (coin_flip() ? 1 : -1)),
                    (BALL_VELOCITY * (coin_flip() ? 1 : -1)),
                    "Ball");
        return;
    }

    /**Basic Movement Logic in pixels per second (this is where we use our delta_time factor)
     * this ensures that we're moving exactly the number of pixels specified in a second
     * irrespective of our computers clock speed - and independant of the FPS
     * 		rectangle.x += 70*delta_time; //Increment pixels to the RIGHT in the x direction per second
     * 		rectangle.y += 50*delta_time; //Increment pixels DOWN in the y direction per second
     * **/
    b->x += b->vel_x * time;
    b->y += b->vel_y * time;
}

/**
// THIS IS THE CHEAT - Collision Detection between rectangles provided by SDL - rather do my own logic...
void rectOverlap() {
    SDL_Rect b = {
        (int)ball.x - BALL_SIZE,
        (int)ball.y - BALL_SIZE,
        (int)ball.width,
        (int)ball.height
    };

    SDL_Rect p1 = {
        (int)paddle1.x,
        (int)paddle1.y,
        (int)paddle1.width,
        (int)paddle1.height
    };

    if(SDL_HasIntersection(&b, &p1)) ball.vel_y = fabs(ball.vel_y); //Ball go left

    SDL_Rect p2 = {
        (int)paddle2.x,
        (int)paddle2.y,
        (int)paddle2.width,
        (int)paddle2.height
    };

    if(SDL_HasIntersection(&b, &p2)) ball.vel_y = -fabs(ball.vel_y); //Ball go right
}
**/

// Processes keyboard input to update Paddle positions - CURRENTLY USED
void update_paddles(float time)
{
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_SPACE])
    {
        served = TRUE;
    }
    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_RIGHT])
    {
        paddle1_g.x += PADDLE_VELOCITY * time;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_LEFT])
    {
        paddle1_g.x -= PADDLE_VELOCITY * time;
    }
    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_D])
    {
        paddle2_g.x += PADDLE_VELOCITY * time;
    }
    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_A])
    {
        paddle2_g.x -= PADDLE_VELOCITY * time;
    }
}

// Render our ball objects - CURRENTLY UNUSED
void render_ball(object_t *ball)
{
    SDL_Rect rect = {(int)ball->x, //- (int)ball->width/2,
                     (int)ball->y, //- (int)ball->height/2,
                     (int)ball->width,
                     (int)ball->height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &net_g);
}

// Render our Paddle objects - CURRENTLY UNUSED
void render_paddles(object_t *paddle1, object_t *paddle2)
{
    SDL_Rect p1 = {(int)paddle1->x, (int)paddle1->y, (int)paddle1->width, (int)paddle1->height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &p1);

    SDL_Rect p2 = {(int)paddle2->x, (int)paddle2->y, (int)paddle2->width, (int)paddle2->height};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &p2);
}

//////////////////////////////////////////////////////////////////////
// Alternate Function Flow
//////////////////////////////////////////////////////////////////////

// Check for ball collision with the walls - CURRENTLY USED
void ball_collision(object_t *b)
{
    if (b->x < 0)
    {
        b->vel_x = fabs(b->vel_x);
    }

    if (b->x > WINDOW_WIDTH - BALL_SIZE)
    {
        b->vel_x = -fabs(b->vel_x);
    }
}

// Check for ball collision with the paddles - CURRENTLY USED
void check_collision(object_t *ball, object_t paddle1, object_t paddle2)
{
    if (ball->x + ball->width > paddle1.x && ball->x < paddle1.x + paddle1.width &&
        ball->y + ball->height >= paddle1.y && ball->y + ball->height < WINDOW_HEIGHT - MARGIN - paddle1.height + 5)
    {
        ball->vel_y *= -1;
    }

    if (ball->x + ball->width > paddle2.x && ball->x < paddle2.x + paddle2.width &&
        ball->y <= paddle2.y + paddle2.height && ball->y > MARGIN + paddle2.height - 5)
    {
        ball->vel_y *= -1;
    }
}

// Detects Paddle collisions with the walls...ALT Version - CURRENTLY UNUSED
void paddle_boundary()
{
    if (paddle1_g.x < MARGIN)
    {
        paddle1_g.x = MARGIN;
    }
    if (paddle1_g.x > WINDOW_WIDTH - PADDLE_WIDTH - MARGIN)
    {
        paddle1_g.x = WINDOW_WIDTH - PADDLE_WIDTH - MARGIN;
    }
    if (paddle2_g.x < MARGIN)
    {
        paddle2_g.x = MARGIN;
    }
    if (paddle2_g.x > WINDOW_WIDTH - PADDLE_WIDTH - MARGIN)
    {
        paddle2_g.x = WINDOW_WIDTH - PADDLE_WIDTH - MARGIN;
    }
    // rectOverlap(); //THE CHEAT if required... (hopefully not!)
}

// Prevent paddles from moving outside the boundaries of the window - CURRENTLY USED
void paddle_collision(object_t *paddle)
{
    if (paddle->x < MARGIN)
    {
        paddle->x = MARGIN;
    }
    if (paddle->x > WINDOW_WIDTH - PADDLE_WIDTH - MARGIN)
    {
        paddle->x = WINDOW_WIDTH - PADDLE_WIDTH - MARGIN;
    }
}

// Check for game over when ball hits the bottom part of the screen...From old Version - CURRENTLY UNUSED
void game_over(object_t *ball)
{
    if ((ball->y + ball->height) > WINDOW_HEIGHT - MARGIN || ball->y < 0 + MARGIN)
    {
        /**OLD METHOD - IGNORE FOR NOW
                        // GG GAME OVER
        ball->x = WINDOW_WIDTH/2;
        ball->y = WINDOW_HEIGHT/2;
        **/
    }
}

// GAME OVER CONDITION **NEEDS WORK AND TWEAKS - CURRENTLY USED
void gg(object_t *ball)
{
    if (ball->y < MARGIN)
    {
        update_score(1, 1);
    }
    if (ball->y > WINDOW_HEIGHT - MARGIN)
    {
        update_score(2, 1);
    }
}

// Get the Delta Time elapsed - CURRENTLY USED
float get_delta_time(void)
{
    /**Implement logic to keep a fixed timestep - Only update at the rate of FPS
     * We need to waste some time/sleep until we reach the frame target time
     * SDL_TICKS_PASSED takes in 2 parameters and compares the current time passed with target time
     * Sleep until we reach the target frame time in ms
     *         while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
     * However, using this while loop above is CPU intensive
     * We can use better methods**/

    // Calculate how much time we have to wait until we reach the target time frame
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    // Call the delay ONLY if we're too fast to process/update this frame
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    /**Delta Time is the difference between the current time and the last time of the last frame in seconds
     * Get the delta_time in seconds and update our objects**/
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    /**Store the milliseconds of the current frame to be used in the next one
     * As soon as we call SDL_Init, it keeps track of these ticks in ms**/
    last_frame_time = SDL_GetTicks();

    return delta_time;
}

// Renders our game objects - CURRENTLY USED
void render_obj(object_t *obj)
{
    /**Draw a Rectangle as our object
     * SDL_rect is a simple Struct from SDL that has x/y coordinates, and width/height of the rectangle**/
    SDL_Rect obj_rect = {(int)obj->x, (int)obj->y, (int)obj->width, (int)obj->height};
    if (obj->name == "Ball" || obj->name == "Net")
    {
        // Pass a color to fill the object with
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
        /**Ask SDL to actually render our rectangle
         * Takes the renderer, and a reference to the struct object - SDL will go ahead render the object and fill it
         * with a color**/
        SDL_RenderFillRect(renderer, &obj_rect);
    }

    if (obj->name == "Paddle1")
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &obj_rect);
    }

    if (obj->name == "Paddle2")
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &obj_rect);
    }
}

//////////////////////////////////////////////////////////////////////
// Back to OG Function Flow - Everything below is called in Main /////
//////////////////////////////////////////////////////////////////////

// Function to initialize the SDL Window
int initialize_window(void)
{
    /**SDL_INIT_EVERYTHING is a constant and the SDL_Init function will try to initialize everything, if it succeeds it
     * returns 0 If the SDL Library wasn't able to Initialize the Graphics/keyboardInput/etc. then we have an error**/
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL...\n"); // fprintf is the raw function of printf
        return FALSE;
    }

    // Create the SDL Window and initialize it with the following paramters: Title, x coordinate, y coordinate, width,
    // height, and several flags we can pass for the window
    window =
        SDL_CreateWindow("PONG",                 // Title of the window
                         SDL_WINDOWPOS_CENTERED, // X Coordinate
                         SDL_WINDOWPOS_CENTERED, // Y Coordinate
                         WINDOW_WIDTH,           // Width
                         WINDOW_HEIGHT,          // Height
                         0 // SDL_WINDOW_BORDERLESS - Flag that makes the window borderless, SDL_WINDOW_FULLSCREEN etc.
        );

    /**If something didn't go right while creating the window, test to check if the return of the pointer to that struct
     * window is actually at that address if NOT then we have an error**/
    if (!window)
    {
        fprintf(stderr, "Error creating SDL Window...\n");
        return FALSE;
    }

    /**Now create the SDL Render and initialize it with the following paramters:
     * Window to attach the renderer to, Driver code/display number (-1 is default driver), and any special flags for
     * rendering (0 for no special flags) SDL_RENDERER_ACCELERATED flag would make it use the computers GPU instead of
     * CPU SDL_RENDERER_PRESENTVSYNC will sync to local monitor refresh rate**/
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Again, check if the Render was successfully created or not
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL Renderer...\n");
        return FALSE;
    }

    /**At this point SDL was initialized, a Window was created, and also the Render was created
     * We were successful in initializing the entire window**/
    return TRUE;
}

// Setup function - Initializes the game struct objects and Players at the start of our game
void setup(void)
{
    /**
    initNet();
    initBall(&ball, (WINDOW_WIDTH/2 - BALL_SIZE/2), (WINDOW_HEIGHT/2 - BALL_SIZE/2), BALL_SIZE, BALL_VELOCITY);
    //otherBall = createBall(BALL_SIZE);
    initPaddle(&paddle1, PADDLE_WIDTH, PADDLE_HEIGHT, (WINDOW_WIDTH/2)-(PADDLE_WIDTH/2), (WINDOW_HEIGHT - PADDLE_HEIGHT
    - MARGIN)); initPaddle(&paddle2, PADDLE_WIDTH, PADDLE_HEIGHT, (WINDOW_WIDTH/2)-(PADDLE_WIDTH/2), MARGIN);
    **/
    init_object(&net_g, NET_X, NET_Y, NET_WIDTH, NET_HEIGHT, 0, 0, "Net");
    init_object(&ball_g,
                BALL_X,
                BALL_Y,
                BALL_SIZE,
                BALL_SIZE,
                (BALL_VELOCITY * (coin_flip() ? 1 : -1)),
                (BALL_VELOCITY * (coin_flip() ? 1 : -1)),
                "Ball");
    init_object(
        &paddle1_g, PADDLE_X, PADDLE1_Y, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_VELOCITY, PADDLE_VELOCITY, "Paddle1");
    init_object(
        &paddle2_g, PADDLE_X, PADDLE2_Y, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_VELOCITY, PADDLE_VELOCITY, "Paddle2");
    player1 = create_player();
    player2 = create_player();
}

// Function to poll SDL events and process our game (keyboard) input
void process_input(void)
{
    SDL_Event event; // The SDL event is not a pointer, just a regular struct

    /**SDL will Poll everything - reads all the inputs/events that are happening
     * Pass the reference to (address of) the event as the parameter**/
    SDL_PollEvent(&event);

    // Check the type of event to see what was polled
    switch (event.type)
    {
        // SDL_Quit is an event that occurs when you click the "x" button on the Window
        case SDL_QUIT:
            game_is_running = FALSE; // Game loops flag has been set to false, this will quit the game loop
            break;
        // Test the code of the Key that was pressed
        case SDL_KEYDOWN:
            // Inside the event struct we have the key, and inside the key we have the key symbol, inside which we have
            // the actual symbol
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                game_is_running = FALSE; // If we press the ESC key, game loop quits
            }
            /** OLDER IMPLEMENTATION JUST IGNORE ALL OF THIS
                // Set paddle velocities based on left/right arrow keys
                if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_UP) paddle1.vel_x = 400;
                if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_DOWN) paddle1.vel_x = -400;
                if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_w) paddle2.vel_x = 400;
                if(event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_s) paddle2.vel_x = -400;
                // Set paddle velocities based on up/down arrow keys
                if(event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) {
                    //paddle.vel_x = -300;
                }
                if(event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) {
                    //paddle.vel_x = 300;
                }
            **/
            break;
            /**	OLDER IMPLEMENTATION JUST IGNORE ALL OF THIS
            case SDL_KEYUP:
                // Reset paddle velocities based on arrow keys
                if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym ==
            SDLK_UP || event.key.keysym.sym == SDLK_DOWN) { paddle1.vel_x = 0; paddle1.vel_y = 0;
                }
                if(event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_s ||
            event.key.keysym.sym == SDLK_w) { paddle2.vel_x = 0; paddle2.vel_y = 0;
                }
                break;
            **/
    }
}

// Update function with a fixed time step
void update(void)
{
    // Grab the time that has elapsed
    float time = get_delta_time();
    // Update ball position based on its velocity
    update_ball(&ball_g, time);
    update_ball(&otherBall_g, time);
    // Update paddle positions
    update_paddles(time);
    // Check collisions between ball and wall
    ball_collision(&ball_g);
    // Check collisions between ball and paddles
    check_collision(&ball_g, paddle1_g, paddle2_g);
    // Check for collisions between paddles and wall
    paddle_collision(&paddle1_g);
    paddle_collision(&paddle2_g);
    // Check condition for Game Over
    gg(&ball_g);
}

// Render function to draw our game objects in the SDL window
void render(void)
{
    /**This function will set the color that we want to use for drawing operations with the following parameters:
     * The Renderer, <R>, <G>, <B>, and <A> where RGB is the color content and A is the transparency
     * A of 255 is OPAQUE - no transparency, 0 is full transparency**/
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    /**Clear the Render context with the color that we just activated
     * Specify the renderer to clear as the argument**/
    SDL_RenderClear(renderer);

    // Now we can go ahead and start drawing our game objects

    render_obj(&net_g);
    render_obj(&ball_g);
    render_obj(&paddle1_g);
    render_obj(&paddle2_g);
    // renderBall(&otherBall);
    // renderPaddles(&paddle1, &paddle2);

    /**Afterwards - SDL uses a Double Buffer:
     * First one is the buffer that the user is seeing at the moment, and behind is a hidden buffer where we can start
     * populating our game objects to be displayed The idea is to render everything necessary in the back/hidden buffer
     * and then show everything at once to prevent flickering or glitches Once the back buffer has been completely
     * rendered, we swap the back buffer with the front buffer and the process repeats SDL_RenderPresent is our Buffer
     * Swap It takes in the "renderer" as its parameter - swaps the 2 buffers and diplays the back-buffer content in the
     * renderer context**/
    SDL_RenderPresent(renderer);
}

// Function to destroy our SDL Window and Renderer
void destroy_window(void)
{
    // Destroy all the elements in the reverse order that they were created
    /**Since our renderer and window are global variables
     * we are wrapping them with a NULL check to ensure program safety and avoid segmentation faults**/
    if (renderer)
    {
        SDL_DestroyRenderer(renderer); // if renderer is not NULL
    }

    if (window)
    {
        SDL_DestroyWindow(window); // if window is not NULL
    }

    SDL_Quit();
}

// Main Function
int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));

    // another way of calling shutdown
    atexit(destroy_window);

    // init window will return TRUE or FALSE - this wouldn't execute the next line until it returns TRUE
    game_is_running = initialize_window();

    setup();

    // Start the basic skeleton of a naive Game loop
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    // Once the game loop quits, clear all the contents
    // destroy_window();

    return 0;
}
