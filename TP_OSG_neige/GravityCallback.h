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

class GravityCallback : public osg::NodeCallback {
protected:
	osg::Vec3f position = osg::Vec3f(0, 0, 0);
	osg::Vec3f velocity = osg::Vec3f(0, 0,	0);
	osg::Vec3f acceleration = osg::Vec3f(0, 0, zGravity/4);
	clock_t last_t = clock();

public:
	GravityCallback() {};
	void operator()(osg::Node* node, osg::NodeVisitor* nv) {

		osg::PositionAttitudeTransform* pat = static_cast<osg::PositionAttitudeTransform*>(node);
		
		osg::Vec3f position = pat->getPosition();

		clock_t current_t = clock();
		float delta = (current_t - last_t);
		delta /= 1000.0f;

		velocity += acceleration * delta;
		position += velocity * delta;

		if (position.z() <= minHeight) {
			position.z() = maxHeight;
			velocity = { 0, 0, 0 };
		}

		if (position.z() <= snowflakeSize + groundsize[2] && collideWithGround) {
			velocity = { 0, 0, 0 };
			acceleration = { 0, 0, 0 };
		}

		pat->setPosition(position);

		last_t = current_t;
	};
};
