#pragma once

///An implementation of GL_ARRAY_BUFFER
///
///In modern OpenGL, vertices are stored in the form of a buffer. A VertexBuffer object contains the actual vlaues of the attributes of a vertex.
///@see VertexBufferLayout
class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    ///Creates a VertexBuffer object
    ///@param data pointer to the first element
    ///@param size the total size of the data to be inserted
    ///@note VertexBuffer object is bound as soon as it is created.
    ///@attention A VertexArray object must be bound for a VertexBuffer object to work.
    VertexBuffer(const void* data , unsigned int size);
    ~VertexBuffer();

    ///Updates the data in the VertexBuffer object
    ///@param data pointer to the first element of the new data
    ///@param size the total size of the new data to be inserted
    void updateData(const void* data , unsigned int size);

    ///Binds the current VertexBuffer object. The object remains in the bind state unless unbind() is called or the VertexArray object assosciated with the current object is unbound.
    ///@attention A VertexArray object must be bound at the time of execution of this method for the current object to work.
    void bind() const;
    ///Unbinds the current VertexBuffer object
    void unbind() const;
};
