#pragma once

namespace CommonUtilities
{
	namespace Intersection
	{
		struct AABB;
	}
}

namespace Prism
{
	class Instance;
	class TreeNode;

	class Octree
	{
	public:
		Octree(int aMaxDepth);
		~Octree();

		void Add(Instance* aInstance);
		void Update();
		void GetOccupantsInAABB(const CommonUtilities::Intersection::AABB& aAABB, CU::GrowingArray<Instance*>& aOutArray);


	private:
		void operator=(Octree&) = delete;

		const int myMaxDepth;
		TreeNode* myRoot;
	};
}