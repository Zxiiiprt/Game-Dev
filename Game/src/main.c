#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h" //Not in angled brackets since we're just grabbing this as a file from the same folder, not from a system directory

SDL_Window *window = NULL; //Address of the SDL Window as a global variable
SDL_Renderer *renderer = NULL; //Address of the SDL Renderer
int game_is_running = FALSE; //Flag to notify if game is running or not, quits the game loop if false
//Keep track of the last frame we call (in milliseconds)
int last_frame_time = 0;

struct rectangle {
	float x, y, width, height;
} rectangle;

int initialize_window(void) {
	/**SDL_INIT_EVERYTHING is a constant and the SDL_Init function will try to initialize everything, if it succeeds it returns 0
	 * If the SDL Library wasn't able to Initialize the Graphics/keyboardInput/etc. then we have an error**/
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL...\n"); //fprintf is the raw function of printf
		return FALSE;
	}
	//Create the SDL Window and initialize it with the following paramters: Title, x coordinate, y coordinate, width, height, and several flags we can pass for the window
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
	//Again, check if the Render was successfully created or not
	if(!renderer) {
		fprintf(stderr, "Error creating SDL Renderer...\n");
		return FALSE;
	}
	/**At this point SDL was initialized, a Window was created, and also the Render was created
	 * We were successful in initializing the entire window**/
	return TRUE;
}

void process_input() {
	SDL_Event event; //The SDL event is not a pointer, just a regular struct
	//SDL will Poll everything - reads all the inputs/events that are happening
	SDL_PollEvent(&event); //Pass the reference to (address of) the event as the parameter

	//Check the type of event to see what was polled
	switch(event.type) {
		//SDL_Quit is an event that occurs when you click the "x" button on the Window
		case SDL_QUIT:
			game_is_running = FALSE; //Game loops flag has been set to false, this will quit the game loop
			break;
		//Test the code of the Key that was pressed
		case SDL_KEYDOWN:
			//Inside the event struct we have the key, and inside the key we have the key symbol, inside which we have the actual symbol
			if(event.key.keysym.sym == SDLK_ESCAPE) game_is_running = FALSE; //If we press the ESC key, game loop quits
			break;
	//	default:
	//		printf("Clearly some invalid event that we did not handle has occurred...\n");
	//		break;
	}
}

void setup() {
	//Setup the struct object (rectangle) at the start of the game
	rectangle.x = 20;
	rectangle.y = 20;
	rectangle.width = 15;
	rectangle.height = 15;
}

void update() {
	//Implement logic to keep a fixed timestep - Only update at the rate of FPS
	//We need to waste some time/sleep until we reach the frame target tim
	//SDL_TICKS_PASSED takes in 2 parameters and compares the current time passed with target time
	while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

	//As soon as we call SDL_Init, it keeps track of these ticks in ms
	last_frame_time = SDL_GetTicks();

	//Basic Movement Logic
	rectangle.x += 2; //Increment a pixel to the RIGHT in the x direction
	rectangle.y += 2; //Increment a pixel DOWN in the y direction
}

void render() {
	/**This function will set the color that we want to use for drawing operations with the following parameters:
	 * The Renderer, <R>, <G>, <B>, and <A> where RGB is the color content and A is the transparency
	 * A of 255 is OPAQUE - no transparency, 0 if full transparency**/
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	/**Clear the Render context with the color that we just activated
	 * Specify the renderer to clear as the argument**/
	SDL_RenderClear(renderer); 
	
	//Now we can go ahead and start drawing our game objects

	/**Draw a Rectangle
	 * SDL_rect is a simple Struct from SDL that has x/y coordinates, and width/height of the rectangle**/
	SDL_Rect rectangle_rect = {
		(int)rectangle.x,
		(int)rectangle.y, 
		(int)rectangle.width,
		(int)rectangle.height
	};
	//Pass a color to fill the rectangle with
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	/**Ask SDL to actually render our rectangle
	 * Takes the renderer, and a reference to the struct object - SDL will go ahead render the Rectangle and fill it with a color**/
	SDL_RenderFillRect(renderer, &rectangle_rect);  

	/**Afterwards - SDL uses a Double Buffer:
	 * First one is the buffer that the user is seeing at the moment, and behind is a hidden buffer where we can start populating our game objects to be displayed
	 * The idea is to render everything necessary in the back/hidden buffer and then show everything at once to prevent flickering or glitches
	 * Once the back buffer has been completely rendered, we swap the back buffer with the front buffer and the process repeats
	 * SDL_RenderPresent is our Buffer Swap
	 * It takes in the "renderer" as its parameter - swaps the 2 buffers and diplays the back-buffer content in the renderer context**/
	SDL_RenderPresent(renderer); 
}

void destroy_window() {
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
