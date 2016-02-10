#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

//FUNCTION: detect the memory leak in DEBUG mode
void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 17864;
#endif
}

int main(int argc, char** argv)
{
	installMemoryLeakDetector();
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/cow.osg");
	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform;
	pat1->setPosition(osg::Vec3(-10.0, 0.0, 0.0));
	pat1->setScale(osg::Vec3(0.5, 0.5, 0.5));
	pat1->addChild(node.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform;
	pat2->setPosition(osg::Vec3(10.0, 0.0, 0.0));
	pat2->addChild(node.get());

	root->addChild(pat1.get());
	root->addChild(pat2.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}
