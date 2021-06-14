#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h" //Not in angled brackets since we're just grabbing this as a file from the same folder, not from a system directory

// Global Variables of our System
SDL_Window *window = NULL; //Address of the SDL Window as a global variable
SDL_Renderer *renderer = NULL; //Address of the SDL Renderer
int game_is_running = FALSE; //Flag to notify if game is running or not, quits the game loop if false
int last_frame_time = 0; //Keep track of the last frame we call (in milliseconds)

// Declaring struct of our Game objects
struct objects {
	float x, y, width, height, vel_x, vel_y;
} ball, paddle;

// Function to initialize the SDL Window
int initialize_window(void) {
	/**SDL_INIT_EVERYTHING is a constant and the SDL_Init function will try to initialize everything, if it succeeds it returns 0
	 * If the SDL Library wasn't able to Initialize the Graphics/keyboardInput/etc. then we have an error**/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL...\n"); //fprintf is the raw function of printf
		return FALSE;
	}
	// Create the SDL Window and initialize it with the following paramters: Title, x coordinate, y coordinate, width, height, and several flags we can pass for the window
	window = SDL_CreateWindow(
		NULL, //Title of the window
		SDL_WINDOWPOS_CENTERED, //X Coordinate
		SDL_WINDOWPOS_CENTERED, //Y Coordinate
		WINDOW_WIDTH, //Width
		WINDOW_HEIGHT, //Height
		SDL_WINDOW_BORDERLESS //Flag that makes the window borderless
	);
	/**If something didn't go right while creating the window, test to check if the return of the pointer to that struct window is actually at that address
	 * if NOT then we have an error**/
	if(!window) {
		fprintf(stderr, "Error creating SDL Window...\n");
		return FALSE;
	}
	/**Now create the SDL Render and initialize it with the following paramters:
	 * Window to attach the renderer to, Driver code/display number (-1 is default driver), and any special flags for rendering (0 for no special flags)**/
	renderer = SDL_CreateRenderer(window, -1, 0);
	// Again, check if the Render was successfully created or not
	if(!renderer) {
		fprintf(stderr, "Error creating SDL Renderer...\n");
		return FALSE;
	}
	/**At this point SDL was initialized, a Window was created, and also the Render was created
	 * We were successful in initializing the entire window**/
	return TRUE;
}

// Function to poll SDL events and process our game (keyboard) input
void process_input(void) {
	SDL_Event event; //The SDL event is not a pointer, just a regular struct
	// SDL will Poll everything - reads all the inputs/events that are happening
	SDL_PollEvent(&event); //Pass the reference to (address of) the event as the parameter

	// Check the type of event to see what was polled
	switch(event.type) {
		// SDL_Quit is an event that occurs when you click the "x" button on the Window
		case SDL_QUIT:
			game_is_running = FALSE; //Game loops flag has been set to false, this will quit the game loop
			break;
		// Test the code of the Key that was pressed
		case SDL_KEYDOWN:
			// Inside the event struct we have the key, and inside the key we have the key symbol, inside which we have the actual symbol
			if(event.key.keysym.sym == SDLK_ESCAPE) game_is_running = FALSE; //If we press the ESC key, game loop quits
            
            // Set paddle velocities based on left/right arrow keys
			if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) paddle.vel_x = 300;
            else if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) paddle.vel_x = -300;
            break;
        case SDL_KEYUP:
            
            // Reset paddle velocities based on left/right arrow keys
            if(event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d) {
                paddle.vel_x = 0;
                paddle.vel_y = 0;
            }
            break;
            
	//	default:
	//		printf("Clearly some invalid event that we did not handle has occurred...\n");
	//		break;
	}
}

// Setup function - Initializes the game struct object at the start of our game
void setup(void) {
    // Initialize the ball object moving down at a constant velocity
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;
    ball.vel_x = 300;
    ball.vel_y = 300;
    // Initialize paddle position
    paddle.x = (WINDOW_WIDTH/2)-(paddle.width/2); //centered at the bottom of the screen
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.width = 100;
    paddle.height = 20;
    paddle.vel_x = 0;
    paddle.vel_y = 0;
}

// Update function with a fixed time step
void update(void) {
	// Implement logic to keep a fixed timestep - Only update at the rate of FPS
	    // We need to waste some time/sleep until we reach the frame target tim
	    // SDL_TICKS_PASSED takes in 2 parameters and compares the current time passed with target time
        // Sleep until we reach the target frame time in ms
	        //while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
        // However, using this while loop above is CPU intensive
        // We can use better methods
    // Calculate how much time we have to wait until we reach the target time frame    
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    // Call the delay ONLY if we're too fast to process/update this frame
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) SDL_Delay(time_to_wait);

    // Delta Time is the difference between the current time and the last time of the last frame in seconds
    // Get the delta_time in seconds and update our objects
    float delta_time = (SDL_GetTicks() - last_frame_time)/1000.0f;

    // Store the milliseconds of the current frame to be used in the next one
	// As soon as we call SDL_Init, it keeps track of these ticks in ms
	last_frame_time = SDL_GetTicks();

	// Basic Movement Logic in pixels per second (this is where we use our delta_time factor)
    // this ensures that we're moving exactly the number of pixels specified in a second
    // irrespective of our computers clock speed - and independant of the FPS
	    //rectangle.x += 70*delta_time; //Increment pixels to the RIGHT in the x direction per second
	    //rectangle.y += 50*delta_time; //Increment pixels DOWN in the y direction per second
    
    // Update ball position based on its velocity
    ball.x += ball.vel_x*delta_time;
    ball.y += ball.vel_y*delta_time;

    // Update paddle position based on its velocity
    paddle.x += paddle.vel_x*delta_time;
    //paddle.y += paddle.vel_y*delta_time;
    
    // TODO: Check for ball collision with the walls

    // TODO: Check for ball collision with the paddle

    // TODO: Prevent paddle from moving outside the boundaries of the window

    // Check for game over when ball hits the bottom part of the screen
}

//Render function to draw our game objects in the SDL window
void render(void) {
	/**This function will set the color that we want to use for drawing operations with the following parameters:
	 * The Renderer, <R>, <G>, <B>, and <A> where RGB is the color content and A is the transparency
	 * A of 255 is OPAQUE - no transparency, 0 is full transparency**/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	/**Clear the Render context with the color that we just activated
	 * Specify the renderer to clear as the argument**/
	SDL_RenderClear(renderer); 
	
	//Now we can go ahead and start drawing our game objects

	/**Draw a Rectangle as our ball object
	 * SDL_rect is a simple Struct from SDL that has x/y coordinates, and width/height of the rectangle**/
	SDL_Rect ball_rect = {
		(int)ball.x,
		(int)ball.y, 
		(int)ball.width,
		(int)ball.height
	};
	//Pass a color to fill the ball with
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	/**Ask SDL to actually render our rectangle
	 * Takes the renderer, and a reference to the struct object - SDL will go ahead render the ball and fill it with a color**/
	SDL_RenderFillRect(renderer, &ball_rect);

    /**Draw a Rectangle as our paddle object
	 * SDL_rect is a simple Struct from SDL that has x/y coordinates, and width/height of the rectangle**/
	SDL_Rect paddle_rect = {
		(int)paddle.x,
		(int)paddle.y, 
		(int)paddle.width,
		(int)paddle.height
	};
	//Pass a color to fill the paddle with
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	/**Ask SDL to actually render our rectangle
	 * Takes the renderer, and a reference to the struct object - SDL will go ahead render the paddle and fill it with a color**/
	SDL_RenderFillRect(renderer, &paddle_rect);

	/**Afterwards - SDL uses a Double Buffer:
	 * First one is the buffer that the user is seeing at the moment, and behind is a hidden buffer where we can start populating our game objects to be displayed
	 * The idea is to render everything necessary in the back/hidden buffer and then show everything at once to prevent flickering or glitches
	 * Once the back buffer has been completely rendered, we swap the back buffer with the front buffer and the process repeats
	 * SDL_RenderPresent is our Buffer Swap
	 * It takes in the "renderer" as its parameter - swaps the 2 buffers and diplays the back-buffer content in the renderer context**/
	SDL_RenderPresent(renderer); 
}

void destroy_window(void) {
	//Destroy all the elements in the reverse order that they were created
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	//init window will return TRUE or FALSE - this wouldn't execute the next line until it returns TRUE
	game_is_running = initialize_window();

	setup();	

	//Start the basic skeleton of a naive Game loop
	while(game_is_running) {
		process_input();
		update();
		render();
	}	
	//Once the game loop quits, clear all the contents
	destroy_window();

	return 0;
}
