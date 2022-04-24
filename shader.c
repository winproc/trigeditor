#include <stdio.h>

#include "shader.h"

int read_glsl(const char* filename, char* buffer) {
    FILE* shader_file;
    shader_file = fopen(filename, "r");

    if (!shader_file) {
        return 0;
    } else {
        size_t read_bytes = fread(buffer, sizeof(char), 1024, shader_file);
        
        fclose(shader_file);

        return read_bytes;
    }
}