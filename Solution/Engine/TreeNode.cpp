#include "stdafx.h"

#include "Instance.h"
#include <Intersection.h>
#include "TreeNode.h"


Prism::TreeNode::TreeNode(const CU::Vector3<float>& aPosition, float aHalfWidth, TreeNode* aParent
		, int aDepth, int aMaxDepth)
	: myPosition(aPosition)
	, myHalfWidth(aHalfWidth)
	, myParent(aParent)
	, myDepth(aDepth)
	, myMaxDepth(aMaxDepth)
	, myLooseness(1.f)
	, myObjects(32)
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
		myObjects.Add(anObject);
		//anObject->SetDepth(myDepth);
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

void Prism::TreeNode::GetOccupantsInAABB(const CommonUtilities::Intersection::AABB& aAABB
	, CU::GrowingArray<Instance*>& aOutArray)
{
	for (int i = 0; i < myObjects.Size(); ++i)
	{
		//add check against frustum
		aOutArray.Add(myObjects[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		if (myChildren[i] != nullptr)
		//	&& CU::Intersection::AABBvsAABB(myChildren[i]->GetMinCorner()
		//	, myChildren[i]->GetMaxCorner()
		//	, aFrustum.GetMinCorner()
		//	, aFrustum.GetMaxCorner()) == true)
		{
			myChildren[i]->GetOccupantsInAABB(aAABB, aOutArray);
		}
	}
}