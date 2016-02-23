/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-21
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: øÌ∆¡±‰–Œ
*/

#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

int main()
{
	osg::ref_ptr<osgViewer::Viewer> OsgViewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> RootGroup = new osg::Group;
	osg::ref_ptr<osg::Node> CowNode = osgDB::readNodeFile("../OSGData/cow.osg");

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0; traits->y = 0;
	traits->width = 1024; traits->height = 768;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;	
	osg::ref_ptr<osg::GraphicsContext> graphicsContext = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (graphicsContext->valid())
	{
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;
		graphicsContext->setClearColor(osg::Vec4f(0.2f,0.2f,0.6f,1.0f));
		graphicsContext->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	double fovy, aspectRatio, zNear, zFar;
	OsgViewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		OsgViewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0);
	}

	osg::ref_ptr<osg::Camera> Camera = OsgViewer->getCamera();
	Camera->setViewport(0, 0 , traits->width, traits->height);
	Camera->setGraphicsContext(graphicsContext.get());

	RootGroup->addChild(CowNode);
	osgUtil::Optimizer Opti;
	Opti.optimize(RootGroup.get());
	OsgViewer->setSceneData(RootGroup.get());
	OsgViewer->realize();
	OsgViewer->run();
}