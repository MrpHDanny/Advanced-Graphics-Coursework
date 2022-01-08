#include "Character.h"
#include <random>

Character::Character(OGLRenderer& rend)
{
	this->r = &rend;
	shader = new Shader("SkinningVertex.glsl", "TexturedFragment.glsl");

	/*
		I just couldn't make the model to animate propely so I stuck with adding the Role_T model instead
	*/
	// mesh = Mesh::LoadFromMeshFile("ybot2.msh");
	//anim = new MeshAnimation("idlehappy.anm");
	///material = new MeshMaterial("ybot.mat");

	mesh = Mesh::LoadFromMeshFile("Role_T.msh");
	anim = new MeshAnimation("Role_T.anm");
	material = new MeshMaterial("Role_T.mat");

	for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
	{
		const  MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);

		const  string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string  path = TEXTUREDIR + *filename;

		GLuint  texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}

	charCount = 10;
	terrainMap = new HeightMap(TEXTUREDIR"coursework/islandmap.PNG");


	int totalVertices = terrainMap->GetImageWidth() * terrainMap->GetImageWidth();
	// Pick random vertices
	std::random_device rd2;
	std::mt19937 gen(rd2());
	std::uniform_int_distribution<> pickVertice(1, totalVertices);
	std::uniform_int_distribution<> pickRotation(1, 360);


	for (int i = 0; i < charCount; i++)
	{
		charPositions[i] = pickVertice(gen);
		charRotations[i] = pickRotation(gen);

		// Don't spawn them below water level or too high on top of mountains
		while (terrainMap->vertices[charPositions[i]].y < 800 || terrainMap->vertices[charPositions[i]].y > 1700)
		{
			charPositions[i] = pickVertice(gen);
		}
	}

	currentFrame = 0;
	frameTime = 0.0f;
}

Character::~Character()
{
	delete mesh;
	delete shader;
	delete material;
	delete anim;
	delete terrainMap;
}

void Character::Draw(float dt, GLuint shadowTex)
{
	Matrix4 temp = r->modelMatrix;

	frameTime -= dt;
	while (frameTime < 0.0f)
	{
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}


	r->BindShader(shader);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	
	for (int i = 0; i < charCount; i++)
	{
		r->modelMatrix =
			Matrix4::Translation(Vector3(
				terrainMap->vertices[charPositions[i]].x,
				terrainMap->vertices[charPositions[i]].y ,
				terrainMap->vertices[charPositions[i]].z))
			* Matrix4::Scale(Vector3(50, 50, 50))
			* Matrix4::Rotation(charRotations[i], Vector3(0.0f, 1.0f, 0.0f));

		r->UpdateShaderMatrices();

		vector <Matrix4 > frameMatrices;

		const  Matrix4* invBindPose = mesh->GetInverseBindPose();
		const  Matrix4* frameData = anim->GetJointData(currentFrame);

		for (unsigned int i = 0; i < mesh->GetJointCount(); ++i)
		{
			frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
		}

		int j = glGetUniformLocation(shader->GetProgram(), "joints");

		glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

		for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, matTextures[i]);
			mesh->DrawSubMesh(i);
		}
	}
	
	
	r->modelMatrix = temp;
}