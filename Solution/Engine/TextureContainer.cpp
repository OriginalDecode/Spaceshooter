#include "stdafx.h"
#include "TextureContainer.h"
#include "Texture.h"

TextureContainer::TextureContainer()
{
}


TextureContainer::~TextureContainer()
{
}

Texture* TextureContainer::GetTexture(const std::string& aFileName)
{
	auto it = myTextures.find(aFileName);

	if (it == myTextures.end())
	{
		LoadTexture(aFileName);
	}

	return myTextures[aFileName];
}

void TextureContainer::LoadTexture(const std::string& aFileName)
{
	Texture* newTex = new Texture();
	newTex->LoadTexture(aFileName);

	myTextures[aFileName] = newTex;
}