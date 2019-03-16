#include "pch.h"
#include "PickupObject.h"


PickupObject::PickupObject(SceneManager* scene_manager, const char* mesh_file_name, Vector3 position , Vector3 scale ) 
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);

	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);

	pickup_effect_ = nullptr;
	picked_up_ = false;
}


PickupObject::~PickupObject()
{
	if (pickup_effect_ != nullptr) delete(pickup_effect_);
}

SceneNode* PickupObject::getSceneNode() const
{
	return  entity_node_;
}

bool PickupObject::collidesWith(SceneNode* other_node, float distance) {

	Ogre::Vector3 pos1 = entity_node_->getPosition();
	if (pos1.squaredDistance(other_node->getPosition()) < distance * distance)
	{
		return true;
	}
	return false;

}

void PickupObject::runPickupEffect()
{
	// TODO: Instantiate and run the effect here (try velocity (3.0, -0.5, 24.0)
	pickup_effect_ = new SwirlEffect(getSceneNode(), Ogre::Vector3(3.0, -0.5, 24.0));
	scene_manager_->getRootSceneNode()->removeChild(getSceneNode());
	getSceneNode()->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
	scene_manager_->getRootSceneNode()->getChild("MyPlayer")->addChild(getSceneNode());
	entity_->setMaterialName("OgreMaterial2");
	entity_node_->setScale(Vector3(0.5, 0.5, 0.5));
	pickup_effect_->run();
	picked_up_ = true;
	
}

void PickupObject::newPickupEffect()
{
	if (pickup_effect_ != nullptr)
	{
		scene_manager_->getRootSceneNode()->getChild("MyPlayer")->removeChild(getSceneNode());
		scene_manager_->getRootSceneNode()->addChild(getSceneNode());
		getSceneNode()->setPosition(Vector3((double)Math::RangeRandom(-19, 19), 0.0, (double)Math::RangeRandom(-19, 19)));
		delete pickup_effect_;
		pickup_effect_ = NULL;
		picked_up_ = false;
		entity_node_->setScale(Vector3(1, 1, 1));
		entity_->setMaterialName("OgreMaterial");

	}

}

bool PickupObject::isPickedUp() {
	return picked_up_;
}

void PickupObject::update(float delta_time) const
{
	if (pickup_effect_ != nullptr) {
		if (pickup_effect_->isRunning())
		{
			pickup_effect_->update(delta_time);
		}
	} 
}

const Vector3 PickupObject::getPosition() const
{
	return entity_node_->getPosition();
}
void PickupObject::setMaterialName(const char* material)
{
	entity_->setMaterialName(material);
}
