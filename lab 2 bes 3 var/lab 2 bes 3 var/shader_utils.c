#include "shader_utils.h"

char* load_shader_source(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", file_path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* source = (char*)malloc(file_size + 1);
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    return source;
}

GLuint compile_shader(const char* source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info_log[1024];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "Ошибка компиляции шейдера: %s\n", info_log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path) {
    char* vertex_source = load_shader_source(vertex_shader_path);
    char* fragment_source = load_shader_source(fragment_shader_path);
    if (!vertex_source || !fragment_source) {
        free(vertex_source);
        free(fragment_source);
        return 0;
    }
    GLuint vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
    free(vertex_source);
    free(fragment_source);
    if (!vertex_shader || !fragment_shader) {
        return 0;
    }
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info_log[1024];
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "Ошибка линковки шейдерной программы: %s\n", info_log);
        glDeleteProgram(shader_program);
        return 0;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

void set_uniform(GLuint shader_program, const char* uniform_name, GLint count, GLenum type, const void* value) {
    GLint location = glGetUniformLocation(shader_program, uniform_name);
    if (location == -1) {
        fprintf(stderr, "Ошибка: uniform переменная %s не найдена\n", uniform_name);
        return;
    }
    switch (type) {
    case GL_FLOAT: glUniform1fv(location, count, (const GLfloat*)value); break;
    case GL_FLOAT_VEC3: glUniform3fv(location, count, (const GLfloat*)value); break;
    case GL_FLOAT_MAT4: glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)value); break;
    default: fprintf(stderr, "Ошибка: неподдерживаемый тип uniform переменной\n"); break;
    }
}