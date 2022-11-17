#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <iostream>
#include <string>
#include <ctime>

#include "constants.h"

class ConstSpeedFallingCallback : public osg::NodeCallback {
protected:
	osg::Vec3f position = osg::Vec3f(0, 0, 0);
	osg::Vec3f velocity = osg::Vec3f(0, 0, zSpeed);
	osg::Vec3f acceleration = osg::Vec3f(0, 0, 0);
	clock_t last_t = clock();
	clock_t collisionTime = clock();
	bool isOnGround = false;

public:
	ConstSpeedFallingCallback() {};
	void operator()(osg::Node* node, osg::NodeVisitor* nv) {

		osg::PositionAttitudeTransform* pat = static_cast<osg::PositionAttitudeTransform*>(node);

		if (isOnGround) {
			if ((float)(clock() - collisionTime) / 1000 > timeOnGround) {
				isOnGround = false;
			}
		}
		else {
			osg::Vec3f position = pat->getPosition();

			clock_t current_t = clock();
			float delta = (current_t - last_t);
			delta /= 1000.0f;

			velocity += acceleration * delta;
			position += velocity * delta;


			// RESPAWN AT TOP (recycle snowflakes)
			if (position.z() <= minHeight) {
				position.z() = maxHeight;
			}

			// GROUND COLLISION
			if (position.z() <= snowflakeSize + groundsize[2] && collideWithGround) {
				isOnGround = true;
				collisionTime = clock();
			}

			pat->setPosition(position);

			last_t = current_t;
		}

		
	};
};
