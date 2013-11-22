#pragma once

#include "../EngineIncludes.h"
#include <unordered_map>

namespace Oyster
{
	namespace Resources
	{
		struct Manager
		{
			//Expects to be deleted either trough manager or after a clean
			static Oyster::Render::Model* LoadModel(std::string Filename, Matrix Scale);
			static void Clean();

		private:
			static std::unordered_map< std::string, Oyster::Render::ModelInfo*> loadedModels;
		};
	}
}