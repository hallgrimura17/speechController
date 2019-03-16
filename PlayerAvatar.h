#pragma once
#include "pch.h"
#include "SpeechController.h"
using namespace Ogre;
using namespace OgreBites;

class PlayerAvatar
{
public:
	Entity* entity_;
	Entity* sword1_;
	Entity* sword2_;
	SceneNode* sword1_node_;
	SceneNode* sword2_node_;
	ParticleSystem* partSystem;
	ParticleSystem* partSystem2;
	bool weaponsDrawn;

	SceneNode* entity_node_;
	SceneManager* scene_manager_;
	int walking_speed_;
	AnimationState* animation_state_top_;
	AnimationState* animation_state_base_;
	AnimationState* animation_state_hand_;
	AnimationState* animation_state_closed_;
	float rotation_ = 0.0;
	float rotation_speed_ = 5.0f;
	PlayerAvatar(SceneManager* scene_manager, String mesh_file_name);
	virtual ~PlayerAvatar() {}
	void update(Ogre::Real delta_time, const Uint8* state);
	void Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time);
	Ogre::Radian GetRotation(const Ogre::Vector3 & vec);
	void SetIdleAnimationLoop();
	void SetWalkingAnimationLoop();
	void StopAnimationLoop(void) const;
	void StartAnimatonLoop(void) const;
	void changeWeaponStatus();
	SceneNode* getSceneNode();
	void recognizeSpeech();
};
