#pragma once

#include "skengio/utils/singleton.h"
#include "skengio/material.h"
#include <list>

namespace SKEngio {
	class MaterialsManager final : public Singleton<MaterialsManager> {
	public:
		void Destroy();

		Material* NewMaterial();


	private:
		std::list<Material*> materials;
	};
}