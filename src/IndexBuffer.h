#pragma once
///An implementation of GL_ELEMENT_ARRAY_BUFFER
class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    ///Creates an IndexBuffer object
    ///@param data pointer to an array of indices
    ///@param count number of indices
    ///@attention the newly created IndexBuffer is also bound
    ///@warning Following assertions need to be satisfied.
    ///~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
    ///sizeof(GL_uint) == sizeof(unsigned int)
    ///~~~~~~~~~~~~~~~~~~~~~~~~~~~
    IndexBuffer(const unsigned int* data , unsigned int count);
    ~IndexBuffer();
    ///Binds the current IndexBuffer object. The object remains in the bind state unless unbind() is called.
    ///@attention A VertexArray object must be bound at the time of execution of this method for the current object to work.    
    void bind() const;
    ///Unbinds the current IndexBuffer object.
    void unbind() const;

    ///Returns the number of indices in the current object.
    inline unsigned int getCount() const { return m_Count; }
};
