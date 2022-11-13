#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


//see: https://gamedev.stackexchange.com/questions/124816/quaternion-based-camera-pitch-freaking-out-and-z-axis-drift

namespace SKEngio {
	class Transform3D {
	public:
		void computeModelMatrix() {
			if (dirty) {
				//glm::mat4 rotMat = glm::mat4_cast(glm::inverse(orientationQuat * rotationQuat));
				//modelMatrix = glm::translate(rotMat, position);
				//dirty = false;

				glm::mat4 rotMat = glm::mat4_cast(glm::inverse(orientationQuat * rotationQuat));
				glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
				glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
				modelMatrix = transMat * rotMat * scaleMat;
				dirty = false;

			}
		}

		void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
		{
			computeModelMatrix();
			modelMatrix = parentGlobalModelMatrix * modelMatrix;
		}


		glm::mat4 getLocalModelmatrix() {
			computeModelMatrix();
			return modelMatrix;
		}

		const glm::mat4 getModelMatrix() const
		{
			return modelMatrix;
		}

		void reset() {
			position = glm::vec3(0.0f);
			rotationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			orientationQuat = glm::angleAxis(0.0f, 0.0f, 0.0f, 0.0f);
			modelMatrix = glm::mat4_cast(glm::inverse(orientationQuat * rotationQuat));
			dirty = false;
		}

		void rotate(const float x, const float y, const float z) {
			glm::vec3 rotationDelta = glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z));
			orientationQuat = glm::quat( rotationDelta ) * orientationQuat;
			dirty = true;
		}

		void rotateR(const float x, const float y, const float z) {
			orientationQuat = glm::quat(glm::vec3(x, y, z)) * orientationQuat;
			dirty = true;
		}

		void translate(const float x, const float y, const float z) {
			position += glm::vec3(x, y, z);
			dirty = true;
		}

		void translate(glm::vec3 deltaPos) {
			position += deltaPos;
			dirty = true;
		}

		void setPosition(const float x, const float y, const float z) {
			position = glm::vec3(x, y, z);
			dirty = true;
		}

		void setPosition(glm::vec3 deltaPos) {
			position = deltaPos;
			dirty = true;
		}

		void setScale(const float x, const float y, const float z) {
			scale = glm::vec3(x, y, z);
			dirty = true;
		}

		void setScale(glm::vec3 newScale) {
			scale = newScale;
			dirty = true;
		}

		const glm::vec3 getPosition() {
			return position;
		}


		glm::vec3 getRight()
		{
			computeModelMatrix();
			return glm::vec3(modelMatrix[0]);
		}

		glm::vec3 getUp() 
		{
			computeModelMatrix();
			return glm::vec3(modelMatrix[1]);
		}

		glm::vec3 getBackward() 
		{
			computeModelMatrix();
			return glm::normalize(glm::vec3(modelMatrix[2]));
		}

		glm::vec3 getForward() 
		{
			computeModelMatrix();
			return glm::normalize(glm::vec3(-modelMatrix[2]));
		}

		const bool isDirty() {
			return dirty;
		}

	private:


		bool dirty = true;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		//glm::quat orientationQuat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::quat orientationQuat = glm::angleAxis(0.0f, 0.0f, 0.0f, 0.0f);
		glm::mat4 modelMatrix = glm::mat4_cast(glm::inverse(orientationQuat * rotationQuat));
	};
}
