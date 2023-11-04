#include "solidBrush.h"
#include "solidTextBrush.h"

#include <string>

const char *YetAnotherUI::SolidBrush::sVertexShaderCode = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *YetAnotherUI::SolidBrush::sFragmentShaderCode = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 matColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = matColor;\n"
    "}\n\0";

const char *YetAnotherUI::SolidTextBrush::sVertexShaderCode = 
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex> \n"
    "out vec2 TexCoords;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
    "    TexCoords = vertex.zw;\n"
    "}\n";

const char *YetAnotherUI::SolidTextBrush::sFragmentShaderCode = 
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 FragColor;\n"
    "\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "\n"
    "void main() {\n"
    "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "    FragColor = vec4(textColor, 1.0) * sampled;\n"
    "}\n";
