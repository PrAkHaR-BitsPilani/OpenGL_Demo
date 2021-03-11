#pragma once
#include <vector>
#include <cassert>
#include <glad/glad.h>

///A struct denoting a particular attribute of a vertex.
///
///A vertex in OpenGL has many attributes - position, color, normal, texture coordinates and many more. These attributes are represented in one of the primitive data types.
///Currently suported primitives are GL_FLOAT , GL_UNSIGNED_INT , GL_UNSIGNED BYTE
///@see VertexBufferLayout
struct VertexBufferElement
{
    ///Type of primitive required to represent the attribute.
    unsigned int type;
    ///Number of primitives required to represent the attribute.
    ///@attention Must be 1,2,3,4.
    unsigned int count;
    ///Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
    unsigned char normalized;
    ///returns the size of the given type
    ///@warning \p type needs to be one of the supported types
    static unsigned int getSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT :             return sizeof(GLfloat);
            case GL_UNSIGNED_INT :      return sizeof(GLuint);
            case GL_UNSIGNED_BYTE :     return sizeof(GLubyte);
        }
        assert(false);
        return 0;
    }

};
///Layout of a VertexBuffer
///
///A vertex in openGL has many attributes. A VertexBufferLayout object specifies how different attributes are stored in memory and in which order.
class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
        :m_Stride(0){}
    ///Templated method to push values of type T into the layout.
    ///@param T type of primitive required to specify a particular attribute.
    ///@param count determines how many occurences of T are required to specify a particular attribute.
    ///@attention \p count must be 1,2,3,4.
    template<typename T> void push(unsigned int count);

    ///Returns a std::vector containg all the attributes the layout.
    ///Each attribute is an instance of a VertexBufferElement.
    inline const std::vector<VertexBufferElement> getElements() const& {return m_Elements;}
    ///Returns the total stride of the layout.
    ///Here stride refers to the total size occupied by one vertex.
    inline unsigned int getStride() const { return m_Stride;}
};

template <> inline void VertexBufferLayout::push<float>(unsigned int count)
{
    m_Elements.push_back({GL_FLOAT , count , GL_FALSE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template <> inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_INT , count , GL_FALSE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template <> inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE , count , GL_TRUE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}