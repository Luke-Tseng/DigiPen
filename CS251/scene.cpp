
////////////////////////////////////////////////////////////////////////
// The scene class contains all the parameters needed to define and
// draw a simple scene, including:
//   * Geometry
//   * Light parameters
//   * Material properties
//   * viewport size parameters
//   * Viewing transformation values
//   * others ...
//
// Some of these parameters are set when the scene is built, and
// others are set by the framework in response to user mouse/keyboard
// interactions.  All of them can be used to draw the scene.

#include "math.h"
#include <iostream>
#include <stdlib.h>

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
using namespace gl;

#include <glu.h> // For gluErrorString

#define GLM_FORCE_CTOR_INIT
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp> // For printing GLM objects with to_string

#include "framework.h"
#include "shapes.h"
#include "object.h"
#include "texture.h"
#include "transform.h"
const bool fullPolyCount = true; // Use false when emulating the graphics pipeline in software

const float PI = 3.14159f;
const float rad = PI / 180.0f; // Convert degrees to radians

glm::mat4 Identity(1.0);

const float grndSize = 100.0;       // Island radius;  Minimum about 20;  Maximum 1000 or so
const float grndOctaves = 4.0;      // Number of levels of detail to compute
const float grndFreq = 0.03;        // Number of hills per (approx) 50m
const float grndPersistence = 0.03; // Terrain roughness: Slight:0.01  rough:0.05
const float grndLow = -3.0;         // Lowest extent below sea level
const float grndHigh = 5.0;         // Highest extent above sea level

////////////////////////////////////////////////////////////////////////
// This macro makes it easy to sprinkle checks for OpenGL errors
// throughout your code.  Most OpenGL calls can record errors, and a
// careful programmer will check the error status *often*, perhaps as
// often as after every OpenGL call.  At the very least, once per
// refresh will tell you if something is going wrong.
#define CHECKERROR                                                                                       \
    {                                                                                                    \
        GLenum err = glGetError();                                                                       \
        if (err != GL_NO_ERROR)                                                                          \
        {                                                                                                \
            fprintf(stderr, "OpenGL error (at line scene.cpp:%d): %s\n", __LINE__, gluErrorString(err)); \
            exit(-1);                                                                                    \
        }                                                                                                \
    }

// Create an RGB color from human friendly parameters: hue, saturation, value
glm::vec3 HSV2RGB(const float h, const float s, const float v)
{
	if (s == 0.0)
		return glm::vec3(v, v, v);

	int i = (int)(h * 6.0) % 6;
	float f = (h * 6.0f) - i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));
	if (i == 0)
		return glm::vec3(v, t, p);
	else if (i == 1)
		return glm::vec3(q, v, p);
	else if (i == 2)
		return glm::vec3(p, v, t);
	else if (i == 3)
		return glm::vec3(p, q, v);
	else if (i == 4)
		return glm::vec3(t, p, v);
	else /*i == 5*/
		return glm::vec3(v, p, q);
}

////////////////////////////////////////////////////////////////////////
// Constructs a hemisphere of spheres of varying hues
Object* SphereOfSpheres(Shape* SpherePolygons)
{
	Object* ob = new Object(NULL, nullId);

	for (float angle = 0.0; angle < 360.0; angle += 18.0)
		for (float row = 0.075; row < PI / 2.0; row += PI / 2.0 / 6.0)
		{
			glm::vec3 hue = HSV2RGB(angle / 360.0, 1.0f - 2.0f * row / PI, 1.0f);

			Object* sp = new Object(SpherePolygons, spheresId,
				hue, glm::vec3(0.05, 0.05, 0.05), 120.0);
			float s = sin(row);
			float c = cos(row);
			ob->add(sp, Rotate(2, angle) * Translate(c, 0, s) * Scale(0.075 * c, 0.075 * c, 0.075 * c));
		}
	return ob;
}

////////////////////////////////////////////////////////////////////////
// Constructs a -1...+1  quad (canvas) framed by four (elongated) boxes
Object* FramedPicture(const glm::mat4& modelTr, const int objectId,
	Shape* BoxPolygons, Shape* QuadPolygons, Texture* texture)
{
	// This draws the frame as four (elongated) boxes of size +-1.0
	float w = 0.05; // Width of frame boards.

	Object* frame = new Object(NULL, nullId);
	Object* ob;

	glm::vec3 woodColor(87.0 / 255.0, 51.0 / 255.0, 35.0 / 255.0);
	ob = new Object(BoxPolygons, frameId,
		woodColor, glm::vec3(0.02, 0.02, 0.02), 10.0);
	frame->add(ob, Translate(0.0, 0.0, 1.0 + w) * Scale(1.0, w, w));
	frame->add(ob, Translate(0.0, 0.0, -1.0 - w) * Scale(1.0, w, w));
	frame->add(ob, Translate(1.0 + w, 0.0, 0.0) * Scale(w, w, 1.0 + 2 * w));
	frame->add(ob, Translate(-1.0 - w, 0.0, 0.0) * Scale(w, w, 1.0 + 2 * w));

	ob = new Object(QuadPolygons, objectId,
		woodColor, glm::vec3(0.0, 0.0, 0.0), 10.0, texture);
	frame->add(ob, Rotate(0, 90));

	return frame;
}

////////////////////////////////////////////////////////////////////////
// InitializeScene is called once during setup to create all the
// textures, shape VAOs, and shader programs as well as setting a
// number of other parameters.
void Scene::InitializeScene()
{
	glEnable(GL_DEPTH_TEST);
	CHECKERROR;

	// @@ Initialize interactive viewing variables here. (spin, tilt, ry, front back, ...)
	spin = 0.0;
	tilt = 30.0;
	tx = 0.0;
	ty = 0.0;
	zoom = 25.0;
	ry = 0.4;
	rx = 0.0;
	front = 0.5;
	back = 5000.0;
	eye = glm::vec3(0.0, -20.0, 0.0);
	speed = 10.0;
	w_down = false;
	a_down = false;
	s_down = false;
	d_down = false;
	transformation_mode = true;

	// Set initial light parameters
	lightSpin = 150.0;
	lightTilt = -45.0;
	lightDist = 100.0;
	// @@ Perhaps initialize additional scene lighting values here. (lightVal, lightAmb)

	CHECKERROR;
	objectRoot = new Object(NULL, nullId);

	// Enable OpenGL depth-testing
	glEnable(GL_DEPTH_TEST);

	// Create the lighting shader program from source code files.
	// @@ Initialize additional shaders if necessary
	lightingProgram = new ShaderProgram();
	lightingProgram->AddShader("lighting.vert", GL_VERTEX_SHADER);
	lightingProgram->AddShader("lighting.frag", GL_FRAGMENT_SHADER);

	glBindAttribLocation(lightingProgram->programId, 0, "vertex");
	glBindAttribLocation(lightingProgram->programId, 1, "vertexNormal");
	glBindAttribLocation(lightingProgram->programId, 2, "vertexTexture");
	glBindAttribLocation(lightingProgram->programId, 3, "vertexTangent");
	lightingProgram->LinkProgram();

	// Create all the Polygon shapes
	proceduralground = new ProceduralGround(grndSize, 400,
		grndOctaves, grndFreq, grndPersistence,
		grndLow, grndHigh);

	Shape* TeapotPolygons = new Teapot(fullPolyCount ? 12 : 2);
	Shape* BoxPolygons = new Box();
	Shape* SpherePolygons = new Sphere(32);
	Shape* RoomPolygons = new Ply("room.ply");
	Shape* FloorPolygons = new Plane(10.0, 10);
	Shape* QuadPolygons = new Quad();
	Shape* SeaPolygons = new Plane(2000.0, 50);
	Shape* GroundPolygons = proceduralground;

	// Various colors used in the subsequent models
	glm::vec3 woodColor(87.0 / 255.0, 51.0 / 255.0, 35.0 / 255.0);
	glm::vec3 brickColor(134.0 / 255.0, 60.0 / 255.0, 56.0 / 255.0);
	glm::vec3 floorColor(6 * 16 / 255.0, 5.5 * 16 / 255.0, 3 * 16 / 255.0);
	glm::vec3 brassColor(0.5, 0.5, 0.1);
	glm::vec3 grassColor(62.0 / 255.0, 102.0 / 255.0, 38.0 / 255.0);
	glm::vec3 waterColor(0.3, 0.3, 1.0);

	glm::vec3 black(0.0, 0.0, 0.0);
	glm::vec3 brightSpec(0.05, 0.05, 0.05);
	glm::vec3 polishedSpec(0.03, 0.03, 0.03);

	// Creates all the models from which the scene is composed.  Each
	// is created with a polygon shape (possibly NULL), a
	// transformation, and the surface lighting parameters Kd, Ks, and
	// alpha.

	// @@ This is where you could read in all the textures and
	// associate them with the various objects being created in the
	// next dozen lines of code.

	Texture* teapotTexture = new Texture("textures/cracks.png");
	Texture* boxTexture = new Texture("textures/Brazilian_rosewood_pxr128.png");
	Texture* floorTexture = new Texture("textures/6670-diffuse.jpg");
	Texture* groundTexture = new Texture("textures/grass.jpg");
	Texture* houseTexture = new Texture("textures/my-house-01.png");
	Texture* roomTexture = new Texture("textures/Standard_red_pxr128.png");
	Texture* skyTexture = new Texture("skys/sky.jpg");

	Texture* boxNormalMap = new Texture("textures/Brazilian_rosewood_pxr128_normal.png");
	Texture* roomNormalMap = new Texture("textures/Standard_red_pxr128_normal.png");
	Texture* seaNormalMap = new Texture("textures/ripples_normalmap.png");
	Texture* floorNormalMap = new Texture("textures/6670-normal.jpg");


	// @@ To change an object's surface parameters (Kd, Ks, or alpha),
	// modify the following lines.

	sky = new Object(SpherePolygons, skyId, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, skyTexture);
	Object::skyTexture = skyTexture;

	central = new Object(NULL, nullId);
	anim = new Object(NULL, nullId);
	room = new Object(RoomPolygons, roomId, brickColor, black, 1, roomTexture, roomNormalMap);
	floor = new Object(FloorPolygons, floorId, floorColor, black, 1, floorTexture, floorNormalMap);
	teapot = new Object(TeapotPolygons, teapotId, brassColor, brightSpec, 120, teapotTexture);
	podium = new Object(BoxPolygons, boxId, glm::vec3(woodColor), polishedSpec, 10, boxTexture, boxNormalMap);
	ground = new Object(GroundPolygons, groundId, grassColor, black, 1, groundTexture);
	sea = new Object(SeaPolygons, seaId, waterColor, brightSpec, 120, NULL, seaNormalMap);

	leftFrame = FramedPicture(Identity, lPicId, BoxPolygons, QuadPolygons, NULL);
	rightFrame = FramedPicture(Identity, rPicId, BoxPolygons, QuadPolygons, houseTexture);
	spheres = SphereOfSpheres(SpherePolygons);
#ifdef REFL
	spheres->drawMe = true;
#else
	spheres->drawMe = false;
#endif

	// @@ To change the scene hierarchy, examine the hierarchy created
	// by the following object->add() calls and adjust as you wish.
	// The objects being manipulated and their polygon shapes are
	// created above here.

	// Scene is composed of sky, ground, sea, room and some central models
	if (fullPolyCount)
	{
		objectRoot->add(sky, Scale(2000.0, 2000.0, 2000.0));
		objectRoot->add(sea);
		objectRoot->add(ground);
	}
	objectRoot->add(central);
#ifndef REFL
	objectRoot->add(room, Translate(0.0, 0.0, 0.02));
#endif
	objectRoot->add(floor, Translate(0.0, 0.0, 0.02));

	// Central model has a rudimentary animation (constant rotation on Z)
	animated.push_back(anim);

	// Central contains a teapot on a podium and an external sphere of spheres
	central->add(podium, Translate(0.0, 0, 0));
	central->add(anim, Translate(0.0, 0, 0));
	anim->add(teapot, Translate(0, 0, 1) * Scale(0.31, 0.31, 0.31));

	if (fullPolyCount)
		anim->add(spheres, Translate(0.0, 0.0, 0.0) * Scale(16, 16, 16));

	// Room contains two framed pictures
	if (fullPolyCount)
	{
		room->add(leftFrame, Translate(-1.5, 9.85, 1.) * Scale(0.8, 0.8, 0.8));
		room->add(rightFrame, Translate(1.5, 9.85, 1.) * Scale(0.8, 0.8, 0.8));
	}

	CHECKERROR;

	// Options menu stuff
	show_demo_window = false;
}

void Scene::DrawMenu()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		// This menu demonstrates how to provide the user a list of toggleable settings.
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Draw spheres", "", spheres->drawMe))
			{
				spheres->drawMe ^= true;
			}
			if (ImGui::MenuItem("Draw walls", "", room->drawMe))
			{
				room->drawMe ^= true;
			}
			if (ImGui::MenuItem("Draw ground/sea", "", ground->drawMe))
			{
				ground->drawMe ^= true;
				sea->drawMe = ground->drawMe;
			}
			ImGui::EndMenu();
		}

		// This menu demonstrates how to provide the user a choice
		// among a set of choices.  The current choice is stored in a
		// variable named "mode" in the application, and sent to the
		// shader to be used as you wish.
		if (ImGui::BeginMenu("Menu "))
		{
			if (ImGui::MenuItem("<sample menu of choices>", "", false, false))
			{
			}
			if (ImGui::MenuItem("Do nothing 0", "", mode == 0))
			{
				mode = 0;
			}
			if (ImGui::MenuItem("Do nothing 1", "", mode == 1))
			{
				mode = 1;
			}
			if (ImGui::MenuItem("Do nothing 2", "", mode == 2))
			{
				mode = 2;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene::BuildTransforms()
{

	// @@ When you are ready to try interactive viewing, replace the
	// following hard coded values for WorldProj and WorldView with
	// transformation matrices calculated from variables such as spin,
	// tilt, tr, ry, front, and back.

	if (transformation_mode)
	{
		WorldView = Rotate(0, tilt - 90) * Rotate(2, spin) * Translate(-eye.x, -eye.y, -eye.z);
	}
	else
	{
		WorldView = Translate(tx, ty, -zoom) * Rotate(0, tilt - 90) * Rotate(2, spin);
	}
	WorldProj = Perspective(rx, ry, front, back);

	// @@ Print the two matrices (in column-major order) for
	// comparison with the project document.
	// std::cout << "WorldView: " << glm::to_string(WorldView) << std::endl;
	// std::cout << "WorldProj: " << glm::to_string(WorldProj) << std::endl;
}

////////////////////////////////////////////////////////////////////////
// Procedure DrawScene is called whenever the scene needs to be
// drawn. (Which is often: 30 to 60 times per second are the common
// goals.)
void Scene::DrawScene()
{
	// Recalculate position of eye
	static double last_time = glfwGetTime();
	double current_time = glfwGetTime();
	double time_since_last_refresh = current_time - last_time;
	last_time = current_time;
	const double step = speed * time_since_last_refresh;
	const float radians = spin * PI / 180.0;
	if (w_down)
	{
		eye += glm::vec3(step * sin(radians), step * cos(radians), 0.0);
	}
	if (s_down)
	{
		eye -= glm::vec3(step * sin(radians), step * cos(radians), 0.0);
	}
	if (d_down)
	{
		eye += glm::vec3(step * cos(radians), -step * sin(radians), 0.0);
	}
	if (a_down)
	{
		eye -= glm::vec3(step * cos(radians), -step * sin(radians), 0.0);
	}

	eye.z = proceduralground->HeightAt(eye.x, eye.y) + 2;

	// Set the viewport
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Recalculate rx
	rx = 0.4 * width / height;

	CHECKERROR;
	// Calculate the light's position from lightSpin, lightTilt, lightDist
	lightPos = glm::vec3(lightDist * cos(lightSpin * rad) * sin(lightTilt * rad),
		lightDist * sin(lightSpin * rad) * sin(lightTilt * rad),
		lightDist * cos(lightTilt * rad));

	// Update position of any continuously animating objects
	double atime = 360.0 * glfwGetTime() / 36;
	for (std::vector<Object*>::iterator m = animated.begin(); m < animated.end(); m++)
		(*m)->animTr = Rotate(2, atime);

	BuildTransforms();

	// The lighting algorithm needs the inverse of the WorldView matrix
	WorldInverse = glm::inverse(WorldView);

	////////////////////////////////////////////////////////////////////////////////
	// Anatomy of a pass:
	//   Choose a shader  (create the shader in InitializeScene above)
	//   Choose and FBO/Render-Target (if needed; create the FBO in InitializeScene above)
	//   Set the viewport (to the pixel size of the screen or FBO)
	//   Clear the screen.
	//   Set the uniform variables required by the shader
	//   Draw the geometry
	//   Unset the FBO (if one was used)
	//   Unset the shader
	////////////////////////////////////////////////////////////////////////////////

	CHECKERROR;
	int loc, programId;

	////////////////////////////////////////////////////////////////////////////////
	// Lighting pass
	////////////////////////////////////////////////////////////////////////////////

	// Choose the lighting shader
	lightingProgram->UseShader();
	programId = lightingProgram->programId;

	// Set the viewport, and clear the screen
	glViewport(0, 0, width, height);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// @@ The scene specific parameters (uniform variables) used by
	// the shader are set here.  Object specific parameters are set in
	// the Draw procedure in object.cpp

	loc = glGetUniformLocation(programId, "WorldProj");
	glUniformMatrix4fv(loc, 1, GL_FALSE, Pntr(WorldProj));
	loc = glGetUniformLocation(programId, "WorldView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, Pntr(WorldView));
	loc = glGetUniformLocation(programId, "WorldInverse");
	glUniformMatrix4fv(loc, 1, GL_FALSE, Pntr(WorldInverse));
	loc = glGetUniformLocation(programId, "lightPos");
	glUniform3fv(loc, 1, &(lightPos[0]));
	loc = glGetUniformLocation(programId, "mode");
	glUniform1i(loc, mode);
	CHECKERROR;

	// Draw all objects (This recursively traverses the object hierarchy.)
	CHECKERROR;
	objectRoot->Draw(lightingProgram, Identity);
	CHECKERROR;

	// Turn off the shader
	lightingProgram->UnuseShader();

	////////////////////////////////////////////////////////////////////////////////
	// End of Lighting pass
	////////////////////////////////////////////////////////////////////////////////
}
