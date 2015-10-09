#include "stdafx.h"

#include "EngineEnums.h"
#include "Frustum.h"
#include "Instance.h"
#include <Intersection.h>
#include "TreeNode.h"

#include <sstream>

std::stringstream ss;
std::stringstream ss2;
std::stringstream ss3;

Prism::TreeNode::TreeNode(const CU::Vector3<float>& aPosition, float aHalfWidth, TreeNode* aParent
		, int aDepth, int aMaxDepth)
	: myPosition(aPosition)
	, myHalfWidth(aHalfWidth)
	, myParent(aParent)
	, myDepth(aDepth)
	, myMaxDepth(aMaxDepth)
	, myLooseness(1.f)
	, myObjectsDynamic(16)
	, myObjectsStatic(16)
{
	myLooseWidth = myHalfWidth * myLooseness;
}

Prism::TreeNode::~TreeNode()
{
}

void Prism::TreeNode::InsertObjectDown(Instance* anObject)
{
	bool straddle = false;
	int childIndex = 0;

	for (int i = 0; i < 3; ++i)
	{
		float delta = 0;

		if (i == 0)
		{
			delta = anObject->GetPosition().x - myPosition.x;
		}
		else if (i == 1)
		{
			delta = anObject->GetPosition().y - myPosition.y;
		}
		else if (i == 2)
		{
			delta = anObject->GetPosition().z - myPosition.z;
		}

		if (abs(delta) + myHalfWidth * (myLooseness - 1.f) <= anObject->GetRadius())
		{
			straddle = true;
			break;
		}

		if (delta > 0.0f)
		{
			childIndex |= (1 << i);
		}
	}
	
	if (straddle == false && myDepth < myMaxDepth - 1)
	{
		if (myChildren[childIndex] == nullptr)
		{
			myChildren[childIndex] = SpawnChild(childIndex);
		}

		myChildren[childIndex]->InsertObjectDown(anObject);
	}
	else
	{
		if (anObject->GetOctreeType() == eOctreeType::DYNAMIC)
		{
			myObjectsDynamic.Add(anObject);
		}
		else if (anObject->GetOctreeType() == eOctreeType::STATIC)
		{
			myObjectsStatic.Add(anObject);
		}
		else
		{
			DL_ASSERT("Unknown octree type.");
		}
	}
}

Prism::TreeNode* Prism::TreeNode::SpawnChild(int anId)
{
	CU::Vector3<int> dir;

	switch (anId)
	{
	case 0:
		dir = CU::Vector3<int>(-1, -1, -1);
		break;
	case 1:
		dir = CU::Vector3<int>(+1, -1, -1);
		break;
	case 2:
		dir = CU::Vector3<int>(-1, +1, -1);
		break;
	case 3:
		dir = CU::Vector3<int>(+1, +1, -1);
		break;
	case 4:
		dir = CU::Vector3<int>(-1, -1, +1);
		break;
	case 5:
		dir = CU::Vector3<int>(+1, -1, +1);
		break;
	case 6:
		dir = CU::Vector3<int>(-1, +1, +1);
		break;
	case 7:
		dir = CU::Vector3<int>(+1, +1, +1);
		break;
	default:
		DL_ASSERT("bad index!");
		break;
	}

	CU::Vector3<float> pos(myPosition);
	pos.x += dir.x * myHalfWidth / 2.f;
	pos.y += dir.y * myHalfWidth / 2.f;
	pos.z += dir.z * myHalfWidth / 2.f;
	return new TreeNode(pos, myHalfWidth / 2.f, this, myDepth + 1, myMaxDepth);
}

bool Prism::TreeNode::NodeVsAABB(const CommonUtilities::Intersection::AABB& aAABB)
{
	CU::Vector3<float> myCenter(myPosition.x, myPosition.y, myPosition.z);

	CU::Vector3<float> otherCenter(aAABB.myMinPos.x + aAABB.myExtents.x / 2.f, aAABB.myMinPos.y + aAABB.myExtents.y / 2.f, aAABB.myMinPos.z + aAABB.myExtents.z / 2.f);


	if (abs(myCenter.x - otherCenter.x) > (myHalfWidth + aAABB.myExtents.x / 2.f)) return false;
	if (abs(myCenter.y - otherCenter.y) > (myHalfWidth + aAABB.myExtents.y / 2.f)) return false;
	if (abs(myCenter.z - otherCenter.z) > (myHalfWidth + aAABB.myExtents.z / 2.f)) return false;

	return true;
}

int totalTreeNodes = 0;
int maxNumOfDynamic = 0;
int maxDynamicDepth = 0;
int maxNumOfStatic = 0;
int maxStaticDepth = 0;


void Prism::TreeNode::GetOccupantsInAABB(const Frustum& aFrustum
	, CU::GrowingArray<Instance*>& aOutArray)
{
	if (myDepth == 0)
	{
		ss << "Total: " << totalTreeNodes;
		ss2 << "Max Dynamic: " << maxNumOfDynamic << " depth: " << maxDynamicDepth;
		ss3 << "Max Static: " << maxNumOfStatic << " depth: " << maxStaticDepth;
		Engine::GetInstance()->PrintDebugText(ss.str(), { 700.f, -700.f });
		Engine::GetInstance()->PrintDebugText(ss2.str(), { 700.f, -730.f });
		Engine::GetInstance()->PrintDebugText(ss3.str(), { 700.f, -760.f });
		ss.clear();
		ss.str(std::string());
		ss2.clear();
		ss2.str(std::string());
		ss3.clear();
		ss3.str(std::string());
		totalTreeNodes = 0;
		maxNumOfDynamic = 0;
		maxNumOfStatic = 0;
	}
	++totalTreeNodes;
	if (myObjectsDynamic.Size() > maxNumOfDynamic)
	{
		maxNumOfDynamic = myObjectsDynamic.Size();
		maxDynamicDepth = myDepth;
	}
	if (myObjectsStatic.Size() > maxNumOfStatic)
	{
		maxNumOfStatic = myObjectsStatic.Size();
		maxStaticDepth = myDepth;
	}
	for (int i = 0; i < myObjectsDynamic.Size(); ++i)
	{
		if (aFrustum.Inside(myObjectsDynamic[i]->GetPosition(), myObjectsDynamic[i]->GetRadius()) == true)
		{
			aOutArray.Add(myObjectsDynamic[i]);
		}
	}
	for (int i = 0; i < myObjectsStatic.Size(); ++i)
	{
		if (aFrustum.Inside(myObjectsStatic[i]->GetPosition(), myObjectsStatic[i]->GetRadius()) == true)
		{
			aOutArray.Add(myObjectsStatic[i]);
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		if (myChildren[i] != nullptr
			&& CU::Intersection::AABBvsAABB(myChildren[i]->GetMinCorner()
			, myChildren[i]->GetMaxCorner()
			, aFrustum.GetCornerMin()
			, aFrustum.GetCornerMax()) == true)
		{
			ss << i << " ";
			myChildren[i]->GetOccupantsInAABB(aFrustum, aOutArray);
		}
	}
}