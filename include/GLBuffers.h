#pragma once
#include <vector>

struct GlVbAttrib
{
	unsigned int attribId;
	int size;
	std::size_t stride;
	void* offset;
};

class GlBufferBase
{
protected:
	GlBufferBase(unsigned int inBufferType);
	virtual ~GlBufferBase();
	unsigned int BufferId;
	unsigned int BufferType;
public:
	void InitData(void* data, std::size_t numBytes);
	virtual void Bind() const;
	virtual void Unbind() const;
};

class GlVertexBufferBase : public GlBufferBase
{
public:
	GlVertexBufferBase();
	~GlVertexBufferBase();

	void AddAttrib(const GlVbAttrib& attrib);
	void Bind() const override;
	void Unbind() const override;

protected:
	std::vector<GlVbAttrib> VertexBufferAttribs;
};
#if _MSC_VER || __SWITCH__
//VBO and VAO for GL 4.5
class GlVertexBufferNext : public GlVertexBufferBase
{
public:
	GlVertexBufferNext();
	~GlVertexBufferNext();
	
	void Bind() const override;
	void Unbind() const override;
protected:
	unsigned int VertexArrayObject;
};

typedef GlVertexBufferNext GlVertexBuffer;
#else
typedef GlVertexBufferBase GlVertexBuffer;

#endif

class GlIndexBuffer : public GlBufferBase
{
public:
	GlIndexBuffer();
	~GlIndexBuffer();
};