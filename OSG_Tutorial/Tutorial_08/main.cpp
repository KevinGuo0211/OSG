#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/MatrixTransform>
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
	
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform;
	osg::Matrix m;
	//m.translate(10.0, 0.0, 0.0);
	m.makeTranslate(osg::Vec3(10.0, 0.0, 0.0));
//	m.makeRotate(45.0, 1.0, 0.0, 0.0);
	mt1->setMatrix(m);
	mt1->addChild(node.get());

	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform;
	osg::Matrix t;

//	t.makeRotate(45.0, 0.0, 1.0, 0.0);	
	t.makeTranslate(osg::Vec3(-10.0, 0.0, 0.0));
	mt2->setMatrix(t);
	mt2->addChild(node.get());

	root->addChild(mt1.get());
	root->addChild(mt2.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}
