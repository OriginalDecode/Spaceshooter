#include "stdafx.h"

#include "CommonHelper.h"
#include "Texture.h"
#include "TextureContainer.h"


Prism::TextureContainer::~TextureContainer()
{
	for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
	{
		delete it->second;
	}

	myTextures.clear();
}

Prism::Texture* Prism::TextureContainer::GetTexture(const std::string& aFileName)
{
	auto it = myTextures.find(aFileName);

	if (it == myTextures.end())
	{
		LoadTexture(aFileName);
	}

	return myTextures[aFileName];
}

void Prism::TextureContainer::LoadTexture(const std::string& aFileName)
{
	Texture* newTex = new Texture();
	if (aFileName.empty() == false)
	{
		const char* fileEnding = &aFileName[aFileName.size() - 3];
		std::string stringEnding(fileEnding);
		CU::ToLower(stringEnding);
		std::string errorMessage = "Texture file-format not .DDS/.dds in [" + aFileName + "].";
		//DL_ASSERT_EXP(stringEnding == "dds", errorMessage.c_str());
	}
	newTex->LoadTexture(aFileName);

	myTextures[aFileName] = newTex;
}