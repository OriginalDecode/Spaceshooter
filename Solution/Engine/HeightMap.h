#pragma once

namespace Prism
{
	struct HeightMap
	{
		HeightMap(int aWidth, int aHeight, const unsigned char* const someData) : myWidth(aWidth), myHeight(aHeight), myData(someData){}
		~HeightMap(){
			delete[] myData;
		}
		void operator=(HeightMap&) = delete;

		const int myWidth;
		const int myHeight;
		const unsigned char* const myData;
	};
}