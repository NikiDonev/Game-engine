#include "Shader.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::vector<uint32_t> compiledIDs;
	compiledIDs.push_back(ShaderPipeline(vertexPath, ShaderType::Vertex));
	compiledIDs.push_back(ShaderPipeline(fragmentPath, ShaderType::Fragment));
	LinkProgram(compiledIDs);
}

Shader::Shader(const std::vector<std::pair<std::string, ShaderType>>& shaderStagePaths) {
	std::vector<uint32_t> compiledIDs;
	for (const auto& [path, stageType] : shaderStagePaths) 
		compiledIDs.push_back(ShaderPipeline(path, stageType));

	LinkProgram(compiledIDs);
}

uint32_t Shader::ShaderPipeline(const std::string& shaderPath, ShaderType type) {
	std::string shaderSrc = ReadShaderFile(shaderPath);
	std::string processed = ProcessShaderCode(shaderSrc);
	return CompileShader(shaderSrc, type);
}

std::string Shader::ProcessShaderCode(const std::string& rawShaderCode) {
	return rawShaderCode;
}

std::string Shader::ReadShaderFile(const std::string& filePath) {
	bool isSourceCode = (filePath.find("#version") != std::string::npos);
	if (isSourceCode) return filePath;

	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "[Shader Error] Failed to open file: " << filePath << std::endl;
		return "";
	}
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

uint32_t Shader::CompileShader(const std::string& shaderCode, ShaderType type) {
	uint32_t shaderID = glCreateShader(getGLType(type));
	const char* shaderSource = shaderCode.c_str();
	glShaderSource(shaderID, 1, &shaderSource, NULL);
	glCompileShader(shaderID);
	checkCompileErrors(shaderID, "[shader type]");
	return shaderID;
}

void Shader::LinkProgram(const std::vector<uint32_t>& compiledIDs) {
	m_ID = glCreateProgram();
	for (uint32_t shader : compiledIDs)
		glAttachShader(m_ID, shader);

	glLinkProgram(m_ID);

	checkCompileErrors(m_ID, "PROGRAM");
	for (uint32_t shader : compiledIDs)
		glDeleteShader(shader);
}

Shader::Shader(Shader&& other) noexcept { Swap(other); }
Shader& Shader::operator=(Shader&& other) noexcept {
	if (this != &other) {
		if (m_ID != 0) glDeleteProgram(m_ID);
		m_ID = 0;
		Swap(other);
	}
	return *this;
}
void Shader::Swap(Shader& other) noexcept {
	std::swap(m_ID, other.m_ID);
	std::swap(m_UniformCache, other.m_UniformCache);
}


int Shader::getUniformLocation(const std::string& name) {
	auto it = m_UniformCache.find(name);
	if (it != m_UniformCache.end())
		return it->second;

	int location = glGetUniformLocation(m_ID, name.c_str());

	if (location == -1) {
		std::cerr << "ERROR::SHADER UNIFORM: Uniform '" << name << "' does not exist! \n";
	}

	m_UniformCache[name] = location;
	return location;
}

GLenum Shader::getGLType(ShaderType type) {
	switch (type) {
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
	}
	return -1;
}