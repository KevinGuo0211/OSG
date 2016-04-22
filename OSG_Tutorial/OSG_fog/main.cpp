#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Fog>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <iostream>

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Fog> createFog(bool vLinear)
{
	osg::ref_ptr<osg::Fog> fog = new osg::Fog;
	fog->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	fog->setDensity(0.01f);

	if (!vLinear)
	{
		fog->setMode(osg::Fog::LINEAR);
	}
	else
	{
		fog->setMode(osg::Fog::EXP);
	}

	fog->setStart(5.0f);
	fog->setEnd(2000.0f);
	return fog.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/lz.osg");
	root->addChild(node.get());

	root->getOrCreateStateSet()->setAttributeAndModes(createFog(false), osg::StateAttribute::ON);
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 0 ;
}