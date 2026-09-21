#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <variant>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T>
using Ref = std::shared_ptr<T>;

using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

using UniformPacket = std::unordered_map<std::string, UniformValue>;

struct Uniform {
	std::string name;
	UniformValue value;
};

//struct UniformPacket {
//	//std::vector<Uniform> uniforms;
//	std::unordered_map<std::string, UniformValue> uniforms;
//
//	template <typename T>
//	void Add(const std::string& name, const T& value) {
//		uniforms.insert({ name, UniformValue(value) });
//	}
//
//};

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

	Shader() = default;
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::vector<std::pair<std::string, ShaderType>>& shaderStagePaths);

	std::string ReadShaderFile(const std::string& filePath);
	std::string ProcessShaderCode(const std::string& rawShaderCode);
	uint32_t CompileShader(const std::string& shaderCode, ShaderType type);

	uint32_t ShaderPipeline(const std::string& shaderPath, ShaderType type);

	void LinkProgram(const std::vector<uint32_t>& compiledIDs);

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;
	void Swap(Shader& other) noexcept;

	void use() { glUseProgram(m_ID); }
	~Shader() { if(m_ID != 0) glDeleteProgram(m_ID); }

	void setBool(const std::string& name, bool value) {
		glUniform1i(getUniformLocation(name), (int)value);
	}
	void setInt(const std::string& name, int value) {
		glUniform1i(getUniformLocation(name), value);
	}
	void setFloat(const std::string& name, float value) {
		glUniform1f(getUniformLocation(name), value);
	}
	void setVec2(const std::string& name, const glm::vec2& value) {
		glUniform2fv(getUniformLocation(name), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) {
		glUniform2f(getUniformLocation(name), x, y);
	}
	void setVec3(const std::string& name, const glm::vec3& value) {
		glUniform3fv(getUniformLocation(name), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(getUniformLocation(name), x, y, z);
	}
	void setVec4(const std::string& name, const glm::vec4& value) {
		glUniform4fv(getUniformLocation(name), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(getUniformLocation(name), x, y, z, w);
	}
	void setMat2(const std::string& name, const glm::mat2& mat) {
		glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string& name, const glm::mat3& mat){
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, const glm::mat4& mat){
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void ApplyUniforms(const UniformPacket& packet);

private:

	void deleteProgram() {}

	int getUniformLocation(const std::string& name);
	void checkCompileErrors(unsigned int shader, std::string type);

	GLenum getGLType(ShaderType type);

	std::unordered_map<std::string, int> m_UniformCache;
	uint32_t m_ID{};
};
