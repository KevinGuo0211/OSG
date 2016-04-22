#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/FireEffect>
#include <osgUtil/Optimizer>
#include <iostream>

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> createExplosion()
{
	osg::ref_ptr<osg::Group> Explosion = new osg::Group;

	osg::Vec3 Wind(1.0, .0, 0.0);
	osg::Vec3 Position(0.0, 0.0, -10.0);
	osg::ref_ptr<osgParticle::ExplosionEffect> ExploEffect = new osgParticle::ExplosionEffect(Position, 10.0);
	osg::ref_ptr<osgParticle::ExplosionDebrisEffect> ExploDebris = new osgParticle::ExplosionDebrisEffect(Position, 10.0);
	osg::ref_ptr<osgParticle::SmokeEffect> SmoEffect = new osgParticle::SmokeEffect(Position, 10.0);
	osg::ref_ptr<osgParticle::FireEffect> FireEffect = new osgParticle::FireEffect(Position, 10.0);

	ExploEffect->setWind(Wind);
	ExploDebris->setWind(Wind);
	SmoEffect->setWind(Wind);
	FireEffect->setWind(Wind);

	Explosion->addChild(ExploEffect);
 	Explosion->addChild(ExploDebris);
 	Explosion->addChild(SmoEffect);
 	Explosion->addChild(FireEffect);

	return Explosion.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/lz.osg");
	root->addChild(createExplosion());

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 0 ;
}