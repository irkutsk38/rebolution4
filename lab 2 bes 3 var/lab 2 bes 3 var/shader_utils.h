#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

char* load_shader_source(const char* file_path);
GLuint compile_shader(const char* source, GLenum shader_type);
GLuint create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path);
void set_uniform(GLuint shader_program, const char* uniform_name, GLint count, GLenum type, const void* value);

#endif
