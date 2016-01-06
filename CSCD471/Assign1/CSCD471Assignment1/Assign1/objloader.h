#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#define GLM_FORCE_CXX11
#include <glm/glm.hpp>


	class OBJLoader {
	public:
		//! Constructor
		OBJLoader();

		//! Destructor
		~OBJLoader();

		
		bool load(const char *filename);

		std::vector<glm::vec3> const &getVertices() const;
		std::vector<glm::vec3> const &getNormals() const;
		std::vector<int> const &getVertexIndices() const;
		std::vector<int> const &getNormalIndices() const;

		 

		void computeNormals(std::vector<glm::vec3> const &vertices,
			std::vector<int> const &indices,
			std::vector<glm::vec3> &normals);
	private:
		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<int> vIndices;
		std::vector<int> nIndices;
	};

#endif

