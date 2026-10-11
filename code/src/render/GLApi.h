#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include <string>

namespace voxel {

struct GLApi {
    PFNGLGENVERTEXARRAYSPROC GenVertexArrays = nullptr;
    PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays = nullptr;
    PFNGLBINDVERTEXARRAYPROC BindVertexArray = nullptr;
    PFNGLGENBUFFERSPROC GenBuffers = nullptr;
    PFNGLDELETEBUFFERSPROC DeleteBuffers = nullptr;
    PFNGLBINDBUFFERPROC BindBuffer = nullptr;
    PFNGLBUFFERDATAPROC BufferData = nullptr;
    PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer = nullptr;
    PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray = nullptr;
    PFNGLCREATESHADERPROC CreateShader = nullptr;
    PFNGLSHADERSOURCEPROC ShaderSource = nullptr;
    PFNGLCOMPILESHADERPROC CompileShader = nullptr;
    PFNGLGETSHADERIVPROC GetShaderiv = nullptr;
    PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog = nullptr;
    PFNGLDELETESHADERPROC DeleteShader = nullptr;
    PFNGLCREATEPROGRAMPROC CreateProgram = nullptr;
    PFNGLATTACHSHADERPROC AttachShader = nullptr;
    PFNGLLINKPROGRAMPROC LinkProgram = nullptr;
    PFNGLGETPROGRAMIVPROC GetProgramiv = nullptr;
    PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog = nullptr;
    PFNGLDELETEPROGRAMPROC DeleteProgram = nullptr;
    PFNGLUSEPROGRAMPROC UseProgram = nullptr;
    PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation = nullptr;
    PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv = nullptr;

    bool Load(std::string& error) {
#define VOXEL_LOAD_GL(member, symbol) \
        if (!LoadFunction(member, symbol, error)) return false
        VOXEL_LOAD_GL(GenVertexArrays, "glGenVertexArrays");
        VOXEL_LOAD_GL(DeleteVertexArrays, "glDeleteVertexArrays");
        VOXEL_LOAD_GL(BindVertexArray, "glBindVertexArray");
        VOXEL_LOAD_GL(GenBuffers, "glGenBuffers");
        VOXEL_LOAD_GL(DeleteBuffers, "glDeleteBuffers");
        VOXEL_LOAD_GL(BindBuffer, "glBindBuffer");
        VOXEL_LOAD_GL(BufferData, "glBufferData");
        VOXEL_LOAD_GL(VertexAttribPointer, "glVertexAttribPointer");
        VOXEL_LOAD_GL(EnableVertexAttribArray, "glEnableVertexAttribArray");
        VOXEL_LOAD_GL(CreateShader, "glCreateShader");
        VOXEL_LOAD_GL(ShaderSource, "glShaderSource");
        VOXEL_LOAD_GL(CompileShader, "glCompileShader");
        VOXEL_LOAD_GL(GetShaderiv, "glGetShaderiv");
        VOXEL_LOAD_GL(GetShaderInfoLog, "glGetShaderInfoLog");
        VOXEL_LOAD_GL(DeleteShader, "glDeleteShader");
        VOXEL_LOAD_GL(CreateProgram, "glCreateProgram");
        VOXEL_LOAD_GL(AttachShader, "glAttachShader");
        VOXEL_LOAD_GL(LinkProgram, "glLinkProgram");
        VOXEL_LOAD_GL(GetProgramiv, "glGetProgramiv");
        VOXEL_LOAD_GL(GetProgramInfoLog, "glGetProgramInfoLog");
        VOXEL_LOAD_GL(DeleteProgram, "glDeleteProgram");
        VOXEL_LOAD_GL(UseProgram, "glUseProgram");
        VOXEL_LOAD_GL(GetUniformLocation, "glGetUniformLocation");
        VOXEL_LOAD_GL(UniformMatrix4fv, "glUniformMatrix4fv");
#undef VOXEL_LOAD_GL
        return true;
    }

private:
    template <typename Function>
    static bool LoadFunction(Function& function, const char* name, std::string& error) {
        function = reinterpret_cast<Function>(SDL_GL_GetProcAddress(name));
        if (function != nullptr) {
            return true;
        }

        error = std::string("OpenGL function could not be loaded: ") + name;
        return false;
    }
};

}
