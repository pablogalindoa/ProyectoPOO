#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread> // For sleep_for
#include <chrono> // For time tracking
#include <cstdlib> // For rand()
#include <ctime>   // For seeding srand()
#include <random> 

#include <vector>
#include "Base/camera.h"
#include <iostream>
#include "Base/Scene.h"
#include "Casa.h"
#include "Coleccionables.h"
#include "MyScene.h"
#include "Rain.h"
#include "ClimateSystem.h"

//RainSystem rainSystem(1000);
float deltaTime = 0.016f;  // Assume 60 FPS, so 1/60 seconds per frame
class Scenario : public Scene {
private:
	SkyDome* sky = nullptr;
	Terreno* terreno;
	std::vector<Billboard*> billBoard;
	std::vector<Model*> ourModel;
	std::vector<Model*> ourModelAparece;
	Model* camara;
	Model* modelAparece;
	Model* mineral1;
	Model* mineral2;
	Model* mineral3;
	Model* enemigo1;
	Model* enemigo2;
	Model* enemigo3;
	Model* perro;
	Model* poder;
	Scene* hechizo;
	Water* water;
	Scene* spell;
	float angulo;
	int animacion = 0;
	int frameArbol = 1;
	bool checkCondi = false;
	bool muerte = false;
	int condicion1 = 0;
	int condicion2 = 0;
	int condicion3 = 0;
public:
	int contadorColeccion = 0;
	int contadorGema = 0;
	int rotacionGema = 0;
	int movimientoEnemigo1 = 0;
	int movimientoEnemigo2 = 0;
	int movimientoEnemigo3 = 0;
	int rotacionEnemigo3 = 0;
	int movimientoPerroY = 0;
	int movimientoPerroZ = 0;
	int movimientoHechizo = 0;
	bool cambiarCielo = false;
	string cielo = "skydome/sky2.jpg";
	string cielo2 = "skydome/sky1.jpg";
	const wchar_t* wchar_cielo;
	Scenario(Camera *cam) {
		glm::vec3 translate;
		glm::vec3 scale;
		Model* model = new Model("models/Cube.obj", cam);
		translate = glm::vec3(0.0f, 0.0f, 3.0f);
		scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		InitGraph(model);
	}
	Scenario(Model *camIni) {
		InitGraph(camIni);
	}

	void InitGraph(Model *main) {
		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;
		camara = main;
		wstring cielo1(cielo.begin(), cielo.end());
		const wchar_t* wchar_cielo = cielo1.c_str();
		//creamos el objeto skydome
		if (!cambiarCielo) {
			sky = new SkyDome(32, 32, 20, (WCHAR*)wchar_cielo, main->cameraDetails);
		}
		else {
			sky = new SkyDome(32, 32, 20, (WCHAR*)L"skydome/sky1.jpg", main->cameraDetails);
		}

		
		//creamos el terreno
		terreno = new Terreno((WCHAR*)L"skydome/terreno1.1.jpg", (WCHAR*)L"skydome/terreno5.jpg", 400, 400, main->cameraDetails);
		water = new Water((WCHAR*)L"textures/terreno1.bmp", (WCHAR*)L"textures/purpleWater.bmp", 200, 90, camara->cameraDetails);
		//rainSystem = RainSystem(1000); // Create rain system with 1000 raindrops
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
		//model = new Model("models/fogata.obj", main->cameraDetails);
		//translate = glm::vec3(0.0f, 10.0f, 25.0f);
		//model->setTranslate(&translate);
		//rotation = glm::vec3(0.0f, 0.0f, 0.0f); //rotation X
		//model->setRotX(45); // 45� rotation
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Candlestick/option_candlestick_obj/Candlestick.obj", main->cameraDetails);
		//translate = glm::vec3(0.0f, 10.0f, 40.0f);
		//scale = glm::vec3(0.001f, 0.001f, 0.001f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//ourModel.push_back(model);
	
		random_device rd;
		mt19937 gen(rd());

		// Define the range [0, 3]
		uniform_int_distribution<int> distrib(0, 2);

		int randomNumber = distrib(gen);

		switch (randomNumber) {
			case 0: {
				// Calabaza
				model = new Model("models/Modelos/Pumpkin/Pumpkin/RAN_Halloween_Pumpkin_2024_Low_Poly.obj", main->cameraDetails);
				translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), 15.0f);
				scale = glm::vec3(10.5f, 10.5f, 10.5f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(90);
				ourModel.push_back(model);

				// Shaman
				model = new Model("models/Modelos/Shaman/Shaman.obj", main->cameraDetails);
				translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), -35.0f);
				scale = glm::vec3(1.2f, 1.2f, 1.2f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(180);
				ourModel.push_back(model);

				// Arbol
				model = new Model("models/Modelos/Arboles1/Arbol1.obj", main->cameraDetails);
				translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), 30.0f);
				scale = glm::vec3(1.1f, 1.1f, 1.1f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				ourModel.push_back(model);
			}
			break;
			case 1: {
				// Calabaza
				model = new Model("models/Modelos/Pumpkin/Pumpkin/RAN_Halloween_Pumpkin_2024_Low_Poly.obj", main->cameraDetails);
				translate = glm::vec3(-70.0f, terreno->Superficie(0.0f, 60.0f), 25.0f);
				scale = glm::vec3(10.5f, 10.5f, 10.5f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(90);
				ourModel.push_back(model);

				// Shaman
				model = new Model("models/Modelos/Shaman/Shaman.obj", main->cameraDetails);
				translate = glm::vec3(-40.0f, terreno->Superficie(0.0f, 60.0f), -55.0f);
				scale = glm::vec3(1.2f, 1.2f, 1.2f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(180);
				ourModel.push_back(model);

				// Arbol
				model = new Model("models/Modelos/Arboles1/Arbol1.obj", main->cameraDetails);
				translate = glm::vec3(-30.0f, terreno->Superficie(0.0f, 60.0f), 40.0f);
				scale = glm::vec3(1.1f, 1.1f, 1.1f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				ourModel.push_back(model);
			}
			break;
			case 2: {
				// Calabaza
				model = new Model("models/Modelos/Pumpkin/Pumpkin/RAN_Halloween_Pumpkin_2024_Low_Poly.obj", main->cameraDetails);
				translate = glm::vec3(-100.0f, terreno->Superficie(0.0f, 60.0f), 15.0f);
				scale = glm::vec3(10.5f, 10.5f, 10.5f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(90);
				ourModel.push_back(model);

				// Shaman
				model = new Model("models/Modelos/Shaman/Shaman.obj", main->cameraDetails);
				translate = glm::vec3(-80.0f, terreno->Superficie(0.0f, 60.0f), -75.0f);
				scale = glm::vec3(1.2f, 1.2f, 1.2f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				model->setRotY(180);
				ourModel.push_back(model);

				// Arbol
				model = new Model("models/Modelos/Arboles1/Arbol1.obj", main->cameraDetails);
				translate = glm::vec3(-90.0f, terreno->Superficie(0.0f, 60.0f), 10.0f);
				scale = glm::vec3(1.1f, 1.1f, 1.1f);	// it's a bit too big for our scene, so scale it down
				model->setTranslate(&translate);
				model->setScale(&scale);
				ourModel.push_back(model);
			}
			break;

		}

		//model = new Model("models/Modelos/Fence/Fence_Salario/obj/objFence.obj", main->cameraDetails);
		//translate = glm::vec3(94.0f, 0, -95.0f);
		//scale = glm::vec3(50.1f, 50.1f, 50.1f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Fence/Fence_Salario/obj/objFence.obj", main->cameraDetails);
		//translate = glm::vec3(123.0f, 0, -95.0f);
		//scale = glm::vec3(50.1f, 50.1f, 50.1f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Tower/Spanish_tower/obj/objTower.obj", main->cameraDetails);
		//translate = glm::vec3(109.985153f, 3.9f, -194.5);
		//scale = glm::vec3(18.5f, 18.5f, 18.5f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//ourModel.push_back(model);

		poder = new Model("models/Modelos/Branch/branchbroken.obj", main->cameraDetails);
		translate = glm::vec3(-50.0f, -10.0f, -85.0f);
		scale = glm::vec3(0.1f, 0.1f, 0.1f);	// it's a bit too big for our scene, so scale it down
		poder->setTranslate(&translate);
		poder->setScale(&scale);
		poder->setRotY(90);
		//ourModel.push_back(model);


		// ENEMIGOS

		enemigo1 = new Model("models/Modelos/Troll/Troll1.obj", main->cameraDetails);
		translate = glm::vec3(-50.0f, 3.0f, -60.0f);
		scale = glm::vec3(5.5f, 5.5f, 5.5f);	// it's a bit too big for our scene, so scale it down
		enemigo1->setTranslate(&translate);
		enemigo1->setScale(&scale);
		enemigo1->setRotY(180);
		//ourModel.push_back(model);

		enemigo2 = new Model("models/Modelos/Dragon/Ruffrunner_model.obj", main->cameraDetails);
		translate = glm::vec3(-160.0f, 60.0f, 8.5f);
		scale = glm::vec3(5.5f, 5.5f, 5.5f);	// it's a bit too big for our scene, so scale it down
		enemigo2->setTranslate(&translate);
		enemigo2->setScale(&scale);
		enemigo2->setRotY(90);
		//ourModel.push_back(model);

		enemigo3 = new Model("models/Modelos/Bird/Bird.obj", main->cameraDetails);
		translate = glm::vec3(-59.0f, terreno->Superficie(0.0f, 60.0f), -100.0f);
		scale = glm::vec3(5.5f, 5.5f, 5.5f);	// it's a bit too big for our scene, so scale it down
		enemigo3->setTranslate(&translate);
		enemigo3->setScale(&scale);
		//enemigo3->setRotY(90);
		//ourModel.push_back(model);

		model = new Model("models/dae/dae/Only_Spider_with_Animations_Export.dae", main->cameraDetails);
		translate = glm::vec3(100.0f, 3.9f, 4.5f);
		scale = glm::vec3(2.0f, 2.0f, 2.0f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		ourModel.push_back(model);
		try {
			Animation* ani = new Animation("models/dae/dae/Only_Spider_with_Animations_Export.dae", model->GetBoneInfoMap(), model->GetBoneCount());
			model->setAnimator(new Animator(ani));
		}
		catch (...) {
			cout << "Could not load animation!\n";
		}

		//model = new Model("models/Modelos/Calderon/Calderon1.obj", main->cameraDetails);
		//translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), -40.0f);
		//scale = glm::vec3(1.5f, 1.5f, 1.5f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Bed2/Bed.obj", main->cameraDetails);
		//translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), -30.0f);
		//scale = glm::vec3(2.5f, 2.5f, 2.5f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Table2/Table.obj", main->cameraDetails);
		//translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), -20.0f);
		//scale = glm::vec3(2.5f, 2.5f, 2.5f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		// Modelos para Coleccionar

		mineral1 = new Model("models/Modelos/Asteroid/Asteroid_1d.obj", main->cameraDetails);
		translate = glm::vec3(-70.0f, terreno->Superficie(0.0f, 60.0f), -10.0f);
		scale = glm::vec3(3.5f, 3.5f, 3.5f);	// it's a bit too big for our scene, so scale it down
		mineral1->setTranslate(&translate);
		mineral1->setScale(&scale);
		mineral1->setRotY(90);
		//ourModel.push_back(model);

		mineral2 = new Model("models/Modelos/Asteroid/Asteroid_1d.obj", main->cameraDetails);
		translate = glm::vec3(-59.0f, terreno->Superficie(0.0f, 60.0f), -118.0f);
		scale = glm::vec3(3.5f, 3.5f, 3.5f);	// it's a bit too big for our scene, so scale it down
		mineral2->setTranslate(&translate);
		mineral2->setScale(&scale);
		mineral2->setRotY(90);
		//ourModel.push_back(model);

		mineral3 = new Model("models/Modelos/Asteroid/Asteroid_1d.obj", main->cameraDetails);
		translate = glm::vec3(-113.0f, terreno->Superficie(0.0f, 60.0f), 72.0f);
		scale = glm::vec3(3.5f, 3.5f, 3.5f);	// it's a bit too big for our scene, so scale it down
		mineral3->setTranslate(&translate);
		mineral3->setScale(&scale);
		mineral3->setRotY(90);
		//ourModel.push_back(model);

		// Gema completa

		/*int contadorGema = 0;
		int rotacionGema = 0;*/

		modelAparece = new Model("models/Modelos/Gema/Gema1.obj", main->cameraDetails);
		translate = glm::vec3(-25.0f, -15.0, 6.46f);
		scale = glm::vec3(6.0f, 6.0f, 6.0f);	// it's a bit too big for our scene, so scale it down
		modelAparece->setTranslate(&translate);
		modelAparece->setScale(&scale);
		ourModelAparece.push_back(modelAparece);



		
		


		model = new Model("models/Modelos/FantasyPillar/FantasyPillar/obj/objPillars.obj", main->cameraDetails);
		translate = glm::vec3(-50.0f, terreno->Superficie(0.0f, 60.0f), 25.0f);
		scale = glm::vec3(15.5f, 15.5f, 15.5f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		model->setRotY(90);
		ourModel.push_back(model);

		perro = new Model("models/Modelos/Wolf2/WolfRiggedAndGameReady/Wolf_One_obj.obj", main->cameraDetails);
		translate = glm::vec3(-23.83f, 9.42, -8.0f);
		scale = glm::vec3(5.5f, 5.5f, 5.5f);	// it's a bit too big for our scene, so scale it down
		perro->setTranslate(&translate);
		perro->setScale(&scale);
		perro->setRotY(90);
		//ourModel.push_back(model);

		//model = new Model("models/Modelos/Survivalist/Textures/Monito1.dae", main->cameraDetails);
		//translate = glm::vec3(-20.0f, terreno->Superficie(0.0f, 60.0f), 15.0f);
		//scale = glm::vec3(0.03f, 0.03f, 0.03f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotX(-90);
		//ourModel.push_back(model);
		//try {
		//	Animation* ani = new Animation("models/Modelos/Survivalist/Textures/Monito1.dae", model->GetBoneInfoMap(), model->GetBoneCount());
		//	model->setAnimator(new Animator(ani));
		//}
		//catch (...) {
		//	cout << "Could not load animation!\n";
		//}

		//model = new Model("models/dae/tex/Blacksmith_AmM_SK.dae", main->cameraDetails);
		//translate = glm::vec3(-20.0f, terreno->Superficie(0.0f, 60.0f), 25.0f);
		//scale = glm::vec3(0.03f, 0.03f, 0.03f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		///*model->setRotX(-90);*/
		//ourModel.push_back(model);
		//try {
		//	Animation* ani = new Animation("models/dae/tex/Blacksmith_AmM_SK.dae", model->GetBoneInfoMap(), model->GetBoneCount());
		//	model->setAnimator(new Animator(ani));
		//}
		//catch (...) {
		//	cout << "Could not load animation!\n";
		//}



		//model = new Model("models/dancing_vampire.dae", main->cameraDetails);
		//translate = glm::vec3(0.0f, terreno->Superficie(0.0f, 60.0f) , 60.0f);
		//scale = glm::vec3(1.1f, 1.1f, 1.1f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(90);
		//ourModel.push_back(model);

		/*try{
			Animation *ani = new Animation("models/dancing_vampire.dae", model->GetBoneInfoMap(), model->GetBoneCount());
		    model->setAnimator(new Animator(ani));
		}catch(...){
			cout << "Could not load animation!\n";
		}*/
		//model = new Model("models/Silly_Dancing.dae", main->cameraDetails);
		//translate = glm::vec3(10.0f, terreno->Superficie(0.0f, 60.0f) , 60.0f);
		//scale = glm::vec3(0.1f, 0.1f, 0.1f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//model->setRotY(180);
		//ourModel.push_back(model);
		//try{
		//	Animation *ani = new Animation("models/Silly_Dancing.dae", model->GetBoneInfoMap(), model->GetBoneCount());
		//    model->setAnimator(new Animator(ani));
		//}catch(...){
		//	cout << "Could not load animation!\n";
		//}
//		model = new Model("models/IronMan.obj", main);
//		translate = glm::vec3(0.0f, 20.0f, 30.0f);
//		scale = glm::vec3(0.025f, 0.025f, 0.025f);	// it's a bit too big for our scene, so scale it down
//		model->setScale(&scale);
//		model->setTranslate(&translate);
//		ourModel.push_back(model);

		

		//model = new Model("models/backpack.obj", main->cameraDetails, false, false);
		//translate = glm::vec3(20.0f, 14.0f, 0.0f);
		//scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
		//model->setTranslate(&translate);
		//model->setScale(&scale);
		//ourModel.push_back(model);
		

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
	Scene* Render() {
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
		//Shader* shader = new Shader("path_to_vertex_shader.vert", "path_to_fragment_shader.frag");
		//ClimateManager climateManager;

		//// Simulate a random climate change (or you can set specific triggers based on time, location, etc.)
		//int randomClimate = rand() % 4;  // Random climate between 0 and 3
		//climateManager.updateClimate(randomClimate);

		//// Apply the lighting effects to the shader
		//climateManager.applyLighting(*shader);  // Assuming 'shader' is your active shader
		// Decimos que dibuje la media esfera
		sky->Draw();
		// Ahora el terreno
		terreno->Draw();

		water->Draw();

		//rainSystem.updateRain(0.016f); // Assuming 60 FPS, ~16ms per frame
		//rainSystem.renderRain();
		// Dibujamos cada billboard que este cargado en el arreglo de billboards.
		/*for (int i = 0; i < billBoard.size(); i++)
			billBoard[i]->Draw();*/

		// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
		for (int i = 0; i < ourModel.size(); i++) {
			ourModel[i]->Draw();
		}
		for (int i = 0; i < ourModel.size(); i++) {
			modelAparece->Draw();
			mineral1->Draw();
			mineral2->Draw();
			mineral3->Draw();
			enemigo1->Draw();
			enemigo2->Draw();
			enemigo3->Draw();
			perro->Draw();
			poder->Draw();
		}

		// Le decimos a winapi que haga el update en la ventana
		if (camara->getTranslate()->x > 100 &&
			camara->getTranslate()->y > 3.89 &&
			camara->getTranslate()->y < 3.91 &&
			camara->getTranslate()->z < -170) {
			glm::vec3 pos = glm::vec3(5.0f, getTerreno()->Superficie(5.0f, -5.0f), -5.0f);
			camara->setTranslate(&pos);
			return new Casa(camara);
		}
		// Todo de la jugabilidad

		// Enemigos atacan

		
		// Coleccion de las piezas del Diamante
		if (camara->getTranslate()->x < -65 && camara->getTranslate()->x > -80 &&
			camara->getTranslate()->y > 9 &&
			camara->getTranslate()->y < 9.2 &&
			camara->getTranslate()->z < -10 && camara->getTranslate()->z > -20) {
			glm::vec3 translate;
			translate = glm::vec3(-70.0f, -20.0f, -10.0f);
			mineral1->setTranslate(&translate);
			condicion1 = 1;
			contadorColeccion = condicion1 + condicion2 + condicion3;

		}
		if (camara->getTranslate()->x < -110 && camara->getTranslate()->x > -118 &&
			camara->getTranslate()->y > 9 &&
			camara->getTranslate()->y < 9.2 &&
			camara->getTranslate()->z > 68 && camara->getTranslate()->x < 75) {
			glm::vec3 translate;
			translate = glm::vec3(-113.0f, -20.0f, 72.0f);
			mineral3->setTranslate(&translate);
			condicion2 = 1;
			contadorColeccion = condicion1 + condicion2 + condicion3;
		}
		if (camara->getTranslate()->x < -50 && camara->getTranslate()->x > -65 &&
			camara->getTranslate()->y > 9 &&
			camara->getTranslate()->y < 9.2 &&
			camara->getTranslate()->z < -115 && camara->getTranslate()->z > -128) {
			glm::vec3 translate;
			translate = glm::vec3(-59.0f, -20.0f, -118.0f);
			mineral2->setTranslate(&translate);
			condicion3 = 1;
			contadorColeccion = condicion1 + condicion2 + condicion3;
		}

		if (contadorColeccion == 3) {
			checkCondi = true;
			

			if (camara->getTranslate()->x > -30 && camara->getTranslate()->x < -20 &&
				camara->getTranslate()->y > 9 &&
				camara->getTranslate()->y < 9.2 &&
				camara->getTranslate()->z > -9.7 && camara->getTranslate()->z < 20.29) {
				wstring cieloW(cielo2.begin(), cielo2.end());
				wchar_cielo = cieloW.c_str();

				cambiarCielo = true;

				//sky->Draw();

				glm::vec3 mataEnemigo1;
				mataEnemigo1 = glm::vec3(113.16f, -26.0f, 10.0f);
				enemigo1->setTranslate(&mataEnemigo1);

				glm::vec3 mataEnemigo2;
				mataEnemigo2 = glm::vec3(133.16f, -26.0f, 10.0f);
				enemigo2->setTranslate(&mataEnemigo2);

				glm::vec3 mataEnemigo3;
				mataEnemigo3 = glm::vec3(153.16f, -26.0f, 10.0f);
				enemigo3->setTranslate(&mataEnemigo3);

			}
			else {

				glm::vec3 translate;
				translate = glm::vec3(-25.0f, 15.0, 6.46f);
				modelAparece->setTranslate(&translate);

				rotacionGema += 10; // Increment rotation
				if (rotacionGema >= 360) {
					rotacionGema = 0; // Reset after full rotation
				}
				modelAparece->setRotY(rotacionGema); // Apply rotation

				movimientoPerroY += 1;
				if (movimientoPerroY >= 38.92) {
					movimientoPerroY = 9.42;
				}
				glm::vec3 translatePerro;
				translatePerro = glm::vec3(-23.83f, movimientoPerroY, -8.0f);
				perro->setTranslate(&translatePerro);

				movimientoPerroZ += 20;
				if (movimientoPerroZ >= 360) {
					movimientoPerroZ = 0;
				}
				perro->setRotZ(movimientoPerroZ);
			}


			/*Scene* spell1 = new MyScene();
			Model* hechizo = spell1->getMainModel();
			float posicionJugador1 = hechizo->getTranslate()->x;
			float posicionJugador2 = hechizo->getTranslate()->y;
			float posicionJugador3 = hechizo->getTranslate()->z;
			
			movimientoHechizo += 1.0f;
			if (movimientoHechizo >= 15) {
				movimientoHechizo = 0;
			}
			glm::vec3 translateHechizo;
			translateHechizo = glm::vec3(posicionJugador1 + movimientoHechizo, posicionJugador2 + 5, posicionJugador3);
			poder->setTranslate(&translateHechizo);*/
		}
		else {
			movimientoEnemigo1 -= 1.0f;
			if (movimientoEnemigo1 <= -130) {
				movimientoEnemigo1 = 0;
			}
			glm::vec3 translateEnemigo1;
			translateEnemigo1 = glm::vec3(113.16f, 6.0f, movimientoEnemigo1);
			enemigo1->setTranslate(&translateEnemigo1);

			movimientoEnemigo2 += 1.0f;
			if (movimientoEnemigo2 >= 210) {
				movimientoEnemigo2 = -160;
			}
			glm::vec3 translateEnemigo2;
			translateEnemigo2 = glm::vec3(movimientoEnemigo2, 50.0f, 8.5f);
			enemigo2->setTranslate(&translateEnemigo2);

			rotacionEnemigo3 += 5;
			if (rotacionEnemigo3 >= 360) {
				rotacionEnemigo3 = 0; // Reset after full rotation
			}
			enemigo3->setRotY(rotacionEnemigo3); // Apply rotation

			movimientoEnemigo3 += 1;
			if (movimientoEnemigo3 >= 15) {
				movimientoEnemigo3 = 9.1;
			}
			glm::vec3 translateEnemigo3;
			translateEnemigo3 = glm::vec3(-59.0f, movimientoEnemigo3, -100.0f);
			enemigo3->setTranslate(&translateEnemigo3);
		}
		
		
		/*if (checkCondi) {
			
			this_thread::sleep_for(chrono::seconds(2));
			if (contadorGema >= 8) {
				contadorGema = 0;
			}			
		}*/
		
		
		return this;
	}

	
	std::vector<Model*> *getLoadedModels() {
		return &ourModel;
	}
	std::vector<Billboard*> *getLoadedBillboards() {
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

	~Scenario() {
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