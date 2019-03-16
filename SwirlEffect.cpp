#include "pch.h"
#include "SwirlEffect.h"

// CONSTRUCTOR
// scene_nnode: The node that gets moved
// cylindrical_velocity_vector: Defines the motion in cylindrical coordinates
//     in the form (up speed, out speed, angular speed)
SwirlEffect::SwirlEffect(SceneNode* scene_node, Ogre::Vector3 cylindrical_velocity_vector)
{
	scene_node_ = scene_node;
	cylindrical_velocity_vector_ = cylindrical_velocity_vector;
	center_pos_ = scene_node_->getPosition();
	start_cylindrical_pos_ = Ogre::Vector3(0.0, 3.0, 0.0);
	end_cylindrical_pos_ = start_cylindrical_pos_ + Ogre::Vector3(8.0, 0.0, 0.0);
}

// Gets the current position relative to the original node position
Ogre::Vector3 SwirlEffect::getCylindrical() const	// "Native" cylindrical coordinates (x=height, y=radius, z=angle)
{
	return current_cylindrical_pos_;
	/*float radiusSquared = center_pos_.x * center_pos_.x + center_pos_.z * center_pos_.z;
	float height = center_pos_.y;
	float angle = 0;
	return Ogre::Vector3(height, sqrt(radiusSquared), angle);*/
}
Ogre::Vector3 SwirlEffect::getCartesian() const		// Converted to Cartesian coordinates (x, y, z)
{
	return Ogre::Vector3(cos((float)current_cylindrical_pos_.z)*current_cylindrical_pos_.y, current_cylindrical_pos_.x, sin((float)current_cylindrical_pos_.z)*current_cylindrical_pos_.y);
	/*X coordinate = cos(angle) * radius
	Y coordinate = height
	Z Coordinate = sin(angle) * radius*/
}
void SwirlEffect::run()					// Starts the motion along the trajectory
{
	center_pos_ = scene_node_->getPosition();
	current_cylindrical_pos_ = Ogre::Vector3(0.0, 3.0, 0.0);
	effect_finished_ = false;
	effect_running_ = true;
}
void SwirlEffect::update(float delta_time) {
	if (effect_running_)
	{
		if (current_cylindrical_pos_.x >= end_cylindrical_pos_.x)
		{
			// HACK: Node is translated below ground to make it (and any object attached) dissapear!
			effect_running_ = false;
			effect_finished_ = true;
		}
		else
		{
			current_cylindrical_pos_ = current_cylindrical_pos_ + delta_time * cylindrical_velocity_vector_;
			scene_node_->setPosition(getCartesian() + center_pos_);
		}
	}
}
bool SwirlEffect::isRunning()				// True if motion is ongoing
{
	return effect_running_;
}
bool SwirlEffect::isFinished()				// False if motion has run its course
{
	return effect_finished_;
}