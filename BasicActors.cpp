#include "BasicActors.h"
#include "RenderPasses.h"
#include "Constant.h"

bool MovingActor::Integrate(float tick)
{
	Position += Inertia*tick/1000.0f;
	return true;
}

ImageActor::ImageActor(const std::string& image)
{
	Image = TextureManager::GetTexture(image);
}

void ImageActor::Render()
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans,glm::vec3(Position.x,Position.y,0.0f));
	trans = glm::rotate(trans, Rotation, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, glm::vec3(Image.Width*0.5f, Image.Height*0.5f, 1.0f)); //0.5 because the unit cube is -1 to 1
	trans = glm::scale(trans, glm::vec3(Scale,Scale,1.0f));
	trans = glm::scale(trans, glm::vec3(SCALE,SCALE,1.0f));
	RenderPasses::GetRenderPasses().RenderImageBox(Image,trans,true);
}
