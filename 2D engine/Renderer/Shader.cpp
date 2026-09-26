#include "Shader.h"
#include "../Debug/Logging.h"

Shader::Shader(const std::string& vertexPathOrCode, const std::string& fragmentPathOrCode) {
	std::vector<uint32_t> compiledIDs;
	compiledIDs.push_back(ShaderPipeline(vertexPathOrCode, ShaderType::Vertex));
	compiledIDs.push_back(ShaderPipeline(fragmentPathOrCode, ShaderType::Fragment));
	LinkProgram(compiledIDs);
}

Shader::Shader(const std::vector<std::pair<std::string, ShaderType>>& shaderStagePaths) {
	std::vector<uint32_t> compiledIDs;
	for (const auto& [path, stageType] : shaderStagePaths) 
		compiledIDs.push_back(ShaderPipeline(path, stageType));

	LinkProgram(compiledIDs);
}

uint32_t Shader::ShaderPipeline(const std::string& shaderPathOrCode, ShaderType type) {
	std::string shaderSrc = ReadShaderFile(shaderPathOrCode);
	std::string processed = ProcessShaderCode(shaderSrc);
	return CompileShader(shaderSrc, type);
}

std::string Shader::ProcessShaderCode(const std::string& rawShaderCode) {
	return rawShaderCode;
}

std::string Shader::ReadShaderFile(const std::string& filePathOrCode) {
	bool isSourceCode = (filePathOrCode.find("#version") != std::string::npos);
	if (isSourceCode) return filePathOrCode;

	std::ifstream file(filePathOrCode);
	if (!file.is_open()) {
		LOG_ERROR("SHADER ERROR: Failed to open file: %s", filePathOrCode.c_str());
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

	std::string shaderType = "[shader type]";
	switch (type) {
	case ShaderType::Vertex: shaderType = "vertex"; break;
	case ShaderType::Fragment: shaderType = "fragment"; break;
	case ShaderType::Geometry: shaderType = "geometry"; break;
	case ShaderType::Compute: shaderType = "compute"; break;
	}
	checkCompileErrors(shaderID, shaderType);
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
		LOG_ERROR("SHADER UNIFORM: Uniform %s does not exist", name.c_str());
	}

	m_UniformCache[name] = location;
	return location;
}

void Shader::ApplyUniforms(const UniformPacket& packet) {
	use();
	for (const auto& [name, value] : packet.uniforms) {
		if (auto* p = std::get_if<int>(&value)) setInt(name, *p);
		else if (auto* p = std::get_if<float>(&value)) setFloat(name, *p);
		else if (auto* p = std::get_if<glm::vec2>(&value)) setVec2(name, *p);
		else if (auto* p = std::get_if<glm::vec3>(&value)) setVec3(name, *p);
		else if (auto* p = std::get_if<glm::vec4>(&value)) setVec4(name, *p);
		else if (auto* p = std::get_if<glm::mat4>(&value)) setMat4(name, *p);
	}
}

GLenum Shader::getGLType(ShaderType type) {
	switch (type) {
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
	}
	return -1;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			//std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			LOG_ERROR("SHADER COMPILATION ERROR of type: %s \n %s \n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			LOG_ERROR("PROGRAM LINKING ERROR of type: %s \n %s \n -- --------------------------------------------------- -- ", type.c_str(), infoLog);
			//std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}