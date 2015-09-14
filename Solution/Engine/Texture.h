#pragma once
#include <string>
struct ID3D11ShaderResourceView;

class Texture
{
public:
	Texture();
	~Texture();

	bool LoadTexture(const std::string& aFilePath);
	const std::string& GetFileName() const;
	ID3D11ShaderResourceView* GetShaderView();

private:
	std::string myFileName;
	ID3D11ShaderResourceView* myTexture;
};

