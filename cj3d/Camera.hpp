#ifndef CJ_CAMERA_HPP
#define CJ_CAMERA_HPP

#include <SDL3/SDL.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace cj {
	class Camera {
	public:
		Camera() {
			front = glm::vec3(0, 0, 1);
			position = glm::vec3(0, 0, -3);
			upVector = glm::vec3(0, 1, 0);
			viewMatrix = glm::identity<glm::mat4>();
		};

		//translates the view matrix based on rotation
		void relativeMove(glm::vec3 cameraMovement, float dt) {
			position -= glm::normalize(glm::cross(front, upVector)) * speed * dt * cameraMovement.x;
			position += front * speed * dt * cameraMovement.z;
			position -= glm::vec3(0, speed * dt, 0) * cameraMovement.y;
			updateViewMatrix();
		}

		//x is yaw, y is pitch, and z is roll
		void rotate(glm::vec3 rotation) {
			yaw += rotation.x * sensitivity;
			pitch += rotation.y * sensitivity;
			if (pitch >= 89.9f)
				pitch = 89.9f;
			if (pitch <= -89.9f)
				pitch = -89.9f;

			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			updateViewMatrix();
		}

		//x and y only
		void rotate(glm::vec2 rotation) {
			rotate(glm::vec3(rotation.x, rotation.y, 0.0f));
		}
		//Call this if you mess with the member variables directly
		void update() {
			rotate(glm::vec3(0));
			relativeMove(glm::vec3(0), 0.0f);
		}

		//Some sfml specific functions
		void processEvent(const SDL_Event& event) {
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (event.button.button == SDL_BUTTON_RIGHT) {
					cameraMouseDownPos = glm::vec2(event.button.x, event.button.y);
				}
			}
		}

		void fixedUpdate() {
			const bool* keyArray = SDL_GetKeyboardState(NULL);
			float speed = 0.0015f;
			if (keyArray[SDL_SCANCODE_LSHIFT]) {
				speed = 10.0f;
			}

			cameraMovement = glm::vec3(0);
			
			if (keyArray[SDL_SCANCODE_A])
				cameraMovement.x = -1.0f * speed;
			if (keyArray[SDL_SCANCODE_D])
				cameraMovement.x = 1.0f * speed;
			if (keyArray[SDL_SCANCODE_W])
				cameraMovement.z = -1.0f * speed;
			if (keyArray[SDL_SCANCODE_S])
				cameraMovement.z = 1.0f * speed;
			if (keyArray[SDL_SCANCODE_Q])
				cameraMovement.y = 1.0f * speed;
			if (keyArray[SDL_SCANCODE_E])
				cameraMovement.y = -1.0f * speed;
		}

		void variableUpdate(float dt) {
			cameraRotation = glm::vec2(0);
			//Mouse delta
			float x,y;
			SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(&x, &y);
			if (mouseButtons & (1 << (SDL_BUTTON_RIGHT-1))) {
				glm::vec2 mousePos = glm::vec2(x, y);
				cameraRotation = (mousePos - cameraMouseDownPos);
				//sf::Mouse::setPosition(sf::Vector2i(cameraMouseDownPos.x, cameraMouseDownPos.y));
				cameraMouseDownPos = mousePos;
			}

			//instancedBuffer.update(matrices.data(), matrices.size(), sizeof(glm::mat4));

			relativeMove(cameraMovement, 1 / dt);
			rotate(cameraRotation);
			update();
		}
	private:
		glm::vec3 cameraMovement;
		glm::vec2 cameraMouseDownPos;
		glm::vec2 cameraRotation;
	public:

		glm::mat4 viewMatrix;

		float speed = 7.0f;
		float sensitivity = 0.1f;

		float yaw = 90.0f;
		float pitch = 0.0f;
		float roll = 0.0f;

		glm::vec3 position, front, upVector;
	private:
		void updateViewMatrix() {
			viewMatrix = glm::lookAt(position, position - front, upVector);
		}
	};
}
#endif
