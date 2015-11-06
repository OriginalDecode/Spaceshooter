#include "stdafx.h"
#include "HeightMap.h"
#include "HeightMapFactory.h"
#include "Tga32.h"

namespace Prism
{
	HeightMap* HeightMapFactory::Create(const char* aFilePath)
	{
		Tga32::Image* image = Tga32::Load(aFilePath);

		int width = image->myWidth;
		int height = image->myHeight;

		unsigned char* data = new unsigned char[width * height];

		for (int i = 0; i < width * height; ++i)
		{
			data[i] = image->myImage[i * 4];
		}

		return new HeightMap(width, height, data);
	}
}