#pragma once
#include <string>

namespace Shader {

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	std::string readShader(const char* shader_path);
}