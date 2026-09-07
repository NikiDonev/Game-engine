#pragma once
#include <glad/glad.h>
#include <string>

std::string TranslateGlDebugId(unsigned int id);

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam);

void enableReportGlErrors();