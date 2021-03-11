#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
///Handles VertexArrays in an efficient way.
class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    ///Binds \p vertexBuffer to the current VertexArray object where the layout is specified by \p layout.
    void addBuffer(const VertexBuffer& vertexBuffer , const VertexBufferLayout& layout);

    ///Binds the current VertexArray object.
    void bind() const;
    ///Unbinds the current VertexArray object.
    ///Calling this method also unbinds the VertexBuffer object assosciated with the current object.
    ///The assosciated IndexBuffer object (if any) still remains in bound state.
    ///@see VertexBuffer::unbind() IndexBuffer::unbind()
    void unbind() const;
};


