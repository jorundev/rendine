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

void loop(data_t *data)
{
	glClear(GL_COLOR_BUFFER_BIT);

	assethdnl_t	handle;

	handle = asset_new(ASSET_TYPE_TEXTURE);
	asset_load_from_file(handle, "res/texture.png");

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYUP:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					asset_release_all();
					exit(0);
				break;
		}
	}
}

bool run(void)
{
	data_t	data;

	if (!init(800, 600, &data))
		return true;

	glClearColor(1.f, .0f, .0f, 1.f);
	while (true) {
		loop(&data);
		SDL_GL_SwapWindow(data.window);
	}

	return false;
}

int main(void)
{
	return run() ? EXIT_FAILURE : EXIT_SUCCESS;
}
