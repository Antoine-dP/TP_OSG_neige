// TestOSG.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#ifdef _WIN32
#include <Windows.h>
#endif

// base
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/Fog>
#include <osg/Plane>
#include <osg/animationpath>

// Callbacks
#include "GravityCallback.h"
#include "ConstSpeedFallingCallback.h"


// Keyboard input
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>

#include <iostream>
#include <string>
#include <vector>

#include <ctime>

#include "constants.h"

osg::Vec3f velocity = osg::Vec3f( 0,0,0 );
osg::Vec3f acceleration = osg::Vec3f(0, 0, -9.81);

clock_t last_t = clock();


/// GLOBAL VARIABLES 



float snowFlakeDensity = 2.0f;
int numSnowflakes = (int)(snowFlakeDensity * (maxHeight-minHeight + timeOnGround * abs(zSpeed)) * groundsize[0] * groundsize[1]);

bool fallType_isConstSpeed = true;



int main()
{
	std::cout << "Hello World!\n";
	srand(time(0));
	//numSnowflakes = 200;
	//Creating the viewer	
	osgViewer::Viewer viewer;


	//Creating the root node
	osg::ref_ptr<osg::Group> root(new osg::Group);

	//The geode containing our shape
	osg::ref_ptr<osg::Geode> myshapegeode0(new osg::Geode);
	osg::ref_ptr<osg::Geode> geodeGround(new osg::Geode);
	osg::ref_ptr<osg::Geode> geodeSphere(new osg::Geode);



	//Our shape: a capsule, it could have been any other geometry (a box, plane, cylinder etc.)
	osg::ref_ptr<osg::Capsule> myCapsule(new osg::Capsule(osg::Vec3f(), 1, 2));
	osg::ref_ptr<osg::Box> myGround(new osg::Box(osg::Vec3f(0.0f, 0.0f, 0.0f), groundsize[0], groundsize[1], groundsize[2]));
	osg::ref_ptr<osg::Sphere> mySphere(new osg::Sphere(osg::Vec3f(0.0f, 0.0f, 0.0f), snowflakeSize));

	//mySphere->get;


	//Our shape drawable
	osg::ref_ptr<osg::ShapeDrawable> capsuledrawable(new osg::ShapeDrawable(myCapsule.get()));
	osg::ref_ptr<osg::ShapeDrawable> groundDrawable(new osg::ShapeDrawable(myGround.get()));
	osg::ref_ptr<osg::ShapeDrawable> sphereDrawable(new osg::ShapeDrawable(mySphere.get()));



	///* Importation */
	// Create transformation node
	osg::ref_ptr<osg::MatrixTransform> cowScaleMAT(new osg::MatrixTransform);
	// Scale matrix
	osg::Matrix cowScaleMatrix;
	cowScaleMatrix.makeScale(osg::Vec3f(0.1f, 0.1f, 0.1f));
	//Loading the terrain node
	//const std::string filename = "C:\\Users\\Eleve\\Documents\\TP\\ImagRV\\OpenSceneGraph\\OSG_data\\OSG_data\\Terrain2.3ds";
	const std::string filename = "cow.osg";
	osg::ref_ptr<osg::Node> cownode(osgDB::readNodeFile(filename));
	//Set transformation node parameters
	cowScaleMAT->addChild(cownode.get());
	cowScaleMAT->setMatrix(cowScaleMatrix);


/// PAT
	std::vector<osg::ref_ptr<osg::PositionAttitudeTransform>> patList;
	std::vector<osg::ref_ptr<osg::PositionAttitudeTransform>> patAnimList;
	std::vector<osg::Vec3f> posTransList;
	
	for (int i = 0; i < numSnowflakes; i++)
	{
		osg::ref_ptr<osg::PositionAttitudeTransform> PAT_Sphere_I(new osg::PositionAttitudeTransform);
		float x = (float)(rand() % 1000) / 1000;
		float y = (float)(rand() % 1000) / 1000;
		float z = (float)(rand() % 1000) / 1000;
		osg::Vec3f spherePosTrans_I = osg::Vec3f((x - 0.5)* groundsize[0], (y - 0.5) * groundsize[1], z * (maxHeight-minHeight+timeOnGround*abs(zSpeed)));
		patList.push_back(PAT_Sphere_I);
		posTransList.push_back(spherePosTrans_I);
	}
	for (int i = 0; i < numSnowflakes; i++)
	{
		osg::ref_ptr<osg::PositionAttitudeTransform> PAT_Anim_Sphere_I(new osg::PositionAttitudeTransform);
		patAnimList.push_back(PAT_Anim_Sphere_I);
	}

	osg::ref_ptr<osg::PositionAttitudeTransform> PAT_0(new osg::PositionAttitudeTransform);
	osg::Vec3f objectPosTrans0 = osg::Vec3f(0, 0, 3);
	osg::ref_ptr<osg::PositionAttitudeTransform> PAT_Ground(new osg::PositionAttitudeTransform);
	osg::Vec3f groundPosTrans = osg::Vec3f(0, 0, 0);
	osg::ref_ptr<osg::PositionAttitudeTransform> PAT_Sphere1(new osg::PositionAttitudeTransform);
	osg::Vec3f spherePosTrans1 = osg::Vec3f(0, 0, 2);



	// Set color of the shape
	capsuledrawable->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.0));

	std::vector<osg::ref_ptr<osg::StateSet>> stateSetList;
	for (int i = 0; i < patList.size(); i++)
	{
		osg::ref_ptr<osg::StateSet> nodeStateSet_I(patList[i]->getOrCreateStateSet());

	}
	osg::ref_ptr<osg::StateSet> nodeStateSet0(myshapegeode0->getOrCreateStateSet());



	// Création d'un Animation Path 
	osg::ref_ptr<osg::AnimationPath> spherePath = new osg::AnimationPath;
	// Définition du mode de boucle 
	spherePath->setLoopMode(osg::AnimationPath::LOOP);
	// Création de point de contrôle
	osg::AnimationPath::ControlPoint c0(osg::Vec3(-0.5, -0.5, 0));
	osg::AnimationPath::ControlPoint c1(osg::Vec3(-0.5, 0.5, 0));
	osg::AnimationPath::ControlPoint c2(osg::Vec3(0.5, 0.5, 0));
	osg::AnimationPath::ControlPoint c3(osg::Vec3(0.5, -0.5, 0));
	spherePath->insert(0.0f, c0);
	spherePath->insert(1.0f, c1);
	spherePath->insert(2.0f, c2);
	spherePath->insert(3.0f, c3);
	spherePath->insert(4.0f, c0);

	

	// FOR CAPSULE
	////Loading texture image object
	//osg::ref_ptr<osg::Image> image2(osgDB::readImageFile("C:\\Users\\Eleve\\Documents\\TP\\ImagRV\\OpenSceneGraph\\OSG_data\\OSG_data\\wood.png"));
	////Bind the image to a 2D texture object
	//osg::ref_ptr<osg::Texture2D> tex2(new osg::Texture2D());
	//tex2->setImage(image2);
	////Applying texture on the object
	//nodeStateSet0->setTextureAttributeAndModes(0, tex2);

	/* LIGHTING */
	//Create light source
	//osg::ref_ptr<osg::LightSource> lightSource1 = new osg::LightSource;
	////Create a local light
	//osg::ref_ptr<osg::Light> myLight = new osg::Light;
	//myLight->setLightNum(1);
	//osg::Vec4f lightPosition(0.0f, 0.0f, 0.0f, 1.0f); // light point
	//myLight->setPosition(lightPosition);
	////myLight->setAmbient(osg::Vec4f(1.0f, 1.0f, 1.0f, 0.0f));
	//myLight->setDiffuse(osg::Vec4f(1.0f, 0.0f, 1.0f, 1.0f));
	//myLight->setConstantAttenuation(1.0f);
	////Attach the light to the light source
	//lightSource1->setLight(myLight);
	////Position the lightSource into the scene graph
	//lightPAT->addChild(lightSource1);
	//// light will applied on following node
	//// (mainly root node to illuminate all the scene)
	//osg::ref_ptr<osg::StateSet> rootSS((root->getOrCreateStateSet()));
	//// activate light on choosen node
	//rootSS->setAttributeAndModes(myLight, osg::StateAttribute::ON);



	//// FOG
	//myFog->setColor(osg::Vec4(1.0f, 0.5f, 0.5f, 1.0f));
	//myFog->setStart(0.01f);
	//myFog->setEnd(10000.0f);
	//myFog->setDensity(0.01f);
	//rootSS2->setAttributeAndModes(myFog, osg::StateAttribute::ON);


	/* SCENE GRAPH */

	// Add the shape drawable to the geode
	myshapegeode0->addDrawable(capsuledrawable.get());
	geodeGround->addDrawable(groundDrawable);
	geodeSphere->addDrawable(sphereDrawable);


	// Add ground to node
	

	// Add geode to PAT
	PAT_0->addChild(myshapegeode0.get());
	PAT_0->setPosition(objectPosTrans0);
	PAT_Ground->addChild(geodeGround.get());
	PAT_Ground->setPosition(groundPosTrans);
	PAT_Sphere1->addChild(geodeSphere.get());
	PAT_Sphere1->setPosition(spherePosTrans1);

	for (int i = 0; i < patList.size(); i++)
	{
		patList[i]->addChild(geodeSphere.get());
		patList[i]->setPosition(posTransList[i]);
	}
	for (int i = 0; i < patList.size(); i++)
	{
		patAnimList[i]->addChild(patList[i]);
		osg::Vec3f spherePosTrans_I = osg::Vec3f(0, 0, 0);
		patAnimList[i]->setPosition(spherePosTrans_I);
	}


	// Add the geode to the scene graph root (Group)
	//root->addChild(PAT_0.get());
	root->addChild(PAT_Ground.get());
	//root->addChild(PAT_Sphere1.get());

	for (int i = 0; i < patList.size(); i++)
	{
		root->addChild(patAnimList[i].get());
	}



	// Set the scene data
	viewer.setSceneData(root.get());


	//Stats Event Handler s key
	viewer.addEventHandler(new osgViewer::StatsHandler);

	//Windows size handler
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// add the state manipulator
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));



/// Material
	for (int i = 0; i < patList.size(); i++)
	{
		osg::ref_ptr<osg::Material> material(new osg::Material());
		material->setDiffuse(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 0.0));
		material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 1.0, 0.0, 0.0));
		//stateSetList[i]->setAttribute(material);
	}
	osg::ref_ptr<osg::Material> material(new osg::Material());
	material->setDiffuse(osg::Material::FRONT, osg::Vec4(1.0, 1.0, 1.0, 0.0));
	material->setSpecular(osg::Material::FRONT, osg::Vec4(0.0, 1.0, 0.0, 0.0));
	nodeStateSet0->setAttribute(material);


/// CALLBACKS

	osg::ref_ptr<osg::AnimationPathCallback> rAniCallback = new osg::AnimationPathCallback(spherePath.get());
	//osg::ref_ptr<osg::Callback> GravityCallback = new osg::Callback();
	//PAT_Sphere1->setUpdateCallback(rAniCallback.get());
	PAT_Sphere1->setUpdateCallback(new GravityCallback());

	if (fallType_isConstSpeed) {
		for (int i = 0; i < patList.size(); i++)
		{
			patList[i]->setUpdateCallback(new ConstSpeedFallingCallback());
			patAnimList[i]->setUpdateCallback(rAniCallback.get());
		}
	}
	else {
		for (int i = 0; i < patList.size(); i++)
		{
			patList[i]->setUpdateCallback(new GravityCallback());
		}
	}
	


	/* START VIEWER */

	// The viewer.run() method starts the threads and the traversals.
	return (viewer.run());
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

