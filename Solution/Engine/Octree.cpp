#include "stdafx.h"

#include "Octree.h"
#include <sstream>
#include "TreeNode.h"
#include <Vector.h>

Prism::Octree::Octree(int aMaxDepth)
	: myMaxDepth(aMaxDepth)
{
	myRoot = new TreeNode(CU::Vector3<float>(0, 0, 0), 400.f, nullptr, 0, aMaxDepth);
}

Prism::Octree::~Octree()
{
	delete myRoot;
}

void Prism::Octree::Add(Instance* aInstance)
{
	myRoot->InsertObjectDown(aInstance);
}

void Prism::Octree::Update()
{
	//std::stringstream ss;
	//static int numberOfNodes[7];
	//ss << 
	//Engine::GetInstance()->PrintDebugText()
}

void Prism::Octree::GetOccupantsInAABB(const Frustum& aFrustum
		, CU::GrowingArray<Instance*>& aOutArray)
{
	myRoot->GetOccupantsInAABB(aFrustum, aOutArray);
}