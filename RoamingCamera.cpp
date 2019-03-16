#include "pch.h"
#include "RoamingCamera.h"


RoamingCamera::RoamingCamera(SceneManager* scene_manager, RenderWindow* render_window, Vector3 position, Vector3 lookat_position )
{
	// Creating and attaching the camera
	camera_node_ = scene_manager->getRootSceneNode()->createChildSceneNode();
	camera_ = scene_manager->createCamera("RoamingCamera");
	camera_node_->attachObject(camera_);
	camera_node_->setPosition(position);
	camera_node_->lookAt(lookat_position, Ogre::Node::TransformSpace::TS_WORLD);

	// Configuring the camera
	camera_->setNearClipDistance(5);
	Ogre::Viewport* vp = render_window->addViewport(camera_);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Setting motion parameters
	movement_speed_ = 20.0f;
	rotation_speed_ = 3.0f;

}

RoamingCamera::~RoamingCamera()
{
}

void RoamingCamera::update(Ogre::Real delta_time, const Uint8 * keyboard_state)
{
	int x = 0, y = 0;

	// Leave if right mouse button is not being pressed
	// ...but also retrieve and store mouse movement
	if (!(SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON_LMASK)) return;

	// Construct displacement vector
	Ogre::Vector3 vec = Ogre::Vector3(0, 0, 0);
	if (keyboard_state[SDL_SCANCODE_W]) {
		vec = Ogre::Vector3(0, 0, -1);
	}
	if (keyboard_state[SDL_SCANCODE_S]) {
		vec += Ogre::Vector3(0, 0, 1);
	}
	if (keyboard_state[SDL_SCANCODE_A]) {
		vec += Ogre::Vector3(-1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_D]) {
		vec += Ogre::Vector3(1, 0, 0);
	}
	if (keyboard_state[SDL_SCANCODE_Q]) {
		vec += Ogre::Vector3(0, 1, 0);
	}
	if (keyboard_state[SDL_SCANCODE_E]) {
		vec += Ogre::Vector3(0, -1, 0);
	}

	// Construct view rotation
	float rotX = x * delta_time * -1 * rotation_speed_;
	float rotY = y * delta_time * -1 * rotation_speed_;

	// Update camera with new displacement and rotation
	camera_->yaw(Ogre::Radian(rotX));
	camera_->pitch(Ogre::Radian(rotY));
	camera_->moveRelative(delta_time * movement_speed_ * vec);
}