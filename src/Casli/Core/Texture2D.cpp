#include "Texture2D.h"

Texture2D::Texture2D(TEXTURE2D_DESC *desc, SUBRESOURCE_DATA *sd)
	: IBuffer(sizeof(float), desc->Width * desc->Height, sd->pSysMem), width(desc->Width), height(desc->Height),
	mipLevels(desc->MipLevels), format(desc->Format)
{

}

Texture2D::~Texture2D()
{
}

void Texture2D::GenerateMips()
{
}
