#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cglm/vec2.h>
#include <glad/glad.h>

#include "types.h"
#include "assets.h"

typedef
struct data_s {
	SDL_Window		*window;
	SDL_GLContext	*gl;
	assethdnl_t		texture;
}	data_t;

bool init(unsigned int width, unsigned int height, data_t *data)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	data->window = SDL_CreateWindow(	"",
										SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
										width, height,
										SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN	);
	data->gl = SDL_GL_CreateContext(data->window);

	if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
		printf("Failed to init OpenGL context\n");
		return false;
	}

	return true;
}


void	close(data_t *data)
{
	asset_release_all();
	SDL_DestroyWindow(data->window);
	SDL_Quit();
	exit(0);
}


void loop(data_t *data)
{
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					close(data);
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
            		case SDL_WINDOWEVENT_CLOSE:   // exit game
                		close(data);
                		break;
            		default:
                		break;
       			 }
        		break;
		}
	}
}

bool run(void)
{
	data_t	data;

	if (!init(800, 600, &data))
		return false;

	data.texture = asset_new(ASSET_TYPE_TEXTURE);
	if (!asset_load_from_file(data.texture, "res/texture.png")) {
		printf("[%s] ERROR: %s\n", asset_get_error_function(), asset_get_error());
		return false;
	}

	glClearColor(1.f, .0f, .0f, 1.f);
	while (true) {
		loop(&data);
		SDL_GL_SwapWindow(data.window);
	}

	return true;
}

int main(void)
{
	bool ret = run();
	asset_release_all();
	return ret ? EXIT_SUCCESS : EXIT_FAILURE;
}
