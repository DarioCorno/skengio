#pragma once

#ifndef SK_TRANSFORM_
#define SK_TRANSFORM_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const glm::vec3 XVector = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 YVector = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ZVector = glm::vec3(0.0f, 0.0f, 1.0f);

namespace SKEngio {

	class Transform {
	protected:

		glm::mat4 modelMatrix = glm::mat4(1.0f);


	public:

		void resetTransforms() {
			modelMatrix = glm::mat4(1.0f);
		}

		void Translate(const glm::vec3& newPosition)
		{
			modelMatrix = glm::translate(modelMatrix, newPosition);
		}

		void Rotate(const float angle, const glm::vec3& axis)
		{
			modelMatrix = glm::rotate(modelMatrix, angle, axis);
		}

		void Scale(const glm::vec3& newScale)
		{
			modelMatrix = glm::scale(modelMatrix, newScale);
		}

		const glm::vec3& getGlobalPosition() const
		{
			return glm::vec3( modelMatrix[3] );
		}

		const glm::mat4& getModelMatrix() const
		{
			return modelMatrix;
		}

		glm::vec3 getRight() const
		{
			return glm::vec3(  modelMatrix[0] );
		}


		glm::vec3 getUp() const
		{
			return glm::vec3( modelMatrix[1] );
		}

		glm::vec3 getBackward() const
		{
			return glm::vec3( modelMatrix[2] );
		}

		glm::vec3 getForward() const
		{
			return glm::vec3(  -modelMatrix[2] );
		}

	};
}

#endif
