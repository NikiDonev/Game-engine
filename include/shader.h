#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>


enum class ShaderType {
	Vertex,
	Fragment,
	Geometry,
	TessCtrl,
	TessEval,
	Compute
};

class Shader {
public:
	unsigned int ID{};
	std::vector<unsigned int> shaders;
	Shader() {}
	//Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
	//	compileShader(vertexPath, ShaderType::Vertex);
	//	compileShader(fragmentPath, ShaderType::Fragment);
	//	compileShader(geometryPath, ShaderType::Geometry);
	//	linkProgram();
	//}
	void create(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
		compileShader(vertexPath, ShaderType::Vertex);
		compileShader(fragmentPath, ShaderType::Fragment);
		compileShader(geometryPath, ShaderType::Geometry);
		linkProgram();
	}

	void compileShader(const char* path, ShaderType type) {
		if (path == nullptr) return;
		std::ifstream file;
		std::string shaderCode;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::stringstream stream;
		try {
			file.open(path);
			stream << file.rdbuf();

			file.close();
			shaderCode = stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* shaderSource = shaderCode.c_str();
		unsigned int shader = glCreateShader(getGLType(type));
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);
		std::string ext = std::filesystem::path(path).extension().string();
		checkCompileErrors(shader, ext);
		shaders.push_back(shader);
	}

	void linkProgram() {
		ID = glCreateProgram();
		for (unsigned int shader : shaders) 
			glAttachShader(ID, shader);

		glLinkProgram(ID);

		checkCompileErrors(ID, "PROGRAM");
		for (unsigned int shader : shaders)
			glDeleteShader(shader);
		shaders.clear();
	}

	void use() {
		glUseProgram(ID);
	}
	void deleteProgram() {
		glDeleteProgram(ID);
	}
	~Shader() {
		deleteProgram();
	}
	void setBool(const std::string& name, bool value) {
		glUniform1i(getUniformLocation(name), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) {
		glUniform1i(getUniformLocation(name), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) {
		glUniform1f(getUniformLocation(name), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(getUniformLocation(name), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) {
		glUniform2f(getUniformLocation(name), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(getUniformLocation(name), 1, &value[0]);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(getUniformLocation(name), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(getUniformLocation(name), 1, &value[0]);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(getUniformLocation(name), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) {
		glUniformMatrix2fv(getUniformLocation( name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat){
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat){
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

private:

	std::unordered_map<std::string, int> uniformCache;

	int getUniformLocation(const std::string& name) {
		auto it = uniformCache.find(name);
		if (it != uniformCache.end()) 
			return it->second;
		
		int location = glGetUniformLocation(ID, name.c_str());

		if (location == -1) {
			std::cerr << "ERROR::SHADER UNIFORM: Uniform '" << name << "' does not exist! \n";
		}

		uniformCache[name] = location;
		return location;
	}

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	GLenum getGLType(ShaderType type) {
		switch (type) {
		case ShaderType::Vertex: return GL_VERTEX_SHADER;
		case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
		case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
		}
		return -1;
	}
};