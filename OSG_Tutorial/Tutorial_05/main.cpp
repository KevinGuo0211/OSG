#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Switch>

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

osg::ref_ptr<osg::Node> drawLine()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array();
	vert->push_back(osg::Vec3(0.0, 0.0, 0.0));
	vert->push_back(osg::Vec3(1.0, 0.0, 0.0));
	vert->push_back(osg::Vec3(0.0, 1.0, 0.0));
	vert->push_back(osg::Vec3(0.0, 0.0, 1.0));
	geom->setVertexArray(vert.get());

	osg::ref_ptr<osg::DrawElementsUInt> quad = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	quad->push_back(0);
	quad->push_back(1);
	quad->push_back(0);
	quad->push_back(2);
	quad->push_back(0);
	quad->push_back(3);
	geom->addPrimitiveSet(quad.get());

	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
	color->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	geom->setColorArray(color.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::LineWidth> LineSize = new osg::LineWidth();
	LineSize->setWidth(5);
	geode->getOrCreateStateSet()->setAttributeAndModes(LineSize.get(), osg::StateAttribute::ON);

	geode->addDrawable(geom.get());
	return geode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Switch> sw = new osg::Switch();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../Model/bunny.ive");
	//root->addChild(node.get());
	//root->addChild(drawLine());
	sw->addChild(node.get());
	sw->addChild(drawLine(), false);
	root->addChild(sw);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}