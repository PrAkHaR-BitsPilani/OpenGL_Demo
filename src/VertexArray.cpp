#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1 , &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall((glDeleteVertexArrays(1 , &m_RendererID)));
}

void VertexArray::addBuffer(const VertexBuffer& vertexBuffer , const VertexBufferLayout& layout)
{
    bind();
    vertexBuffer.bind();
    unsigned int offset = 0;
    const auto& elements = layout.getElements();
    for(unsigned int i = 0 ; i < elements.size() ; i++)
    {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type,
        elements[i].normalized, layout.getStride() , (const void*)offset)); 
        offset += elements[i].count * VertexBufferElement::getSizeOfType(elements[i].type);
    }
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}