#pragma once
#include <D3D11.h>

struct VertexIndexWrapper
{
	DXGI_FORMAT myFormat;
	char* myIndexData;
	int myNumberOfIndices;
	int mySize;
};

