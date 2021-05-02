#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

#include <rendine/Texture.hpp>
#include <rendine/ShaderProgram.hpp>
#include <rendine/utils/Log.hpp>

float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
};

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

		this->initAssets();
	}

	void initAssets()
	{
		auto file = "res/tex/texture.png";
		auto status = this->texture.loadFromFile(file);
		if (status.isErr()) {
			/* Error while loading texture */
			LOG_ERR('\'' << file << "' -> " << status.unwrapErr());
			return ;
		}
		LOG_INFO("Loaded texture '" << file << '\'');

		file = "res/shaders/vertex.spv";
		this->vert_shader = std::make_shared<rendine::Shader>();
		auto status2 = this->vert_shader->loadFromFile(file, rendine::ShaderStage::Vertex);
		if (status2.isErr()) {
			/* Error while loading shader */
			LOG_ERR('\'' << file << "' -> " << status2.unwrapErr());
			return ;
		}
		LOG_INFO("Loaded shader '" << file << '\'');

		file = "res/shaders/fragment.spv";
		this->frag_shader = std::make_shared<rendine::Shader>();
		auto status3 = this->frag_shader->loadFromFile(file, rendine::ShaderStage::Fragment);
		if (status3.isErr()) {
			/* Error while loading shader */
			LOG_ERR('\'' << file << "' -> " << status3.unwrapErr());
			return ;
		}
		LOG_INFO("Loaded shader '" << file << '\'');

		this->program = std::make_shared<rendine::ShaderProgram>();

		this->program->attachShader(this->vert_shader);
		this->program->attachShader(this->frag_shader);

		auto status4 = this->program->link();
		if (status4.isErr()) {
			/* Error while loading program */
			LOG_ERR(status4.unwrapErr());
			return ;
		}
		LOG_INFO("Loaded shader program (handle " << this->program->getHandle() << ")");


		//////

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glUseProgram(this->program->getHandle());
		/*int vertexColorLocation = glGetUniformLocation(this->program->getHandle(), "material.color");
		glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);*/

		//////////

		LOG_INFO("Successfully initialized engine");
		this->is_valid = true;
	}

	int	run()
	{
		if (!this->is_valid) {
			LOG_EXIT("Program exited because of runtime error");
			return (-1);
		}

		glClearColor(.2f, .2f, .2f, 1.f);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(this->program->getHandle());
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
	std::shared_ptr<rendine::Shader>		frag_shader;
	std::shared_ptr<rendine::Shader>		vert_shader;
	std::shared_ptr<rendine::ShaderProgram>	program;
	GLuint	vbo, vao;
};

int main(void)
{
	LOG_INFO("Launching Rendine version " << RENDINE_VERSION_STRING());
	Program	program(800, 600, "OpenGL");
	return program.run();
}
