#ifndef _billb
#define _billb
#include "Base/Utilities.h"
#include "Base/model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Base/glext.h"
#include "Base/wglext.h"

class Billboard : public Model{

private:
	float alto, ancho;

	void reloadData(vector<Vertex> *vertices){
		float verts[] = {
			// positions        // texture coords
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, //bottom left
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  //bottom right
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f,   //top right
			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  //top left
		};
		vertices->clear();
		for (int i = 0; i < 20; i=i+5) {
			Vertex v;
			v.Position = glm::vec3(*(verts + i), *(verts + i + 1), *(verts + i + 2));
			v.TexCoords = glm::vec2(*(verts + i + 3), *(verts + i + 4));
			vertices->push_back(v);
		}
	}

	void reloadData(vector<Vertex> *vertices, glm::vec3 origin){
		glm::vec3 billcam = glm::vec3(cameraDetails->getPosition().x - origin.x,
			cameraDetails->getPosition().y - origin.y,
			cameraDetails->getPosition().z - origin.z);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(up, billcam));
		right.x *= this->ancho / 2.0;
		right.z *= this->ancho / 2.0;

		float verts[] = {
			// positions        // texture coords
			origin.x + right.x, origin.y       , origin.z + right.z, 1, 1, //bottom left
			origin.x - right.x, origin.y       , origin.z - right.z, 0, 1, //bottom right

			origin.x - right.x, origin.y + alto, origin.z - right.z, 0, 0, //top right
			origin.x + right.x, origin.y + alto, origin.z + right.z, 1, 0, //top left
		};
		vertices->clear();
		for (int i = 0; i < 20; i = i + 5) {
			Vertex v;
			v.Position = glm::vec3(*(verts + i), *(verts + i + 1), *(verts + i + 2));
			v.TexCoords = glm::vec2(*(verts + i + 3), *(verts + i + 4));
			vertices->push_back(v);
		}
	}
public:
	Billboard(WCHAR textura[], float ancho, float alto, float x, float y, float z, Camera* camera) {
		cameraDetails = camera;
		vector<unsigned int> indices;
		vector<Texture> textures;
		vector<Vertex>	vertices;

		this->alto = alto;
		this->ancho= ancho;
//		this->ancho = ancho * 2;
//		this->alto = alto * 2;
		unsigned int texturaB;
		glm::vec3 origin = glm::vec3(x, y, z);
		setTranslate(&origin);
		wstring tex((const wchar_t*)textura);
		string text(tex.begin(), tex.end());
		bool alpha = true;
		texturaB = TextureFromFile(text.c_str(), this->directory, false, true, &alpha);
		Texture t = { texturaB , "texture_diffuse", text.c_str() };
		textures_loaded.push_back(t);
		textures.push_back(t);

		reloadData(&vertices);
		indices = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		gpuDemo = NULL;
		meshes.push_back(Mesh(vertices, indices, textures, GL_DYNAMIC_DRAW, GL_STATIC_DRAW));
		buildKDtree();
	}

	~Billboard(){
		//nos aseguramos de disponer de los recursos previamente reservados
	}

	// Usa el shader default para poder imprimir el billboard
	void Draw() {
		if (gpuDemo == NULL) {
			// build and compile our shader zprogram
			// ------------------------------------
			gpuDemo = new Shader("shaders/billboard.vs", "shaders/billboard.fs");
//			gpuDemo = new Shader("shaders/models/1.model_material_loading.vs", "shaders/models/1.model_material_loading.fs");
			setDefaultShader(true);
		}
		if (getDefaultShader()) {
			gpuDemo->use();
			Model::prepShader(*gpuDemo);
			prepShader(*gpuDemo);
			gpuDemo->setInt("texture_diffuse1", 0);
			Draw(*gpuDemo);
			gpuDemo->desuse();
		} else Draw(*gpuDemo);
	}

	void Draw(Shader &shader) {
//		glDisable(GL_DEPTH_TEST);
		reloadData(&(meshes[0].vertices), *getTranslate());
		Model::Draw(shader);
//		glEnable(GL_DEPTH_TEST);
	}

	void prepShader(Shader& shader) {
		glm::mat4 projection = cameraDetails->getProjection();
		glm::mat4 view = cameraDetails->getView();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
//		model = glm::scale(model, glm::vec3(3.0f,3.0f,3.0f));

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
	}
};

#endif 