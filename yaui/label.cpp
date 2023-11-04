#include <glad/glad.h>

#include "label.h"

YetAnotherUI::Label::Label(Font &font, std::string &text, SolidTextBrush& brush, float scale)
 : mFont(font), mText(text), mBrush(brush), mScale(scale)
{
}

void YetAnotherUI::Label::setText(const std::string &text)
{
    mText = text;
}

void YetAnotherUI::Label::Initialize()
{
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mBrush.Initialize();
    mFont.Initialize();
}

void YetAnotherUI::Label::Render()
{
    mBrush.Render();
    mFont.Render();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mVAO);

    float x = mPosition.x;
    float y = mPosition.y;

    // Iterate through all characters
    char previous = 0;
    for (std::string::const_iterator c = mText.begin(); c != mText.end(); c++) {
        Character ch = mFont.getCharacter(*c);

        float xpos = x + ch.Bearing[0] * mScale;
        float ypos = y - (ch.Size[1] - ch.Bearing[1]) * mScale;

        xpos += mFont.getKerning(*c, previous) * mScale;

        float w = ch.Size[0] * mScale;
        float h = ch.Size[1] * mScale;

        // Update the VBO for each character
        float vertices[6][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos,     0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },

            { xpos, ypos + h,     0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureId);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Move to the next charcter
        x += ch.Advance * mScale;
        previous = *c;
    }
    // Restore state
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void YetAnotherUI::Label::Destroy()
{
    if (mVAO > 0) glDeleteVertexArrays(1, &mVAO);
    if (mVBO > 0) glDeleteBuffers(1, &mVBO);
    mVAO = 0;
    mVBO = 0;
}
