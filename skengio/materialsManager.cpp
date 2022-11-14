#include "materialsManager.h"

namespace SKEngio {

	void MaterialsManager::OnDestroy() {
		for (MaterialSlot* matSlot : materialSlots) {
			//material has been destroyed previously by entity destructor
			delete matSlot;
		}

		materialSlots.clear();
	}

	Material* MaterialsManager::NewMaterial() {
		MaterialSlot* newMS = new MaterialSlot();
		newMS->id = materialSlots.size();
		newMS->material = new Material();
		materialSlots.push_back( newMS );
		newMS->material->materialID = newMS->id;
		return newMS->material;
	}

	void MaterialsManager::DestroyMaterial(Material* material) {
		int matID = material->materialID;
		if (materialSlots[matID]->useCount < 2) {
			//do not remove this material from vector, otherwise all IDX ill be fu**ed up
			materialSlots[matID]->material->OnDestroy();
			delete materialSlots[matID]->material;
			materialSlots[matID] = nullptr;
		}
		else {
			materialSlots[matID]->useCount--;
		}
	}

}