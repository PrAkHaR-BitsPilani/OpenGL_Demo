#include "Texture.h"

Texture::Texture(const std::string& filePath)
    :m_RendererID(0) , m_filePath(filePath) , m_localBuffer(nullptr) ,  m_height(0) , m_width(0) , m_BPP(0)
{
        stbi_set_flip_vertically_on_load(1); // set to 1 if texture is flipped
        m_localBuffer = stbi_load(filePath.c_str() , &m_width , &m_height , &m_BPP , 4);

       GLCall(glGenTextures(1 , &m_RendererID));

       GLCall(glBindTexture(GL_TEXTURE_2D , m_RendererID));

        // TEXTURE FILTERING
       GLCall(glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR)); // try with GL_NEAREST also
       GLCall(glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR));

        // TEXTURE WRAPPERS
       GLCall(glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE));
       GLCall(glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE)); // try other wrappings also also 

       GLCall(glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGBA8 , m_width , m_height , 0 , GL_RGBA , GL_UNSIGNED_BYTE , m_localBuffer));

       unbind();

        if(m_localBuffer)
            stbi_image_free(m_localBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1 , &m_RendererID));
}

void Texture::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D , m_RendererID));
}

void Texture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D , 0));
}