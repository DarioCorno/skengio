#include "materialsManager.h"

namespace SKEngio {

	void MaterialsManager::Destroy() {
		for (Material* mat : materials) {
			mat->OnDestroy();
			delete mat;
		}

		materials.clear();
	}

	Material* MaterialsManager::NewMaterial() {
		Material* newM = new Material();
		materials.push_back( newM );
		newM->materialID = materials.size();
		return newM;
	}

}