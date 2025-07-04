#ifndef CJ_OBJ_READER
#define CJ_OBJ_READER

#include <vector>
#include "glm/glm.hpp"


struct ObjVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoords;
	glm::vec4 color;
};



#endif
