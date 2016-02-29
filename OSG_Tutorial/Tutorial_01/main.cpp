#include <iostream>
#include <string>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
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
	//_crtBreakAlloc = 26175;
#endif
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/cow.osg");
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("../OSGData/lz.osg");

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(node.get());

	osg::ref_ptr<osg::Group> root2 = new osg::Group;
	root2->addChild(node2.get());

	root = root2;

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

	return 1;
}