#pragma once

struct ID3D11Buffer;

namespace Prism
{
	struct VertexBufferWrapper
	{
		VertexBufferWrapper(){ myVertexBuffer = nullptr; }
		~VertexBufferWrapper()
		{ 
			if (myVertexBuffer != nullptr)
			{
				myVertexBuffer->Release();
				myVertexBuffer = nullptr;
			}
		}
		unsigned int myByteOffset;
		int myNumberOfBuffers;
		int myStartSlot;
		unsigned int myStride;
		ID3D11Buffer* myVertexBuffer;
	};
}