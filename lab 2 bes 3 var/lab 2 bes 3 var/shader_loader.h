#pragma once
#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
class Shader_loader {
public:
	GLuint oneLinkProgram(std::string fragment_shader_path = "./fragment_shader.glsl",
		std::string vertex_shader_path = "./vertex_shader.glsl") {
		std::string frag_string, vert_string;
		if (!load_shader_source(fragment_shader_path, frag_string)) {
			std::cerr << "Error with file fragment_shader!\n";
			return 1;
		}
		if (!load_shader_source(vertex_shader_path, vert_string)) {
			std::cerr << "Error with file vertex_shader!\n";
			return 1;
		}
		const char* frag = frag_string.c_str();
		const char* vert = vert_string.c_str();
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &frag, NULL);
		glCompileShader(fs);
		check_compile(fs, GL_FRAGMENT_SHADER);
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vert, NULL);
		glCompileShader(vs);
		check_compile(vs, GL_VERTEX_SHADER);
		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, fs);
		glAttachShader(shader_program, vs);
		glLinkProgram(shader_program);
		glDeleteShader(fs);
		glDeleteShader(vs);
		return shader_program;
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x) {
		glUniform1f(get_location(shader_program, attribute_name), x);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y) {
		glUniform2f(get_location(shader_program, attribute_name), x, y);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y, float z) {
		glUniform3f(get_location(shader_program, attribute_name), x, y, z);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y, float z, float a) {
		glUniform4f(get_location(shader_program, attribute_name), x, y, z, a);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x) {
		glUniform1i(get_location(shader_program, attribute_name), x);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y) {
		glUniform2i(get_location(shader_program, attribute_name), x, y);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y, GLint z) {
		glUniform3i(get_location(shader_program, attribute_name), x, y, z);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y, GLint z, GLint a) {
		glUniform4i(get_location(shader_program, attribute_name), x, y, z, a);
	}
	enum Fv_things { FV1, FV2, FV3, FV4 };
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, GLfloat* value, Fv_things fv_thing) {
		GLint loc = get_location(shader_program, attribute_name);
		switch (fv_thing) {
		case FV1: glUniform1fv(loc, count, value); break;
		case FV2: glUniform2fv(loc, count, value); break;
		case FV3: glUniform3fv(loc, count, value); break;
		case FV4: glUniform4fv(loc, count, value); break;
		}
	}
	enum Iv_things { IV1, IV2, IV3, IV4 };
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, GLint* value, Iv_things iv_thing) {
		GLint loc = get_location(shader_program, attribute_name);
		switch (iv_thing) {
		case IV1: glUniform1iv(loc, count, value); break;
		case IV2: glUniform2iv(loc, count, value); break;
		case IV3: glUniform3iv(loc, count, value); break;
		case IV4: glUniform4iv(loc, count, value); break;
		}
	}
	enum Matrix_things { M2, M3, M4 };
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, const GLfloat* value, GLboolean transpose, Matrix_things m_thing) {
		GLint loc = get_location(shader_program, attribute_name);
		switch (m_thing) {
		case M2: glUniformMatrix2fv(loc, count, transpose, value); break;
		case M3: glUniformMatrix3fv(loc, count, transpose, value); break;
		case M4: glUniformMatrix4fv(loc, count, transpose, value); break;
		}
	}
private:
	GLint get_location(GLuint shader_program, const std::string& attribute_name) {
		return glGetUniformLocation(shader_program, attribute_name.c_str());
	}
	bool load_shader_source(const std::string& path, std::string& out_code) {
		std::ifstream file(path);
		if (!file.is_open()) return false;
		out_code.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return true;
	}
	void check_compile(GLuint shader, GLenum type) {
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", type, infoLog);
		}
	}
};
#pragma once
