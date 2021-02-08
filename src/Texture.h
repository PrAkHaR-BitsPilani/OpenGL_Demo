#pragma once
#include <string>
#include "Renderer.h"
#include "external/stb_image/stb_image.h"

class Texture
{
private:
#include <string>
    unsigned int m_RendererID;
    const std::string& m_filePath;
    unsigned char* m_localBuffer;
    int m_height , m_width , m_BPP;
public:
    Texture(const std::string& filePath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getHeight() const { return m_height;}
    inline int getWidth() const { return m_width;}
};
