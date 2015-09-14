#pragma once

#include "Vertices.h"

struct VertexDataWrapper
{
	int myNumberOfVertices;
	int mySize;
	int myStride;
	VertexType myType;
	char* myVertexData;

	VertexPosColor* GetDataAsVertexPosColor();
};

inline VertexPosColor* VertexDataWrapper::GetDataAsVertexPosColor()
{
	return reinterpret_cast<VertexPosColor*>(myVertexData);
}

