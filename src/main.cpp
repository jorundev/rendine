#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

#include <rendine/Texture.hpp>
#include <rendine/utils/Log.hpp>

class Program {
public:
	Program(unsigned int width, unsigned int height, const char *title)
		: is_valid(false), window(nullptr)
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

		this->window = SDL_CreateWindow(	title,
											SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
											width, height,
											SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN	);
		this->gl = SDL_GL_CreateContext(this->window);

		if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
			std::cout << "Failed to init OpenGL context" << std::endl;
			return ;
		}

		LOG_INFO("Successfully created OpenGL context");

		auto file = "res/tex/texture.png";
		auto status = this->texture.loadFromFile(file);
		if (status.isErr()) {
			/* Error while loading texture */
			LOG_ERR('\'' << file << "' -> " << status.unwrapErr());
			return ;
		}
		LOG_INFO("Loaded texture '" << file << '\'');

		LOG_INFO("Successfully initialized engine");
		this->is_valid = true;
	}

	int	run()
	{
		if (!this->is_valid) {
			LOG_EXIT("Program exited because of runtime error");
			return (-1);
		}

		glClearColor(1.f, .0f, .0f, 1.f);
		while (true) {
			if (!this->loop()) {
				LOG_INFO("Exiting program (Exit event)");
				break ;
			}
			SDL_GL_SwapWindow(this->window);
		}
		return (0);
	}

	bool loop()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_ESCAPE)
						return false;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							return false;
						default:
							break;
					}
					break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		return true;
	}

	~Program()
	{
		SDL_GL_DeleteContext(this->gl);
		SDL_DestroyWindow(this->window);
		SDL_Quit();
	}

private:
	bool				is_valid;
	SDL_Window			*window;
	SDL_GLContext		gl;
	rendine::Texture	texture;
};

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

int main(void)
{
	LOG_INFO("Launching Rendine version " << RENDINE_VERSION_STRING());
	Program	program(800, 600, "OpenGL");
	return program.run();
}
