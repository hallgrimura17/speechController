#include "pch.h"
#include "MyApp.h"

using namespace Ogre;
using namespace OgreBites;

MyApp::MyApp() : ApplicationContext("SC-T-637-GEDE Lab 7")
{
}

void MyApp::setup(void)
{
	ApplicationContext::setup();
	addInputListener(this);
	setupManager();
	setupCamera();
	populateScene();
}
//stop on ESC press
bool MyApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	if (evt.keysym.sym == SDLK_e)
	{
		player_->changeWeaponStatus();
	}
	if (evt.keysym.sym == SDLK_t)
	{
		player_->recognizeSpeech();
	}
	return true;
}
void MyApp::setupManager()
{
	// Get pointers to root and create scene manager
	root_ = getRoot();
	scene_manager_ = root_->createSceneManager();

	// Register scene with the RTSS
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(scene_manager_);
}
void MyApp::setupCamera()
{
	roaming_camera_ = new RoamingCamera(scene_manager_, getRenderWindow(), Vector3(0, 0, 50));
	// Add camera
	/*camera_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	camera_ = scene_manager_->createCamera("MainCamera");
	camera_->setNearClipDistance(1);
	camera_->setAutoAspectRatio(true);
	camera_node_->setPosition(0, 0, 50);
	camera_node_->setAutoTracking(true, player_->entity_node_);
	//camera_node_->lookAt(Ogre::Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
	camera_node_->attachObject(camera_);

	Viewport* vp = getRenderWindow()->addViewport(camera_);
	vp->setBackgroundColour(ColourValue((float)0.5, (float)0.1, (float)0.5));
	camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));*/
}
void MyApp::populateScene()
{
	player_ = new PlayerAvatar(scene_manager_, "Sinbad.mesh");
	cube_ = new PickupObject(scene_manager_, "OgreBox.mesh", Vector3(10.0, 0.0, 20.0));
	wall_ = new PickupObject(scene_manager_, "OgreBox.mesh", Vector3(10.0, 0.0, -60.0), Vector3(100, 50, 1));
	wall_->setMaterialName("OgreMaterial3");
	// Add Ground
	Plane plane(Ogre::Vector3::UNIT_Y, -5);
	MeshManager::getSingleton()
		.createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 500, 500, 1, 1, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);
	Entity* groundEntity = scene_manager_->createEntity("LightPlaneEntity", "MainGround");
	scene_manager_->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/BeachStones");
	//groundEntity->setMaterialName("shader/orange");
	Ogre::Plane water(Ogre::Vector3::UNIT_Y, 0.0f);
	Ogre::MeshManager::getSingleton().createPlane("AnimatedWater", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, water, 30, 30, 40, 40, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* waterEnt = scene_manager_->createEntity("waterEntity", "AnimatedWater");
	waterEnt->setMaterialName("shader/texture");
	scene_manager_->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 0, -40))->attachObject(waterEnt);
	// Set Shadow Technique
	scene_manager_->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
	// Add Directional Light
	scene_manager_->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
	Light* directionalLight = scene_manager_->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDirection(Ogre::Vector3(0, -1, -1));
	SceneNode* directionalLightNode = scene_manager_->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	// Create a particle system
	ParticleSystem* partSystem = scene_manager_->createParticleSystem("smoke", "MySmoke1");
	// Attach the particle system to Cube
	cube_->getSceneNode()->attachObject(partSystem);

}
bool MyApp::frameStarted(const Ogre::FrameEvent &evt)
{
	// Main "game loop" of the application
	// Let parent handle this callback as well
	ApplicationContext::frameStarted(evt);
	// Store the time that has passed since last time we got the callback
	const Ogre::Real delta_time = evt.timeSinceLastFrame;
	// Check what keys of the keyboard are being pressed
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	// Check for collision with a game object that has not yet been picked up
	if (!cube_->isPickedUp() && cube_->collidesWith(player_->getSceneNode(), 6.0))
	{
		cube_->runPickupEffect();
	}
	if (cube_->isPickedUp() && cube_->getPickupEffect()->isFinished())
	{
		cube_->newPickupEffect();
	}

	// Update any subsystems 
	if (player_ != nullptr) player_->update(delta_time, state);
	if (cube_ != nullptr) cube_->update(delta_time);
	if (roaming_camera_ != nullptr) roaming_camera_->update(delta_time, state);
	return true;
}
