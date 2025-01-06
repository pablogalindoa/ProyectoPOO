#pragma once
#ifndef CASA_H
#define CASA_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Base/camera.h"
#include <iostream>
#include "Base/Scene.h"

bool ganar = false;

class Casa : public Scene {
private:
	SkyDome* sky;
	Terreno* terreno;
	std::vector<Billboard*> billBoard;
	std::vector<Model*> ourModel;
	Model* camara;
	Water* water;
	float angulo;
	int animacion = 0;
	int frameArbol = 1;
public:
	Casa(Camera* cam) {
		glm::vec3 translate;
		glm::vec3 scale;
		Model* model = new Model("Modelos/Mago1/Mago1.obj", cam);
		translate = glm::vec3(0.0f, 0.0f, 3.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		InitGraph(model);
	}
	Casa(Model* camIni) {
		InitGraph(camIni);
	}
	void InitGraph(Model* main) {
		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;
		camara = main;
		//creamos el objeto skydome
		sky = new SkyDome(32, 32, 20, (WCHAR*)L"skydome/sky3.jpg", main->cameraDetails);
		//creamos el terreno
		terreno = new Terreno((WCHAR*)L"skydome/casa.jpg", (WCHAR*)L"skydome/floor1.jpg", 100, 100, main->cameraDetails);
		water = new Water((WCHAR*)L"textures/terreno1.bmp", (WCHAR*)L"textures/water.bmp", 1, 1, camara->cameraDetails);
		glm::vec3 translate;
		glm::vec3 scale;
		glm::vec3 rotation;
		translate = glm::vec3(-10.0f, 06.5f, 155.0f);
		water->setTranslate(&translate);
		water->setRotZ(180);
		// load models
		// -----------
		ourModel.push_back(main);

		Model* model;
		float ye = terreno->Superficie(0, 0);

		//model = new Model("models/Modelos/Bird/Bird.obj", main->cameraDetails);
		//translate = glm::vec3(-11.0f, ye, -7.0f);
		//scale = glm::vec3(5.5f, 5.5f, 5.5f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		model = new Model("models/Modelos/Calderon/Calderon1.obj", main->cameraDetails);
		translate = glm::vec3(-16.0f, ye, -15.0f);
		scale = glm::vec3(1.5f, 1.5f, 1.5f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		model->setRotY(90);
		ourModel.push_back(model);

		model = new Model("models/Modelos/Bed2/Bed.obj", main->cameraDetails);
		translate = glm::vec3(-16.0f, ye, -7.0f);
		scale = glm::vec3(2.5f, 2.5f, 2.5f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		model->setRotY(90);
		ourModel.push_back(model);

		//model = new Model("models/Modelos/Shaman/Shaman.obj", main->cameraDetails);
		//translate = glm::vec3(-16.0f, 7, -7.0f);
		//scale = glm::vec3(1.2f, 1.2f, 1.2f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(180);
		//model->setRotX(-90);
		//model->setRotZ(-45);
		//ourModel.push_back(model);

		model = new Model("models/Modelos/Table2/Table.obj", main->cameraDetails);
		translate = glm::vec3(-16.0f, ye, 7.0f);
		scale = glm::vec3(2.5f, 2.5f, 2.5f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		model->setRotY(90);
		ourModel.push_back(model);

		inicializaBillboards();
	}

	void inicializaBillboards() {
		float ye = terreno->Superficie(0, 0);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol.png", 6, 6, 0, ye - 1, 0, camara->cameraDetails));

		ye = terreno->Superficie(5, -5);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol2.png", 6, 6, 5, ye - 1, -5, camara->cameraDetails));

		ye = terreno->Superficie(-9, -15);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye - 1, -15, camara->cameraDetails));
	}

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	Casa* Render() {
		//borramos el biffer de color y el z para el control de profundidad a la 
		//hora del render a nivel pixel.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		//		glClearColor(255.0f, 255.0f, 255.0f, 255.0f);

				// Actualizamos la camara
		camara->cameraDetails->CamaraUpdate(camara->getRotY(), camara->getTranslate());

		if (this->animacion > 25) { // Timer se ejecuta cada 1000/30 = 33.333 ms
			float ye = terreno->Superficie(5, -5);
			Billboard* temporal = billBoard[1];
			wstring textura = L"billboards/Arbol" +
				(this->frameArbol == 1 ? L"" : to_wstring(this->frameArbol)) + L".png";
			billBoard[1] = new Billboard((WCHAR*)textura.c_str(), 6, 6, 5, ye - 1, -5, camara->cameraDetails);
			if (this->frameArbol == 3) {
				this->frameArbol = 1;
			}
			else {
				this->frameArbol++;
			}
			this->animacion = 0;
			delete temporal;
		}
		else {
			animacion++;
		}
		// Decimos que dibuje la media esfera
		sky->Draw();
		// Ahora el terreno
		terreno->Draw();
		water->Draw();
		// Dibujamos cada billboard que este cargado en el arreglo de billboards.
		/*for (int i = 0; i < billBoard.size(); i++)
			billBoard[i]->Draw();*/

		// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
		for (int i = 0; i < ourModel.size(); i++) {
			ourModel[i]->Draw();
		}
		if (camara->getTranslate()->x < -12.5 && camara->getTranslate()->x > -21 &&
			camara->getTranslate()->y > 6.8 &&
			camara->getTranslate()->y < 7 &&
			camara->getTranslate()->z > -8.0 && camara->getTranslate()->z < -3.5) {

			ganar = true;
		}
		

		// Le decimos a winapi que haga el update en la ventana
		return this;
	}

	std::vector<Model*>* getLoadedModels() {
		return &ourModel;
	}
	std::vector<Billboard*>* getLoadedBillboards() {
		return &billBoard;
	}
	Model* getMainModel() {
		return this->camara;
	}
	float getAngulo() {
		return this->angulo;
	}
	void setAngulo(float angulo) {
		this->angulo = angulo;
	}
	SkyDome* getSky() {
		return sky;
	}
	Terreno* getTerreno() {
		return terreno;
	}

	~Casa() {
		if (this->sky != NULL) {
			delete this->sky;
			this->sky = NULL;
		}
		if (this->terreno != NULL) {
			delete this->terreno;
			this->terreno = NULL;
		}
		if (billBoard.size() > 0)
			for (int i = 0; i < billBoard.size(); i++)
				delete billBoard[i];
		this->billBoard.clear();
		if (ourModel.size() > 0)
			for (int i = 0; i < ourModel.size(); i++)
				if (ourModel[i] != camara)
					delete ourModel[i];
		this->ourModel.clear();
	}
};

#endif