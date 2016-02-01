#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
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

osg::ref_ptr<osg::Node> createQuad()
{
	//叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	
	//几何体对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//顶点
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(0.0, 0.0, 0.0));
	v->push_back(osg::Vec3(1.0, 0.0, 0.0));
	v->push_back(osg::Vec3(1.0, 0.0, 1.0));
	v->push_back(osg::Vec3(0.0, 0.0, 1.0));
	geom->setVertexArray(v.get());

	//纹理
	//osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	//vt->push_back(osg::Vec2(0.0, 0.0));
	//vt->push_back(osg::Vec2(1.0, 0.0));
	//vt->push_back(osg::Vec2(1.0, 1.0));
	//vt->push_back(osg::Vec2(0.0, 1.0));
	//geom->setTexCoordArray(0, vt.get());

	//颜色
	osg::ref_ptr<osg::Vec4Array> vc = new osg::Vec4Array();
	vc->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	vc->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	vc->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	vc->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
	geom->setColorArray(vc.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//nc->push_back(osg::Vec3(0.0, -1.0, 0.0));
	//geom->setNormalArray(nc.get());
	//geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	
	geode->addDrawable(geom.get());

	return geode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(createQuad());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->realize();

	viewer->run();

	return 1;
}