#pragma once

#include "skengio/utils/singleton.h"
#include "skengio/material.h"
#include <vector>

namespace SKEngio {

	struct MaterialSlot {
		Material* material = nullptr;
		int id = 0;
		int useCount = 1;
	};

	class MaterialsManager final : public Singleton<MaterialsManager> {
	public:
		void OnDestroy();

		Material* NewMaterial();
		void DestroyMaterial(Material* material);


	private:
		std::vector<MaterialSlot*> materialSlots;
	};
}