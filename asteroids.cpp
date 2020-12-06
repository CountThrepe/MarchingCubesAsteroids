#include <vector>
#include <iostream>

#include "CSCIx229.h"
#include "Point.hpp"
#include "PointArray.hpp"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// Display parameters
const int windowX = 900;
const int windowY = 700;

// View variables
const float skyboxDistance = 8;

int th = 0;
int ph = 0;
int fov = 55;
float asp = 1;
float dim = 5;

int view = 1;
const int numViews = 2;
const char viewNames[numViews][25] = {"Point Array", "Mesh"};

// Lighting variables
int ambient = 10;
int diffuse = 50;
int specular = 10;

int zh = 0;
float lightY = 1.5;
float radius = 3;

Point* light = NULL;

float threshold = 0.3;
float scale = 0.2;
const int nDimensions = 5;
const int dimensions[nDimensions] = {5, 10, 15, 25, 50};
int dimension = 2;
bool cycling = true;
bool vignette = true;
bool interpolate = true;
bool texture = true;
PointArray* pointArr = NULL;

unsigned int skyboxTextures[6] = {0, 0, 0, 0, 0, 0};

void cleanExit() {
    delete pointArr;
    delete light;

    exit(0);
}

void initScene() {
    pointArr = new PointArray(dimensions[dimension], dimensions[dimension], dimensions[dimension], scale, threshold, vignette, interpolate);

    light = new Point(5, 5, 5, 0.1, 1);
}

void resetScene() {
    pointArr->scramble();

    delete light;
    light = new Point(5, 5, 5, 0.1, 1);
}

void drawSkybox() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(-skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,0); glVertex3f(skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(skyboxDistance, -skyboxDistance, skyboxDistance);
    glTexCoord2f(0,1); glVertex3f(-skyboxDistance, -skyboxDistance, skyboxDistance);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3f(-skyboxDistance, skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(skyboxDistance, skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,0); glVertex3f(skyboxDistance, skyboxDistance, skyboxDistance);
    glTexCoord2f(0,0); glVertex3f(-skyboxDistance, skyboxDistance, skyboxDistance);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(-skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(0,0); glVertex3f(skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(0,1); glVertex3f(skyboxDistance, skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(-skyboxDistance, skyboxDistance, -skyboxDistance);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(0,0); glVertex3f(skyboxDistance, -skyboxDistance, skyboxDistance);
    glTexCoord2f(0,1); glVertex3f(skyboxDistance, skyboxDistance, skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(skyboxDistance, skyboxDistance, -skyboxDistance);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(skyboxDistance, -skyboxDistance, skyboxDistance);
    glTexCoord2f(0,0); glVertex3f(-skyboxDistance, -skyboxDistance, skyboxDistance);
    glTexCoord2f(0,1); glVertex3f(-skyboxDistance, skyboxDistance, skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(skyboxDistance, skyboxDistance, skyboxDistance);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, skyboxTextures[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(-skyboxDistance, -skyboxDistance, skyboxDistance);
    glTexCoord2f(0,0); glVertex3f(-skyboxDistance, -skyboxDistance, -skyboxDistance);
    glTexCoord2f(0,1); glVertex3f(-skyboxDistance, skyboxDistance, -skyboxDistance);
    glTexCoord2f(1,1); glVertex3f(-skyboxDistance, skyboxDistance, skyboxDistance);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display(SDL_Window* window) {
    // Reset everything
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set view (based on example 9)
    float Ex = -1.2*dim*Sin(th)*Cos(ph);
    float Ey = +1.2*dim        *Sin(ph);
    float Ez = +1.2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(ph),0);

    // Update lighting
    float position[] = {radius * (float) Cos(zh), lightY, radius * (float) Sin(zh), 1.0};

    light->setPos(position[0], position[1], position[2]);
    glColor3f(1, 1, 1);
    light->render();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Display scene
    if(view == 0) pointArr->renderPoints();
    else if(view == 1) pointArr->renderMesh();

    // Disable lighting
    glDisable(GL_LIGHTING);

    //Skybox
    glColor3f(1, 1, 1);
    drawSkybox();

    // Print onscreen text
    glWindowPos2i(5,5);
    Print("%s | Threshold: %.2f | Scale: %.2f | Offset: %s | Dimensions: %i | I: %s | V: %s",
          viewNames[view], threshold, scale, (cycling ? "Cycling" : "Still"), dimensions[dimension],
          (interpolate ? "On" : "Off"), (vignette ? "On" : "Off"));

    // Show rendered scene
    ErrCheck("display");
    glFlush();
    SDL_GL_SwapWindow(window);
}

void idle() {
    if(cycling) pointArr->incrementOffset();
    pointArr->update();
}

void updateProjection() { // Based on example 9
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, asp, dim/4, 4*dim);
      
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void reshape(SDL_Window* window) {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    asp = (height > 0) ? (double) width / height : 1;
    glViewport(0, 0, width, height);

    updateProjection();
}

int key() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    int shift = SDL_GetModState()&KMOD_SHIFT;

    if(keys[SDL_SCANCODE_ESCAPE]) cleanExit();
    else if(keys[SDL_SCANCODE_R]) resetScene();
    else if(keys[SDL_SCANCODE_0]) {
        th = ph = 0;
    }
    else if(keys[SDL_SCANCODE_SPACE]) view = (view+1)%numViews;

    // Light controls
    else if(keys[SDL_SCANCODE_COMMA] && shift) radius -= 0.1;
    else if(keys[SDL_SCANCODE_PERIOD] && shift) radius += 0.1;
    else if(keys[SDL_SCANCODE_COMMA] && !shift) lightY -= 0.1;
    else if(keys[SDL_SCANCODE_PERIOD] && !shift) lightY += 0.1;
    else if(keys[SDL_SCANCODE_SEMICOLON]) zh -= 4;
    else if(keys[SDL_SCANCODE_APOSTROPHE]) zh += 4;

    // Camera controls
    else if(keys[SDL_SCANCODE_RIGHT]) th -= 5;
    else if(keys[SDL_SCANCODE_LEFT]) th += 5;
    else if(keys[SDL_SCANCODE_UP]) ph += 5;
    else if(keys[SDL_SCANCODE_DOWN]) ph -= 5;

    // PointArray controls
    else if(keys[SDL_SCANCODE_T] && shift && threshold < 1) {
        threshold += 0.05;
        pointArr->setThreshold(threshold);
    } else if(keys[SDL_SCANCODE_T] && !shift && threshold >= 0.05) {
        threshold -= 0.05;
        pointArr->setThreshold(threshold);
    } else if(keys[SDL_SCANCODE_S] && shift) {
        scale += 0.02;
        pointArr->setScale(scale);
    } else if(keys[SDL_SCANCODE_S] && !shift && scale >= 0.02) {
        scale -= 0.02;
        pointArr->setScale(scale);
    } else if(keys[SDL_SCANCODE_I]) {
        interpolate = !interpolate;
        pointArr->setInterpolate(interpolate);
    } else if(keys[SDL_SCANCODE_V]) {
        vignette = !vignette;
        pointArr->setVignette(vignette);
    } else if(keys[SDL_SCANCODE_D] && shift && dimension < nDimensions - 1) {
        dimension++;
        cycling = false;
        pointArr->setDimensions(dimensions[dimension], dimensions[dimension], dimensions[dimension]);
    } else if(keys[SDL_SCANCODE_D] && !shift && dimension > 0) {
        dimension--;
        cycling = false;
        pointArr->setDimensions(dimensions[dimension], dimensions[dimension], dimensions[dimension]);
    } else if(keys[SDL_SCANCODE_O]) cycling = !cycling && dimension < 3;
    else if(keys[SDL_SCANCODE_X]) {
        texture = !texture;
        pointArr->setTexture(texture);
    }

    th %= 360;
    ph %= 360;

    updateProjection();

    return 1;
}

int main(int argc, char* argv[]) {
    int run=1;
    double t0=0;

    //  Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    //  Set size, resizable and double buffering
    SDL_Window* window = SDL_CreateWindow("Final | Nathan Howard",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,windowX,windowY,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (!window ) Fatal("Cannot create window\n");
    SDL_GL_CreateContext(window);
#ifdef USEGLEW
    if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    //  Set screen size
    reshape(window);

    glEnable(GL_DEPTH_TEST);

    // Setup lighting
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    float ambArr[] = {0.01f*ambient, 0.01f*ambient, 0.01f*ambient, 1.0f};
    float diffArr[] = {0.01f*diffuse, 0.01f*diffuse, 0.01f*diffuse, 1.0f};
    float specArr[] = {0.01f*specular, 0.01f*specular, 0.01f*specular, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambArr);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffArr);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specArr);

    skyboxTextures[0] = LoadTexBMP("skyboxBottom.bmp");
    skyboxTextures[1] = LoadTexBMP("skyboxTop.bmp");
    skyboxTextures[2] = LoadTexBMP("skyboxFront.bmp");
    skyboxTextures[3] = LoadTexBMP("skyboxRight.bmp");
    skyboxTextures[4] = LoadTexBMP("skyboxBack.bmp");
    skyboxTextures[5] = LoadTexBMP("skyboxLeft.bmp");

    initScene();

    //  SDL event loop
    ErrCheck("init");
    while (run) {
        //  Elapsed time in seconds
        double t = SDL_GetTicks()/1000.0;

        //  Process all pending events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT:
                    if (event.window.event==SDL_WINDOWEVENT_RESIZED) {
                        SDL_SetWindowSize(window,event.window.data1,event.window.data2);
                        reshape(window);
                    }
                    break;
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_KEYDOWN:
                    run = key();
                    t0 = t+0.5;  // Wait 1/2 s before repeating
                    break;
                default:
                    //  Do nothing
                    break;
            }
        }

        //  Repeat key every 50 ms
        if (t-t0>0.05)
        {
            run = key();
            t0 = t;
        }
        //  Display
        display(window);
        idle();
        //  Slow down display rate to about 100 fps by sleeping 5ms
        SDL_Delay(5);
    }

    //  Shut down SDL
    SDL_Quit();
    cleanExit();
}