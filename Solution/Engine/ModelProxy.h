#pragma once

namespace Prism
{
	class Effect;
	class Model;
	class RenderProcessTarget;

	class ModelProxy
	{
	public:
		ModelProxy();

		void Render(const CU::Matrix44<float>& aOrientation, RenderProcessTarget& aRenderProcessTarget);

		void SetModel(Model* aModel);

		Effect* GetEffect();
		void SetEffect(Effect* aEffect);

		bool IsLoaded() const;
	private:
		Model* myModel;
	};

	inline bool ModelProxy::IsLoaded() const
	{
		if (myModel == nullptr)
		{
			return false;
		}

		return true;
	}
}

