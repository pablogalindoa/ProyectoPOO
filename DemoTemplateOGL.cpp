// DemoTemplateOGL.cpp : Defines the entry point for the application.
//
#include "WinAPIHeaders/framework.h"
#include "WinAPIHeaders/DemoTemplateOGL.h"
#include "Base/Utilities.h"
#include "InputDevices/KeyboardInput.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Base/glext.h"
#include "Base/wglext.h"
#include "Base/model.h"

#ifdef _WIN32 
#include "InputDevices/GamePadRR.h"

#else
#include <GLFW/glfw3.h>
#endif
#include "Base/Scene.h"
#include "Scenario.h"
#include "Rain.h"
#include "Casa.h"

#define MAX_LOADSTRING 100
#define Timer1 100
bool renderiza;                     // Variable para controlar el render
#ifdef _WIN32 
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HDC dc;
HGLRC rc;
GamePadRR* gamPad;                  // Manejador de gamepad
// Funciones para activar OpenGL version > 2.0
int prepareRenderWindow(HINSTANCE hInstance, int nCmdShow);
bool SetUpPixelFormat(HDC hDC, PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB, PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB);
// Callback principal de la ventana en WINAPI
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
#else
GLFWwindow* window;
void window_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
bool checkInput(GameActions* actions, Scene *scene);
bool checkCondicion(GameActions* actions, Scene* scene);
bool checkMuerte(GameActions* actions, Scene* scene);
bool checkGanar(GameActions* actions, Scene* scene);
//RainSystem rainSystem(1000);
void mouseActions();

// Propiedades de la ventana
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
bool newContext = false; // Bandera para identificar si OpenGL 2.0 > esta activa
struct GameTime gameTime;

// Objecto de escena y render
Scene *OGLobj;
Scene* OGLcasa;

#ifdef _WIN32 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEMOTEMPLATEOGL, szWindowClass, MAX_LOADSTRING);
    // Si no logra activar OpenGL 2 o superior termina el programa
    if (prepareRenderWindow(hInstance, nCmdShow))
        return 1;
    LOGGER::LOGS::getLOGGER().setWindow(hWnd);
    // game loop
    gamPad = new GamePadRR(1); // Obtenemos el primer gamepad conectado
    SetTimer(hWnd, Timer1, 1000/30, (TIMERPROC)WndProc);// Asignamos el timer con un render de 30 fps
    MSG msg = { 0 };
#else
int main(int argc, char** argv){
    if (!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "DemoTemplateOGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "No opengl load"<< endl;
        glfwTerminate();
        return -1;
    }
#endif
    // Main character with it's camera
    glm::vec3 translate, scale, v(0, 0, -1);
    translate = glm::vec3(-65.0f, 10.0f, -5.0f);
    //5, ye - 1,-5
    //MainModel *model = new MainModel(hWnd, "models/Cube.obj", translate);
    Camera* camera = new Camera();
    Model* model = new Model("models/Modelos/Mago1/Mago1.obj", translate, camera);
    model->setTranslate(&translate);
    camera->setFront(v);
    camera->setCharacterHeight(4.0);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
    model->setScale(&scale);
    model->setTranslate(&translate);
    OGLcasa = new Casa(model);
    OGLobj = new Scenario(model); // Creamos nuestra escena con esa posicion de inicio
    translate = glm::vec3(5.0f, OGLobj->getTerreno()->Superficie(model->getNextTranslate()->x, model->getNextTranslate()->z), -5.0f);
    model->setTranslate(&translate);
    model->setNextTranslate(&translate);
    renderiza = true;

    int running = 1;
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    gameTime.lastTick = get_nanos() / 1000 /1000;
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifndef _WIN32
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    while(!glfwWindowShouldClose(window)){
        gameTime.deltaTime = get_nanos() / 1000 / 1000 - gameTime.lastTick;
        gameTime.lastTick = get_nanos() / 1000 / 1000;
        glfwPollEvents();
        OGLobj->setAngulo(OGLobj->getAngulo() + 1.5 * gameTime.deltaTime);
#else
    while (running) {
        gameTime.deltaTime = get_nanos() / 1000 / 1000 - gameTime.lastTick;
        gameTime.lastTick = get_nanos() / 1000 / 1000;
#endif
        if (renderiza) {
            GameActions actions;
            // render
            // ------
            bool checkCollition = checkInput(&actions, OGLobj);
            bool checkCondiciones = checkCondicion(&actions, OGLobj);
            bool checkMuertes = checkMuerte(&actions, OGLobj);
            bool checkGanars = checkGanar(&actions, OGLobj);

            if (checkCollition) { // Bandera para buscar colisiones sobre Camara/Modelo
                if (false && OGLobj->lookForCollition(true) != NULL) { // Llamamos a la funcion de colision 
                    model->setNextTranslate(model->getTranslate());
                    model->setNextRotX(model->getRotX());
                    model->setNextRotY(model->getRotY());
                    model->setNextRotZ(model->getRotZ());
                } else {
                    //model->CamaraAvanza();
                    model->setTranslate(model->getNextTranslate());
                    model->setRotX(model->getNextRotX());
                    model->setRotY(model->getNextRotY());
                    model->setRotZ(model->getNextRotZ());
                }
            }
            if(checkMuertes){
                LOGGER::LOGS::getLOGGER("DemoTemplate").info("TE HAN MATADO!");
                break;
            }
            if (ganar) {
                LOGGER::LOGS::getLOGGER("DemoTemplate").info("LOGRASTE LLEGAR A CASA SANO Y SALVO!");
                break;
            }
            /*if (checkCondiciones) {
                LOGGER::LOGS::getLOGGER("DemoTemplate").info("YA TIENES TODAS LAS PIEZAS!");
            }*/
            Scene *escena = OGLobj->Render();
            if (escena != OGLobj) {
                delete OGLobj;
                OGLobj = escena;
            }
//            renderiza = false;
#ifdef _WIN32
            SwapBuffers(dc);
#else
            glfwSwapBuffers(window);
#endif
        }
#ifdef _WIN32
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT){
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
#endif
    }
#ifndef _WIN32
	glfwTerminate();
#endif
    if (OGLobj != NULL) delete OGLobj;
    if (camera != NULL) delete camera;
    if (model != NULL) delete model;
#ifdef _WIN32
    return (int) msg.wParam;
#else
	return 0;
#endif
}

void mouseActions() {
#ifdef _WIN32 
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    cDelta.setPosition(p.x, p.y, cDelta.getLbtn() || cDelta.getRbtn());
#else
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cDelta.setPosition(x, y, cDelta.getLbtn() || cDelta.getRbtn());
#endif
}



// Variables para los modelos que se coleccionarán

bool checkCondiciones = false;
bool checkMuertes = false;
bool checkGanars = false;
int contadorColeccion = 0;
int condicion1 = 0;
int condicion2 = 0;
int condicion3 = 0;


bool checkInput(GameActions *actions, Scene* scene) {
    bool checkCollition = false;
#ifdef _WIN32 
    GamePadRR gamepad(1);
    if (gamPad->IsConnected()) {
        Model* OGLobj = scene->getMainModel();
        //convierto a flotante el valor analogico de tipo entero
        double grados = (float)gamPad->GetState().Gamepad.sThumbLX / 32767.0;
        //debido a que los controles se aguadean con el uso entonces ya no dan el cero
        //en el centro, por eso lo comparo con una ventana de aguadencia de mi control
        if (grados > 0.19 || grados < -0.19)
            //model->CamaraGiraY(grados * 3.0);
            actions->setAngle(grados * 3.0);
        float velocidad = (float)gamPad->GetState().Gamepad.sThumbLY / 32767;
        if (velocidad > 0.19 || velocidad < -0.19) {
            //model->movePosition(velocidad);
            actions->advance = velocidad;
            checkCollition = true;
        }
        XINPUT_STATE state = gamepad.GetState();

        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_B)) {
            string texto = to_string(OGLobj->getTranslate()->x) + ","
                + to_string(OGLobj->getTranslate()->y) + ","
                + to_string(OGLobj->getTranslate()->z);
            LOGGER::LOGS::getLOGGER("DemoTemplate").info(texto);
        }
        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_Y)) {
            string texto = to_string(OGLobj->getTranslate()->x) + ","
                + to_string(OGLobj->getTranslate()->y) + ","
                + to_string(OGLobj->getTranslate()->z);
            LOGGER::LOGS::getLOGGER("DemoTemplate").info(texto);
        }
        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_DPAD_UP)) {
            actions->advance = 4;
            checkCollition = true;
        }
        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
            actions->advance = -4;
            checkCollition = true;
        }
        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
            actions->sideAdvance = 4;
            checkCollition = true;
        }
        if (gamepad.IsButtonPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
            actions->sideAdvance = -4;
            checkCollition = true;
        }
#else
    if (false) {
        cout << "This should be the gamepad code \n";
#endif
    } else {
        mouseActions();
        checkCollition = KeysEvents(actions);
    }
    Model* OGLobj = scene->getMainModel();

    if (actions->mostrarPosicion) {
        string texto = to_string(OGLobj->getTranslate()->x) + ","
            + to_string(OGLobj->getTranslate()->y) + "," 
            + to_string(OGLobj->getTranslate()->z);
        LOGGER::LOGS::getLOGGER("DemoTemplate").info(texto);
    }

  
    /*if (OGLobj->getTranslate()->x < -65 && OGLobj->getTranslate()->x > -80 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z < -10 && OGLobj->getTranslate()->z > -20) {

        condicion1 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (OGLobj->getTranslate()->x < -113 && OGLobj->getTranslate()->x > -117 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z > 72 && OGLobj->getTranslate()->x < 75) {
        condicion2 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (OGLobj->getTranslate()->x < -50 && OGLobj->getTranslate()->x > -65 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z < -115 && OGLobj->getTranslate()->z > -128) {
        condicion3 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (contadorColeccion == 3) {

        checkCondiciones = true;
    }
    if (actions->mostrarInventario) {
        if (checkCondiciones) {
            LOGGER::LOGS::getLOGGER("DemoTemplate").info("YA OBTUVISTE TODAS LAS PIEZAS! EL DIAMANTE DEL PODER HA SIDO CREADO! EN LA ENTRADA SE ENCUENTRA EL VERDADERO PODER");
        }
        else {
            string texto = to_string(contadorColeccion) + " / 3 " + to_string(condicion1) + to_string(condicion2) + to_string(condicion3);
        LOGGER::LOGS::getLOGGER("DemoTemplate").info(texto);
        }

        
    }*/


    if (actions->firstPerson) {
        OGLobj->cameraDetails->setFirstPerson(!OGLobj->cameraDetails->getFirstPerson());
        vector<Model*>*models = scene->getLoadedModels();
        glm::vec3 light(rand() % 100, rand() % 100, rand() % 100);
        for (Model* model : *models) {
            model->lightPos = light;
        }
        //scene->getTerreno()->lightPos = light;
    }
    if (actions->sideAdvance != 0) {
        OGLobj->setNextRotY(OGLobj->getNextRotY() + (5.0 * actions->sideAdvance));
        checkCollition = true;
    }
    if (actions->hAdvance != 0) {
        glm::vec3 pos = *OGLobj->getTranslate();
        pos.x += actions->hAdvance * 1.5 * glm::cos(glm::radians(OGLobj->getRotY()));
        pos.z += actions->hAdvance * 1.5 * glm::sin(glm::radians(OGLobj->getRotY()));
        // Posicionamos la camara/modelo pixeles arriba de su posicion en el terreno
        pos.y = scene->getTerreno()->Superficie(pos.x, pos.z);

        OGLobj->setNextTranslate(&pos);
        checkCollition = true;
    }
    if (actions->advance != 0) {
        glm::vec3 pos = *OGLobj->getTranslate();
        pos.x += actions->advance * 1.5 * glm::sin(glm::radians(OGLobj->getRotY()));
        pos.z += actions->advance * 1.5 * glm::cos(glm::radians(OGLobj->getRotY()));
        // Posicionamos la camara/modelo pixeles arriba de su posicion en el terreno
        pos.y = scene->getTerreno()->Superficie(pos.x, pos.z);
        OGLobj->setNextTranslate(&pos);
        checkCollition = true;
    }
    if (actions->getAngle() != NULL) {
        OGLobj->cameraDetails->calculateAngleAroundPlayer((*actions->getAngle()) * 3.0);
    }
    if (actions->getPitch() != NULL) {
        OGLobj->cameraDetails->setPitch(OGLobj->cameraDetails->getPitch() + (*actions->getPitch()) * 0.50 * 3.0);
    }
    if (actions->getZoom() != NULL) {
        OGLobj->cameraDetails->setZoom(OGLobj->cameraDetails->getZoom() + *actions->getZoom() * 0.50);
    }
    if (actions->getPlayerZoom() != NULL) {
        OGLobj->cameraDetails->calculateZoomPlayer(*actions->getPlayerZoom() * 0.50);
    }

    return checkCollition;
}

bool checkCondicion(GameActions* actions, Scene* scene) {

    Model* OGLobj = scene->getMainModel();
    Model* model;
    mouseActions();
    checkCondiciones = KeysEvents(actions);

    if (OGLobj->getTranslate()->x < -65 && OGLobj->getTranslate()->x > -80 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z < -10 && OGLobj->getTranslate()->z > -20) {
        
        condicion1 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (OGLobj->getTranslate()->x < -110 && OGLobj->getTranslate()->x > -118 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z > 68 && OGLobj->getTranslate()->x < 75) {
        condicion2 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (OGLobj->getTranslate()->x < -50 && OGLobj->getTranslate()->x > -65 &&
        OGLobj->getTranslate()->y > 9 &&
        OGLobj->getTranslate()->y < 9.2 &&
        OGLobj->getTranslate()->z < -115 && OGLobj->getTranslate()->z > -128) {
        condicion3 = 1;
        contadorColeccion = condicion1 + condicion2 + condicion3;
    }
    if (contadorColeccion == 3) {
        checkCondiciones = true;
    }
    if (actions->mostrarInventario) {
        if (checkCondiciones) {
            LOGGER::LOGS::getLOGGER("DemoTemplate").info("YA OBTUVISTE TODAS LAS PIEZAS! EL DIAMANTE DEL PODER HA SIDO CREADO! EN LA ENTRADA SE ENCUENTRA EL VERDADERO PODER");
            
        }
        else {
            string texto = "Debes de obtener todos los minerales para poder crear el diamante del poder y con el podras vencer a los monstruos que protegen el castillo!   " + to_string(contadorColeccion) + " / 3 piezas  -" + to_string(condicion1) + " Esmeralda del Caos - " + to_string(condicion2) + " Esmeralda de la Paz - " + to_string(condicion3) + " Esmeralda de la Sabiduria";
            LOGGER::LOGS::getLOGGER("DemoTemplate").info(texto);
        }
    }
    return checkCondiciones;
}

bool checkMuerte(GameActions* actions, Scene* scene) {

    Model* OGLobj = scene->getMainModel();
    Model* model;
    mouseActions();
    checkMuertes = KeysEvents(actions);

    if (!checkCondiciones) {
        if (OGLobj->getTranslate()->x > 93 && OGLobj->getTranslate()->x < 95 &&
            OGLobj->getTranslate()->y > 3 &&
            OGLobj->getTranslate()->y < 4 &&
            OGLobj->getTranslate()->z > -9.7 && OGLobj->getTranslate()->z < 20.29
            || OGLobj->getTranslate()->x > -62 && OGLobj->getTranslate()->x < -54 &&
            OGLobj->getTranslate()->y > 9 &&
            OGLobj->getTranslate()->y < 9.2 &&
            OGLobj->getTranslate()->z < -99 && OGLobj->getTranslate()->z > -103) {

            checkMuertes = true;
        }
    }
    else {
        checkMuertes = false;
    }
    
    return checkMuertes;
}

bool checkGanar(GameActions* actions, Scene* scene) {
    Model* OGLobj = scene->getMainModel();
    Model* model;
    mouseActions();
    checkGanars = KeysEvents(actions);

    if (!checkCondiciones) {
        if (OGLobj->getTranslate()->x < -12.5 && OGLobj->getTranslate()->x > -21 &&
            OGLobj->getTranslate()->y > 6.8 &&
            OGLobj->getTranslate()->y < 7 &&
            OGLobj->getTranslate()->z > -8.0 && OGLobj->getTranslate()->z < -3.5) {

            checkGanars = true;
        }
    }
    else {
        checkGanars = false;
    }

    return checkGanars;
}

#ifdef _WIN32
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
        } break;
        case WM_COMMAND: {
        } break;
        case WM_MOUSEMOVE: {
            int value = lParam;
        }break;
        case WM_TIMER: {
            if (OGLobj != NULL)
                OGLobj->setAngulo(OGLobj->getAngulo() + 1.5);
//            if (!renderiza)
//                renderiza = true;
        } break;
        case WM_PAINT: {
        }break;
        case WM_DESTROY: {
            if (newContext) {
                ReleaseDC(hWnd, dc);
                wglDeleteContext(rc);
                PostQuitMessage(0);
            }
        } break;
        case WM_SIZE: {
            if (newContext) {
                //esta opcion del switch se ejecuta una sola vez al arrancar y si se
                //afecta el tama�o de la misma se dispara de nuevo
                int height = HIWORD(lParam),
                    width = LOWORD(lParam);
                if (height == 0)
                    width = 1;
                SCR_HEIGHT = height;
                SCR_WIDTH = width;
                glViewport(0, 0, width, height);
            }
        } break;
        case WM_LBUTTONDOWN: {
            cDelta.setLbtn(true);
        }break;
        case WM_LBUTTONUP: {
            cDelta.setLbtn(false);
        }break;
        case WM_RBUTTONDOWN: {
            cDelta.setRbtn(true);
        }break;
        case WM_RBUTTONUP: {
            cDelta.setRbtn(false);
        }break;
        case WM_MOUSEWHEEL: {
            char delta = HIWORD(wParam);
            cDelta.setMouseWheel(delta);
        }break;
        case WM_KEYDOWN: {
            KEYS[wParam] = true;
        } break;
        case WM_KEYUP: {
            if (wParam == KEYB_CAMERA || wParam == KEYB_HMOVEMENT)
                KEYS[wParam] = false;
        } break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetUpPixelFormat(HDC hDC, PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB, PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB) {
    const int pixelAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
//        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
//        WGL_SAMPLES_ARB, 4,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_ALPHA_BITS_ARB, 8,
        0 
    };
    int pixelFormatID;
    UINT numFormats;
    bool status = wglChoosePixelFormatARB(hDC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
    if (status == false || numFormats == 0)
        return true;
    PIXELFORMATDESCRIPTOR PFD;
    DescribePixelFormat(hDC, pixelFormatID, sizeof(PFD), &PFD);
    SetPixelFormat(hDC, pixelFormatID, &PFD);
    return false;
}

// Funciones de inicializacion para ventana compatible con OpenGL
int prepareRenderWindow(HINSTANCE hInstance, int nCmdShow) {
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = szWindowClass;
    RegisterClassEx(&wc);
    hInst = hInstance;
    HWND fakeWND = CreateWindow(
        szWindowClass, L"Fake Window",      // window class, title
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // style
        0, 0,                       // position x, y
        1, 1,                       // width, height
        NULL, NULL,                 // parent window, menu
        hInstance, NULL);           // instance, param
    HDC fakeDC = GetDC(fakeWND);        // Device Context
    PIXELFORMATDESCRIPTOR fakePFD;
    ZeroMemory(&fakePFD, sizeof(fakePFD));
    fakePFD.nSize = sizeof(fakePFD);
    fakePFD.nVersion = 1;
    fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    fakePFD.iPixelType = PFD_TYPE_RGBA;
    fakePFD.cColorBits = 32;
    fakePFD.cAlphaBits = 8;
    fakePFD.cDepthBits = 24;
    int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
    if (fakePFDID == 0) {
        MessageBox(fakeWND, L"ChoosePixelFormat() failed.", L"", 0);
        return 1;
    }
    if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
        MessageBox(fakeWND, L"SetPixelFormat() failed.", L"", 0);
        return 1;
    }
    HGLRC fakeRC = wglCreateContext(fakeDC);    // Rendering Contex
    if (fakeRC == 0) {
        MessageBox(fakeWND, L"wglCreateContext() failed.", L"", 0);
        return 1;
    }
    if (wglMakeCurrent(fakeDC, fakeRC) == false) {
        MessageBox(fakeWND, L"wglMakeCurrent() failed.", L"", 0);
        return 1;
    }
    wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
    if (wglChoosePixelFormatARB == nullptr) {
        MessageBox(fakeWND, L"wglGetProcAddress() failed.", L"", 0);
        return 1;
    }
    wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
    if (wglCreateContextAttribsARB == nullptr) {
        MessageBox(fakeWND, L"wglGetProcAddress() failed.", L"", 0);
        return 1;
    }
    gladLoadGL();
    // create window
    RECT wr = { 0, 0, SCR_WIDTH, SCR_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100,
        wr.right - wr.left, //SCR_WIDTH, 
        wr.bottom - wr.top, //SCR_HEIGHT,
        NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 1;
    }
    dc = NULL;
    dc = GetDC(hWnd);
    if (SetUpPixelFormat(dc, wglChoosePixelFormatARB, wglCreateContextAttribsARB)) {
        MessageBox(hWnd, L"wglChoosePixelFormatARB() failed.", L"", 0);
        return 1;
    }
    const int major_min = 3, minor_min = 2;
    int  contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    rc = NULL;
//    rc = wglCreateContext(dc);
    rc = wglCreateContextAttribsARB(dc, 0, contextAttribs);
    if (rc == NULL) {
        MessageBox(hWnd, L"wglCreateContextAttribsARB() failed.", L"", 0);
        return 1;
    }
//    wglMakeCurrent(dc, rc);
    wglDeleteContext(fakeRC);
    ReleaseDC(fakeWND, fakeDC);
    DestroyWindow(fakeWND);
    if (!wglMakeCurrent(dc, rc)) {
        MessageBox(hWnd, L"wglMakeCurrent() failed.", L"", 0);
        return 1;
    }
    gladLoadGL();
    newContext = true;
    return 0;
}
#else
void window_size_callback(GLFWwindow* window, int width, int height){
    if (height == 0)
        width = 1;
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    char k = (key == GLFW_KEY_LEFT_SHIFT)? input.Shift : key;
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        KEYS[k] = true;
    else
        if (k == KEYB_CAMERA || k == KEYB_HMOVEMENT)
            KEYS[k] = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        cDelta.setRbtn(action == GLFW_PRESS);
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
        cDelta.setLbtn(action == GLFW_PRESS);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    char delta = yoffset;
    cDelta.setMouseWheel(delta);
}
#endif
