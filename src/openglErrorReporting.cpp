#include "openglErrorReporting.h"
#include <iostream>
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

std::string TranslateGlDebugId(unsigned int id)
{
    switch (id) {
    case 0x0500: return "GL_INVALID_ENUM";
    case 0x0501: return "GL_INVALID_VALUE";
    case 0x0502: return "GL_INVALID_OPERATION";
    case 0x0503: return "GL_STACK_OVERFLOW";
    case 0x0504: return "GL_STACK_UNDERFLOW";
    case 0x0505: return "GL_OUT_OF_MEMORY";
    case 0x0506: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case 0x824C: return "GL_CONTEXT_LOST";
    default:     return "Driver-Specific ID (" + std::to_string(id) + ")";
    }
}

void PrintCppSourceLineTrace()
{
    HANDLE process = GetCurrentProcess();

    // FIX: Allocated a real array to hold the stack frame addresses
    void* stack[32];
    USHORT frames = CaptureStackBackTrace(0, 32, stack, NULL);

    IMAGEHLP_LINE64 line;
    ZeroMemory(&line, sizeof(IMAGEHLP_LINE64));
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    DWORD displacement;

    std::cout << "\n--- CALL STACK TRACE ---" << std::endl;
    bool foundYourCode = false;

    for (USHORT i = 0; i < frames; ++i)
    {
        // Translate each frame address to a file and line number
        if (SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacement, &line))
        {
            std::string file(line.FileName);

            // Skip the error reporting files themselves to find your game code
            if (file.find("openglErrorReporting") == std::string::npos &&
                file.find("vctools") == std::string::npos)
            {
                std::cout << "  -> " << file << " (Line: " << line.LineNumber << ")" << std::endl;
                foundYourCode = true;
            }
        }
    }

    if (!foundYourCode) {
        std::cout << "  (Could not resolve symbols for these frames. Ensure your PDB matches.)" << std::endl;
    }
    std::cout << "------------------------\n" << std::endl;
}

void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char* message, const void* userParam)
{
    // Filter out common background noise codes so it doesn't spam your terminal
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131222) return;
    if (type == GL_DEBUG_TYPE_PERFORMANCE) return;

    std::cout << "\n================= OPENGL CRASH BREAKPOINT =================" << std::endl;
    std::cout << "Driver Message: " << message << std::endl;
    std::cout << "===========================================================\n" << std::endl;

    // FORCE VISUAL STUDIO TO STOP HERE INDEPENDENT OF INTEL SYMBOLS
    int* crashPointer = nullptr;
    *crashPointer = 0xDEAD;
}


void enableReportGlErrors()
{
    HANDLE process = GetCurrentProcess();

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS);

    // Passing TRUE forces Windows to hunt down your project's local debug symbols (.pdb)
    SymInitialize(process, NULL, TRUE);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
