#pragma once
#include "pch.h"
#include "PlayerAvatar.h"
#include "RoamingCamera.h"
#include "PickupObject.h"

class MyApp : public ApplicationContext, public InputListener
{
public:
	Root* root_;
	SceneManager* scene_manager_;
	//SceneNode* camera_node_;
	RoamingCamera* roaming_camera_;
	PlayerAvatar* player_;
	PickupObject* cube_;
	PickupObject* wall_;
	MyApp();
	virtual ~MyApp() {}

	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void setupManager();
	void setupCamera();
	void populateScene();
	bool frameStarted(const Ogre::FrameEvent & evt);
};
