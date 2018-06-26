#include "RenderPasses.h"
#include "Shaders.h"
#include "StaticGeo.h"

#include "MyGL.h"
#include "Math.h"
#include "Constant.h"


RenderPasses* GRenderPasses = nullptr;

void RenderPasses::RenderFullScreen(const BadgeImage& Texture)
{
	RenderFullScreen(Texture.TextureId);
}

void RenderPasses::RenderFullScreen(unsigned int Texture)
{
	ShaderPrograms::SetProgram(SP_FullScreen);
	glBindTexture(GL_TEXTURE_2D, Texture);
	UnitCube->Draw();
}

void RenderPasses::RenderImageBox(const BadgeImage& Texture, const glm::mat4& ObjectMatrix, bool EnableAlpha, const glm::vec3& colorMod)
{
	auto programid = ShaderPrograms::SetProgram(SP_ImageBox);

	glm::mat4 ViewProjection = glm::ortho(0.0f,(float)SIZE_X,0.0f,(float)SIZE_Y,0.1f,100.0f); //TODO
	ViewProjection = glm::translate(ViewProjection,glm::vec3(0.0f,0.0f,-1.0f));

	static GLuint ObjectMatrixId = glGetUniformLocation(programid, "Transform");
	static GLuint ProjectionMatrixId = glGetUniformLocation(programid, "Projection");
	static GLuint colorModId = glGetUniformLocation(programid, "ColorMod");

	glUniformMatrix4fv(ObjectMatrixId, 1, GL_FALSE, glm::value_ptr(ObjectMatrix));
	glUniformMatrix4fv(ProjectionMatrixId, 1, GL_FALSE, glm::value_ptr(ViewProjection));
	glUniform3f(colorModId, colorMod.x, colorMod.y, colorMod.z);

	glBindTexture(GL_TEXTURE_2D, Texture.TextureId);

	if(EnableAlpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

#if 0
	glm::mat4 mvp = ViewProjection * ObjectMatrix;
	glm::vec4 p1 = mvp * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 p2 = mvp * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p3 = mvp * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p4 = mvp * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
#endif 
	UnitCube->Draw();

	if(EnableAlpha)
	{
		glDisable(GL_BLEND);
	}

}

void RenderPasses::CheckForError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "GL Error: " << gluErrorString(err) << std::endl;
	}
}
#if USE_GLEW
void RenderPasses::InitGL(int* argc, char *argv[])
{
	int win;

	glutInit(argc, argv);

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(SIZE_X, SIZE_Y);
	win = glutCreateWindow("RasterBadge");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glDisable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(0, SIZE_X, 0, SIZE_Y);
	glViewport(0, 0, SIZE_X, SIZE_Y);

	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	UnitCube = new UnitCubeGeo();

}

void RenderPasses::DestroyGL()
{
	if (UnitCube)
		delete UnitCube;
}
#else //EGL
void RenderPasses::InitGL()
{


}

void RenderPasses::DestroyGL()
{
}
#endif


