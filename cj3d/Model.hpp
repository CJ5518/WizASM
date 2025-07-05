#ifndef CJ_MODEL_HPP
#define CJ_MODEL_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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
		cj::BufferObject<unsigned int> EBO = cj::BufferObject<unsigned int>(cj::BufferType::ElementBuffer, cj::BufferUsage::Static);
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

		void loadModelFromObj(std::string filename) {
			std::vector<Vertex> vertexData;
			std::vector<unsigned int> indices;

			std::ifstream infile(filename);
			std::string line;
			while (std::getline(infile, line)) {
				std::istringstream iss(line);
				std::string lineType;
				iss >> lineType;
				if (lineType == "v") {
					float x,y,z;
					iss >> x >> y >> z;
					Vertex vert;
					vert.position = glm::vec3(x,y,z);
					vertexData.push_back(vert);
				} else if (lineType == "f") {
					int vertId, texId, normId;
					int vertexCount = 0;
					do {
						vertexCount++;
						iss >> vertId;
						iss.ignore(1);
						iss >> texId;
						iss.ignore(1);
						iss >> normId;
						iss.ignore(1);
						indices.push_back(vertId-1);
					} while(!iss.eof());
					if (vertexCount != 3) {
						std::cout << "Obj file " << filename << " isn't triangulated please fix that!\n";
						abort();
					}
				}
			}
			

			//A lot of this is copy pasted should fix that
			VAO.create();
			VAO.bind();

			VBO.create(vertexData.data(), vertexData.size());
			VBO.bind();

			EBO.create(indices.data(), indices.size());
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
			glDrawElements(GL_TRIANGLES, EBO.getItemCount(), GL_UNSIGNED_INT, 0);
		}

		void destroy() {
			VAO.destroy();
			VBO.destroy();
			EBO.destroy();
		}
	};
}

#endif
