#include "Shader.h" 
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Shader {

    unsigned int CompileShader(unsigned int type, const std::string& source) {

		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);


		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile shader!" << std::endl;
			std::cout << message << std::endl;
			delete[] message;
		}

		return id;
    }

    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
		unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		unsigned int program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
    }

	std::string readShader(const char* shader_path) {

		std::ifstream file(shader_path, std::ios::in | std::ios::binary);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			file.close();
			return buffer.str();
		}
		else {
			return "ERROR_READ";
		}
	}

}