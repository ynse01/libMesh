#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "font.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library = NULL;

YetAnotherUI::Font::Font(std::string &path)
{
    FT_Error error;
    if (library == NULL) {
        error = FT_Init_FreeType(&library);
        if (error) {
            // Bail out
            std::cout << "ERROR:FREETYPE: Could not init FreeType library" << std::endl;
            return;
        }
        mFace = malloc(sizeof(FT_FaceRec));
        FT_Face face = reinterpret_cast<FT_Face>(mFace);
        error = FT_New_Face(library, path.c_str(), 0, &face);
        if (error == FT_Err_Unknown_File_Format) {
            // Not supported font file
            std::cout << "ERROR:FREETYPE: Unsupported font file format for: " << path << std::endl;
            return;
        } else if (error) {
            // Something else went wrong.
            std::cout << "ERROR:FREETYPE: Error loading font from: " << path << std::endl;
            return;
        }
        FT_Set_Pixel_Sizes(face, 0, 48);

        mCharacters = std::map<char, Character>();
    }
}

YetAnotherUI::Font::~Font()
{
    FT_Face face = reinterpret_cast<FT_Face>(mFace);
    FT_Done_Face(face);
    mFace = NULL;
}

void YetAnotherUI::Font::Initialize()
{
    if (!mCharacters.empty()) {
        return;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Face face = reinterpret_cast<FT_Face>(mFace);
    for (unsigned char c= 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR:FREETYPE: Fload the load glyph" << std::endl;
            continue;
        }
        // Generate a texture per character
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            textureId, 
            {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x
        };
        mCharacters.insert(std::pair<char, Character>(c, character));
    }
}

void YetAnotherUI::Font::Render()
{
    // Nothing to be done here.
}

void YetAnotherUI::Font::Destroy()
{
    for(unsigned char c = 0; c < 128; c++) {
        Character ch = mCharacters[c];
        glDeleteTextures(1, &ch.TextureId);
    }
    mCharacters.clear();
}
