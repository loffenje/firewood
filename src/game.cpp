#include "os_platform.h"

global_var bool initialized = false;
global_var RendererData renderer_data = {};

extern "C" UPDATE_AND_RENDER(updateAndRender)
{
    if (!initialized) {
        const char *vertex = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "layout (location = 2) in vec3 aTexCoord;\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "uniform mat4 transform;\n"
            "void main()\n"
            "{\n"
            "gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "ourColor = aColor;\n"
            "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
            "}\0";
    
        const char *fragment = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "uniform sampler2D texture1;\n"
            "void main()\n"
            "{\n"
            "   FragColor = texture(texture1, TexCoord);\n"
            "}\n\0";        

        auto shader = Shader::instance();
        shader->createProgram(vertex, fragment);
        initialized = true; 
    }   


   return 0; 
}
