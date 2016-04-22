#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#include <osgUtil/Optimizer>
#include <iostream>

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> createMyParticleSystem()
{
	osg::ref_ptr<osg::Group> MyParticleSystem = new osg::Group;

	osgParticle::Particle ParticleTemplte;
	ParticleTemplte.setLifeTime(2);
	ParticleTemplte.setAlphaRange(osgParticle::rangef(0.0, 1.0));
	ParticleTemplte.setSizeRange(osgParticle::rangef(0.75, 3.0));
	ParticleTemplte.setColorRange(osgParticle::rangev4(osg::Vec4(1.0, 0.5, 0.3, 1.0), osg::Vec4(0.0, 0.7, 1.0, 0.0)));
	ParticleTemplte.setRadius(0.05);
	ParticleTemplte.setMass(0.05);

	osg::ref_ptr<osgParticle::ParticleSystem> ParticleSys = new osgParticle::ParticleSystem();
	ParticleSys->setDefaultAttributes("../OSGData/Images/smoke.rgb", false, false);
	ParticleSys->setDefaultParticleTemplate(ParticleTemplte);

	osg::ref_ptr<osgParticle::ModularEmitter> Emitter = new osgParticle::ModularEmitter;
	Emitter->setParticleSystem(ParticleSys.get());

	osg::ref_ptr<osgParticle::RandomRateCounter> Counter = new osgParticle::RandomRateCounter;
	Counter->setRateRange(1.0, 10.0);
	Emitter->setCounter(Counter.get());

	osg::ref_ptr<osgParticle::PointPlacer> Placer = new osgParticle::PointPlacer;
	Placer->setCenter(osg::Vec3(0.0, 0.0, 0.0));
	Emitter->setPlacer(Placer.get());


	osg::ref_ptr<osgParticle::RadialShooter> Shooter = new osgParticle::RadialShooter;
	Shooter->setInitialSpeedRange(100, 0);
	Emitter->setShooter(Shooter.get());
	MyParticleSystem->addChild(Emitter.get());

	osg::ref_ptr<osgParticle::ModularProgram> Program = new osgParticle::ModularProgram;
	Program->setParticleSystem(ParticleSys);
	
	osg::ref_ptr<osgParticle::AccelOperator> AccelOper = new osgParticle::AccelOperator;
	AccelOper->setToGravity(-1.0);
	Program->addOperator(AccelOper.get());
	
	osg::ref_ptr<osgParticle::FluidFrictionOperator> FluidFrictionOper = new osgParticle::FluidFrictionOperator;
	FluidFrictionOper->setFluidToAir();
	Program->addOperator(FluidFrictionOper.get());
	MyParticleSystem->addChild(Program.get());

	osg::ref_ptr<osgParticle::ParticleSystemUpdater> ParticleSysUpdater = new osgParticle::ParticleSystemUpdater;
	ParticleSysUpdater->addParticleSystem(ParticleSys.get());

	osg::ref_ptr<osg::Geode> Geode = new osg::Geode;
	Geode->addDrawable(ParticleSys.get());

	MyParticleSystem->addChild(Geode.get());
	MyParticleSystem->addChild(ParticleSysUpdater.get());

	return MyParticleSystem.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(createMyParticleSystem());

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 0 ;
}