#include "Fern.h"
#include <random>

Fern::Fern(OGLRenderer& rend)
{
	this->r = &rend;
	shader = new Shader("coursework/treeVertex.glsl", "coursework/treeFragment.glsl");
	mesh = Mesh::LoadFromMeshFile("ferns.msh");
	material = new MeshMaterial("ferns.mat");
	fernCount = 1000;
	terrainMap = new HeightMap(TEXTUREDIR"coursework/islandmap.PNG");


	for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
	{
		const  MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

		const  string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string  path = TEXTUREDIR + *filename;

		GLuint  texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}

	int totalVertices = terrainMap->GetImageWidth() * terrainMap->GetImageWidth();

	// Pick random vertices
	std::random_device rd2;
	std::mt19937 gen(rd2());
	std::uniform_int_distribution<> pickVertice(1, totalVertices);
	std::uniform_int_distribution<> pickRotation(1, 360);


	for (int i = 0; i < fernCount; i++)
	{
		fernPositions[i] = pickVertice(gen);
		fernRotations[i] = pickRotation(gen);

		// Don't spawn them below water level or too high on top of mountains
		while (terrainMap->vertices[fernPositions[i]].y < 800 || terrainMap->vertices[fernPositions[i]].y > 1700)
		{
			fernPositions[i] = pickVertice(gen);
		}
	}
}

Fern::~Fern()
{
	delete shader;
	delete mesh;
	delete material;
	delete terrainMap;
}

void Fern::Draw(float dt, GLuint shadowTex)
{
	Matrix4 temp = r->modelMatrix;

	for (int i = 0; i < fernCount; i++)
	{
		r->BindShader(shader);
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);

		r->modelMatrix =
			Matrix4::Translation(Vector3(
				terrainMap->vertices[fernPositions[i]].x,
				terrainMap->vertices[fernPositions[i]].y - heightOffset,
				terrainMap->vertices[fernPositions[i]].z))
			* Matrix4::Scale(Vector3(3, 3, 3));

		for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, matTextures[i]);
			mesh->DrawSubMesh(i);
		}
		r->UpdateShaderMatrices();

	}
	r->modelMatrix = temp;
}
