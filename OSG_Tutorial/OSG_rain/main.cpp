#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Fog>
#include <osgDB/ReadFile>
#include <osgParticle/PrecipitationEffect>
#include <osgUtil/Optimizer>
#include <iostream>

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/lz.osg");
	root->addChild(node.get());

	osg::ref_ptr<osgParticle::PrecipitationEffect> PreEffect = new osgParticle::PrecipitationEffect;
	PreEffect->rain(0.1);
	root->addChild(PreEffect.get());

//	viewer->getCamera()->setClearColor(PreEffect->getFog()->getColor());
//	node->getOrCreateStateSet()->setAttributeAndModes(PreEffect->getFog());

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 0 ;
}