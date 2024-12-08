#include "eb_model.hpp"

namespace eb
{
	EbModel::EbModel() {}

	EbModel EbModel::createModel()
	{
		return EbModel{};
	}

	void EbModel::draw(VkCommandBuffer cmd)
	{
		return mesh->draw(cmd);
	}
}