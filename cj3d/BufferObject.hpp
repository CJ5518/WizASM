#ifndef CJ_ELEMENT_ARRAY_BUFFER
#define CJ_ELEMENT_ARRAY_BUFFER


#include "glm/glm.hpp"
#include "GL/glew.h"

namespace cj {

	enum class BufferUsage {
		Static = GL_STATIC_DRAW, //Data that doesn't change
		Dynamic = GL_DYNAMIC_DRAW, //Data that changes sometimes
		Stream = GL_STREAM_DRAW //Data that changes tons of times
	};
	enum class BufferType {
		VertexBuffer = GL_ARRAY_BUFFER,
		ElementBuffer = GL_ELEMENT_ARRAY_BUFFER
	};
	template <typename buffDataType>
	class BufferObject {
	public:
		//Delete default constructor, force supplying usage and buffer type
		BufferObject() = delete;
		~BufferObject() {
			destroy();
		}
		BufferObject(BufferType bufferType, BufferUsage usage) : m_usage(usage),m_bufferType(bufferType) {};

		//Delete those pesky operators!
		BufferObject(const BufferObject&) = delete;
		BufferObject& operator=(const BufferObject&) = delete;

		//Copy that buffer to this buffer
		//Doesn't check if that is of the same type so look out
		void update(const BufferObject& that) {
			glBindBuffer(GL_COPY_READ_BUFFER, that.m_id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, m_id);

			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, that.m_itemCount * sizeof(buffDataType));

			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		}

		void create(void* list, size_t nItems) {
			if (!m_id)
				glGenBuffers(1, &m_id);
			
			glBindBuffer((GLenum)m_bufferType, m_id);
			glBufferData((GLenum)m_bufferType, nItems * sizeof(buffDataType), list, (GLenum)m_usage);
			glBindBuffer((GLenum)m_bufferType, 0);

			m_itemCount = nItems;
		}

		void create(void* list, size_t nItems, BufferUsage usage) {
			setUsage(usage);
			create(list, nItems);
		}

		void update(void* list, size_t nItems, size_t offsetItems = 0) {
			bind();

			//Check if the buffer needs to be re-sized
			if (nItems > m_itemCount) {
				glBufferData((GLenum)m_bufferType, nItems * sizeof(buffDataType), list, (GLenum)m_usage);

				m_itemCount = nItems;
			}

			glBufferSubData((GLenum)m_bufferType, offsetItems * sizeof(buffDataType), nItems * sizeof(buffDataType), list);

			glBindBuffer((GLenum)m_bufferType, 0);
		}

		void bind() {
			glBindBuffer((GLenum)m_bufferType, m_id);
		}

		void destroy() {
			if (m_id) {
				glDeleteBuffers(1, &m_id);
				m_id = 0;
			}
		}

		size_t getItemCount() const { return m_itemCount; };
		unsigned int getID() const { return m_id; };
		BufferUsage getUsage() const { return m_usage; };

		void setUsage(BufferUsage usage) {
			m_usage = usage;
		}
	private:
		size_t m_itemCount = 0;
		unsigned int m_id = 0;
		BufferUsage m_usage;
		BufferType m_bufferType;
	};
}

#endif
