#pragma once
#include "pch.h"
#include "SwirlEffect.h"

using namespace Ogre;
using namespace OgreBites;

// PICKUP OBJECT
// A simple game object that can show a nice effect if being picked up
class PickupObject
{
public:

	// CONSTRUCTOR
	// scene_manager: Pointer to the main scene manager
	// mesh_file_name: The asset file name of the mesh that will represent this object visually
	// position: Where the object will be placed in world coordinates
	// scale: How much the size of the object is scaled in the x, y and z dimensions
	PickupObject(SceneManager* scene_manager, const char* mesh_file_name, Vector3 position = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1));
	~PickupObject();

	SceneNode* getSceneNode() const;	// Returns the scene node that holds this object
	void runPickupEffect();				// Display a special motion effect on the game object 
	void newPickupEffect();

	bool isPickedUp();					// True if the pickup effect has been run
	SwirlEffect* getPickupEffect() { return pickup_effect_; }
	bool collidesWith(SceneNode* other_node, float distance); // True if the passed node is within distance of this object
	void setMaterialName(const char* material);
	void update(float delta_time) const; // Updates object, including any running motion effects

	const Vector3 getPosition() const;

private:

	Entity* entity_;				// The game object model
	SceneNode* entity_node_;		// The scene node that holds the model
	SceneManager* scene_manager_;	// The main scene manager

	SwirlEffect* pickup_effect_; // An interesting pickup motion effect 

	bool picked_up_; // True if the pickup effect has been run
};

