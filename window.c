#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>

#include "shader.h"

typedef struct {
    float x;
    float y;
} Vector2;

// buffer array data

GLfloat vertex_info[2048] = {};
int tracker_buf = 0;

GLfloat button_vert_info[2048] = {};
int button_vert_tracker = 0;

// data writing helpers

void write_vertex_info(GLfloat n) {
    vertex_info[tracker_buf] = n;
    tracker_buf++;
    
}

void draw_ui_quad(Vector2 pVert, Vector2 dVert, Vector2 tVert, Vector2 qVert) {
    button_vert_info[button_vert_tracker] = pVert.x;
    button_vert_info[button_vert_tracker + 1] = pVert.y;

    button_vert_tracker += 2;

    button_vert_info[button_vert_tracker] = dVert.x;
    button_vert_info[button_vert_tracker + 1] = dVert.y;

    button_vert_tracker += 2;

    button_vert_info[button_vert_tracker] = tVert.x;
    button_vert_info[button_vert_tracker + 1] = tVert.y;

    button_vert_tracker += 2;

    button_vert_info[button_vert_tracker] = pVert.x;
    button_vert_info[button_vert_tracker + 1] = pVert.y;

    button_vert_tracker += 2;

    button_vert_info[button_vert_tracker] = tVert.x;
    button_vert_info[button_vert_tracker + 1] = tVert.y;

    button_vert_tracker += 2;

    button_vert_info[button_vert_tracker] = qVert.x;
    button_vert_info[button_vert_tracker + 1] = qVert.y;

    button_vert_tracker += 2;
}

// shader compilation helper functions

GLuint compile_glsl_vert(const char* buffer) {
    GLuint shader_name;
    shader_name = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(shader_name, 1, &buffer, NULL);
    glCompileShader(shader_name);

    int success;
    char message[1024];
    glGetShaderiv(shader_name, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader_name, 1024, NULL, message);
        printf("Vertex shader compilation failed:\n%s", message);

        return 0;
    }

    return shader_name;
}

GLuint compile_glsl_frag(const char* buffer) {
    GLuint shader_name = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader_name, 1, &buffer, NULL);
    glCompileShader(shader_name);

    int success;
    char message[1024];
    glGetShaderiv(shader_name, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader_name, 1024, NULL, message);
        printf("Fragment shader compilation failed:\n%s", message);

        return 0;
    }

    return shader_name;
}

GLuint link_shaders(GLuint shader_vert, GLuint shader_frag) {
    GLuint shader_prog;
    shader_prog = glCreateProgram();

    glAttachShader(shader_prog, shader_vert);
    glAttachShader(shader_prog, shader_frag);
    glLinkProgram(shader_prog);

    return shader_prog;
    
}

GLuint create_program(const char* fragment_shader_sn, const char* vertex_shader_sn) {
    char fragment_shader[1024];
    char vertex_shader[1024];

    GLuint frag_name;
    GLuint vert_name;

    if (!read_glsl(fragment_shader_sn, fragment_shader) || !read_glsl(vertex_shader_sn, vertex_shader)) {
        puts("failed to read shaders");
        return 0;

    } else {
        frag_name = compile_glsl_frag(fragment_shader);
        vert_name = compile_glsl_vert(vertex_shader);

        if (!frag_name || !vert_name) {
            return 0;
        }
    }

    return link_shaders(vert_name, frag_name);
}

void display(GLuint program_name, GLuint trig_buf_name, GLuint ui_name) {

    glUseProgram(program_name);

    glBindBuffer(GL_ARRAY_BUFFER, trig_buf_name);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0x0);
    glEnableVertexAttribArray(0);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * tracker_buf, vertex_info);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 40);
    
    glBindBuffer(GL_ARRAY_BUFFER, ui_name);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0x0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 40);
    
}

void on_mouse_button_pressed(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        int wx, wy;
        glfwGetFramebufferSize(window, &wx, &wy);

        double normalized_x, normalized_y;
        normalized_x = ((x/wx - 0.5) * 2);
        normalized_y = ((y/wy - 0.5) * 2) * -1.0;

        write_vertex_info((GLfloat)normalized_x);
        write_vertex_info((GLfloat)normalized_y);
        
    }
}

int main(void) {
    GLFWwindow* window;
    
    if (!glfwInit()) {
        return -1;
    }

    RECT window_size;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &window_size, 0);

    const int window_width = window_size.right - window_size.left;
    const int window_height = window_size.bottom - window_size.top;

    window = glfwCreateWindow(window_width, window_height - 30, "Geometric Editor", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    GLenum error = glewInit();
    if (error != GLEW_OK) {
        puts(glewGetErrorString(error));
        return 0;
    }

    glfwSetMouseButtonCallback(window, on_mouse_button_pressed);

    // buffer data preprocessing

    Vector2 st_p;
    Vector2 st_d;
    Vector2 st_t;
    Vector2 st_q;

    st_p.x = -1.0;
    st_p.y = -0.95;

    st_d.x = -1.0;
    st_d.y = -1.0;

    st_t.x = 1.0;
    st_t.y = -1.0;

    st_q.x = 1.0;
    st_q.y = -0.95;

    draw_ui_quad(st_p, st_d, st_t, st_q); // capture vertices button

    // buffer generation

    GLuint vbuffer_trig;
    glGenBuffers(1, &vbuffer_trig);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_trig);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_info), vertex_info, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0x0);
    glEnableVertexAttribArray(0);

    // UI

    GLuint vbuffer_ui;
    glGenBuffers(1, &vbuffer_ui);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer_ui);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button_vert_info), button_vert_info, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0x0);
    glEnableVertexAttribArray(0);

    // shaders

    GLuint mainProg = create_program("./shaders/checkerboard_frag.glsl", "./shaders/plainpos_vert.glsl");
    
    while (!glfwWindowShouldClose(window))
    {
        
        glClear(GL_COLOR_BUFFER_BIT);
        display(mainProg, vbuffer_trig, vbuffer_ui);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}