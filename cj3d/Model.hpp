#ifndef CJ_MODEL_HPP
#define CJ_MODEL_HPP

#include <vector>
#include "glm/glm.hpp"
#include "VertexArray.hpp"
#include "BufferObject.hpp"
#include "Shader.hpp"

//Very bad class, need to fix shader being hardcoded, data obv also hardcoded,
//draw function is hard coded as well

namespace cj {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvCoords;
		glm::vec4 color;
	};
	class Model {
	public:
		cj::VertexArray VAO;
		cj::BufferObject<Vertex> VBO = cj::BufferObject<Vertex>(cj::BufferType::VertexBuffer, cj::BufferUsage::Static);
		cj::BufferObject<int> EBO = cj::BufferObject<int>(cj::BufferType::ElementBuffer, cj::BufferUsage::Static);
		glm::mat4 modelMatrix = glm::identity<glm::mat4>();
		cj::Shader shader;

		void loadModel() {
			float vertexData[] = {
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
			};
			unsigned int indices[] = {  // note that we start from 0!
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};  
			Vertex vertices[4];
			for (int q = 0; q < 4; q++) {
				int i = q * 3;
				vertices[q].position = glm::vec3(vertexData[i],vertexData[i+1],vertexData[i+2]);
			}
			VAO.create();
			VAO.bind();

			VBO.create(&vertices, 4);
			VBO.bind();

			EBO.create(&indices, 6);
			EBO.bind();

			VAO.setBufferAttribute(sizeof(glm::vec3));
			VAO.setBufferAttribute(sizeof(glm::vec3));
			VAO.setBufferAttribute(sizeof(glm::vec2));
			VAO.setBufferAttribute(sizeof(glm::vec4));
			VAO.flushAttributes();

			shader.loadFromFiles("shaders/vertex.glsl", "shaders/frag.glsl");
			shader.use();
		}

		void draw() {
			shader.use();
			VAO.bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void destroy() {
			VAO.destroy();
			VBO.destroy();
			EBO.destroy();
		}
	};
}

#endif
