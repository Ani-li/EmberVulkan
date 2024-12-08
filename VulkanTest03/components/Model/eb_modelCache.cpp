#include "eb_modelCache.hpp"

namespace eb
{
	EbModelCache::EbModelCache(EbSetup& ebSetup) : ebSetup(ebSetup) {}

	void EbModelCache::registerModel_file(const std::string& filepath, const std::string& name)
	{
		if (modelMap.count(name) == 0) {
			std::shared_ptr<EbMesh> mesh = EbMesh::createModelFromFile(ebSetup, filepath);
			auto obj = EbModel::createModel();
			obj.mesh = mesh;
			modelMap[name] = obj;
		}
	}
	void EbModelCache::registerModel_fullScreen(const std::string& name)
	{
		if (modelMap.count(name) == 0) {
			std::shared_ptr<EbMesh> mesh = EbMesh::createFullScreenModel(ebSetup);
			auto obj = EbModel::createModel();
			obj.mesh = mesh;
			modelMap[name] = obj;
		}
	}
	EbModel EbModelCache::getModel(const std::string& name)
	{
		if (modelMap.count(name) > 0)
		{
			return modelMap[name];
		}
		else
		{
			throw std::runtime_error(name + "does not exist!");
		}
	}
}