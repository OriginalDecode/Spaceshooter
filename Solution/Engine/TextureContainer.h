#pragma once
#include <unordered_map>
class Texture;

class TextureContainer
{
public:
	TextureContainer();
	~TextureContainer();

	Texture* GetTexture(const std::string& aFileName);

private:
	void LoadTexture(const std::string& aFileName);

	std::unordered_map<std::string, Texture*> myTextures;
};

