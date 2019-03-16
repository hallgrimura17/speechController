#include "pch.h"
#include "PlayerAvatar.h"

PlayerAvatar::PlayerAvatar(SceneManager* scene_manager, String mesh_file_name)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_->setCastShadows(true);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode("MyPlayer");
	entity_node_->attachObject(entity_);
	walking_speed_ = 50;
	animation_state_base_ = NULL;
	animation_state_top_ = NULL;
	animation_state_hand_ = entity_->getAnimationState("HandsRelaxed");
	animation_state_closed_ = entity_->getAnimationState("HandsClosed");
	rotation_ = 0.0;
	rotation_speed_ = 5.0f;
	entity_->getSubEntity(1)->setMaterialName("Sinbad/Body2");
	entity_->getSubEntity(0)->setCustomParameter(1, Ogre::Vector4(0.0, 0.0, 1.0, 1.0));
	entity_->setMaterialName("shader/shader");
	// Create a bone iterator.
	Ogre::Skeleton::BoneIterator biter = entity_->getSkeleton()->getBoneIterator();
	// Iterate over the available bones and write their names to the console.

	sword1_ = scene_manager_->createEntity("Sword.mesh");
	sword2_ = scene_manager_->createEntity("Sword.mesh");
	partSystem = scene_manager_->createParticleSystem("sword", "MySword");
	partSystem2 = scene_manager_->createParticleSystem("sword2", "MySword");
	entity_->attachObjectToBone("Handle.R", partSystem);
	entity_->attachObjectToBone("Handle.L", partSystem2);
	entity_->attachObjectToBone("Handle.R", sword1_);
	entity_->attachObjectToBone("Handle.L", sword2_);
	ParticleSystem* partSystem3 = scene_manager_->createParticleSystem("fireworks", "Fireworks");
	// Attach the particle system to Cube
	entity_node_->attachObject(partSystem3);
	weaponsDrawn = true;
}
void PlayerAvatar::update(Ogre::Real delta_time, const Uint8* state)
{
	// Leave if the right mouse button is being pressed (different function)
	if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK)) return;
	// Construct the movement 
	Ogre::Vector3 translate_vector(0, 0, 0);
	Ogre::Vector3 dir(sin(rotation_), 0, cos(rotation_));

	if (state[SDL_SCANCODE_W]) {
		translate_vector = delta_time * walking_speed_ * dir;
	}
	if (state[SDL_SCANCODE_S]) {
		translate_vector = delta_time * walking_speed_ * -1 * dir;
	}
	if (state[SDL_SCANCODE_A]) {
		rotation_ += delta_time * rotation_speed_;
	}
	if (state[SDL_SCANCODE_D]) {
		rotation_ -= delta_time * rotation_speed_;
	}

	Move(translate_vector, rotation_, delta_time);

	if (!translate_vector.isZeroLength())
	{
		SetWalkingAnimationLoop();
	}
	else
	{
		SetIdleAnimationLoop();
	}
	animation_state_base_->addTime(delta_time);
	animation_state_top_->addTime(delta_time);
}
void PlayerAvatar::Move(Ogre::Vector3 translate_vector, float rotation, Ogre::Real delta_time)
{
	entity_node_->translate(translate_vector * delta_time * walking_speed_);
	entity_node_->resetOrientation();
	entity_node_->yaw(Ogre::Radian(rotation));
}

Ogre::Radian PlayerAvatar::GetRotation(const Ogre::Vector3& vec)
{
	Ogre::Vector3 v(vec);
	v.normalise();
	return((v.x <= 0 ? -1.0 : 1.0)* Ogre::Radian(acos(v.z)));
}
void PlayerAvatar::SetIdleAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimatonLoop();

	}
	// Stop other animation if there is any already running
	else if (animation_state_top_->getAnimationName() != "IdleTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("IdleBase");
		animation_state_top_ = entity_->getAnimationState("IdleTop");
		StartAnimatonLoop();
	}
}
void PlayerAvatar::SetWalkingAnimationLoop()
{
	if (animation_state_top_ == nullptr)
	{
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimatonLoop();
	}
	else if (animation_state_top_->getAnimationName() != "RunTop")
	{
		StopAnimationLoop();
		animation_state_base_ = entity_->getAnimationState("RunBase");
		animation_state_top_ = entity_->getAnimationState("RunTop");
		StartAnimatonLoop();
	}
}
void PlayerAvatar::StopAnimationLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		animation_state_base_->getEnabled())
	{
		animation_state_base_->setTimePosition(0.0f);
		animation_state_base_->setEnabled(false);
	}
	if (animation_state_top_ != nullptr &&
		animation_state_top_->getEnabled())
	{
		animation_state_top_->setTimePosition(0.0f);
		animation_state_top_->setEnabled(false);
	}
}
void PlayerAvatar::StartAnimatonLoop(void) const
{
	if (animation_state_base_ != nullptr &&
		!animation_state_base_->getEnabled())
	{
		animation_state_base_->setLoop(true);
		animation_state_base_->setEnabled(true);
	}
	if (animation_state_top_ != nullptr &&
		!animation_state_top_->getEnabled())
	{
		animation_state_top_->setLoop(true);
		animation_state_top_->setEnabled(true);
	}
}
SceneNode* PlayerAvatar::getSceneNode()
{
	return entity_node_;
}
void PlayerAvatar::changeWeaponStatus()
{
	if (!weaponsDrawn) {
		entity_->detachAllObjectsFromBone();
		entity_->attachObjectToBone("Handle.R", sword1_);
		entity_->attachObjectToBone("Handle.L", sword2_);
		entity_->attachObjectToBone("Handle.R", partSystem);
		entity_->attachObjectToBone("Handle.L", partSystem2);
		animation_state_hand_->setEnabled(false);
		animation_state_closed_->setEnabled(true);
		weaponsDrawn = true;
	}
	else if (weaponsDrawn) {
		entity_->detachAllObjectsFromBone();
		entity_->attachObjectToBone("Sheath.R", sword1_);
		entity_->attachObjectToBone("Sheath.L", sword2_);
		animation_state_hand_->setEnabled(true);
		animation_state_closed_->setEnabled(false);
		weaponsDrawn = false;
	}
}
void PlayerAvatar::recognizeSpeech()
{
	string str = SpeechController::wmain();
	cout << str << endl;
	if (str == "Use weapons.") {
		changeWeaponStatus();
	}
}