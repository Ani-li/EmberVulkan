#pragma once
#include <unordered_map>
#include "eb_model.hpp"

namespace eb
{
	class EbModelCache
	{
	public:
		EbModelCache(EbSetup& ebSetup);

		void registerModel_file(const std::string& filepath, const std::string& name);
		void registerModel_fullScreen(const std::string& name);
		EbModel getModel(const std::string& name);

		std::unordered_map<std::string, EbModel> modelMap{};
	private:
		EbSetup& ebSetup;
	};
}