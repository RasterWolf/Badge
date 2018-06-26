#include "RenderPasses.h"
#include "Shaders.h"
#include "StaticGeo.h"

#include "MyGL.h"
#include "Math.h"
#include "Constant.h"



void RenderPasses::RenderFullScreen(const BadgeImage& Texture)
{
	RenderFullScreen(Texture.TextureId);
}

void RenderPasses::RenderFullScreen(unsigned int Texture)
{
	ShaderPrograms::SetProgram(SP_FullScreen);
	glBindTexture(GL_TEXTURE_2D, Texture);
	UnitCube.Draw();
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

#if defined(DEBUG)
	glm::mat4 mvp = ViewProjection * ObjectMatrix;
	glm::vec4 p1 = mvp * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 p2 = mvp * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p3 = mvp * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 p4 = mvp * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
#endif 
	UnitCube.Draw();

	if(EnableAlpha)
	{
		glDisable(GL_BLEND);
	}

}
