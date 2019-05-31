#include "GLBuffers.h"
#include "MyGL.h"

GlBufferBase::GlBufferBase(unsigned int inBufferType)
	: BufferType(inBufferType)
{
	glGenBuffers(1, &BufferId);
}

GlBufferBase::~GlBufferBase()
{
	glDeleteBuffers(1, &BufferId);
}

void GlBufferBase::InitData(void * data, size_t numBytes)
{
	glBindBuffer(BufferType, BufferId);
	glBufferData(BufferType, numBytes, data, GL_STATIC_DRAW);
}

void GlBufferBase::Bind() const
{
	glBindBuffer(BufferType, BufferId);
}

void GlBufferBase::Unbind() const
{
	glBindBuffer(BufferType, -1);
}

//Vertex buffer

GlVertexBufferBase::GlVertexBufferBase() : GlBufferBase(GL_ARRAY_BUFFER)
{
	;
}

GlVertexBufferBase::~GlVertexBufferBase()
{
	;
}

void GlVertexBufferBase::AddAttrib(const GlVbAttrib & attrib)
{
	VertexBufferAttribs.push_back(attrib);
}

void GlVertexBufferBase::Bind() const
{
	GlBufferBase::Bind();

	for (auto& item : VertexBufferAttribs)
	{
		glEnableVertexAttribArray(item.attribId);
		glVertexAttribPointer(item.attribId, item.size, GL_FLOAT, GL_FALSE, item.stride, item.offset);
	}
	GL_ASSERT;
}

void GlVertexBufferBase::Unbind() const
{
	for (auto& item : VertexBufferAttribs)
	{
		glDisableVertexAttribArray(item.attribId);
	}
	GlBufferBase::Unbind();
	GL_ASSERT;
}

//Index Buffer

GlIndexBuffer::GlIndexBuffer() : GlBufferBase(GL_ELEMENT_ARRAY_BUFFER)
{
	;
}

GlIndexBuffer::~GlIndexBuffer()
{
	;
}


//GL VAO
GlVertexBufferNext::GlVertexBufferNext()
{
	glGenVertexArrays(1, &VertexArrayObject);
}

GlVertexBufferNext::~GlVertexBufferNext()
{
	glDeleteVertexArrays(1, &VertexArrayObject);
}

void GlVertexBufferNext::Bind() const
{
	glBindVertexArray(VertexArrayObject);
	GlVertexBufferBase::Bind();
	GL_ASSERT;
}

void GlVertexBufferNext::Unbind() const
{
	GlVertexBufferBase::Unbind();
	glBindVertexArray(-1);
	GL_ASSERT;
}
