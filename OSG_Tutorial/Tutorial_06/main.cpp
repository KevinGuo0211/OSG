#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
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

osg::ref_ptr<osg::Node> createBillboardTree(osg::ref_ptr<osg::Image> vImage)
{
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	v->push_back(osg::Vec3(-0.5, 0.0, .5));
	v->push_back(osg::Vec3(0.5, 0.0, -0.5));
	v->push_back(osg::Vec3(0.5, 0.0, 0.5));
	v->push_back(osg::Vec3(-0.5, 0.0, 0.5));
	geometry->setVertexArray(v.get());

	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array;
	normal->push_back(osg::Vec3(1.0, 0.0, 0.0)^osg::Vec3(0.0, 0.0, 1.0));
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array;
	vt->push_back(osg::Vec2(0.0, 0.0));
	vt->push_back(osg::Vec2(1.0, 0.0));
	vt->push_back(osg::Vec2(1.0, 1.0));
	vt->push_back(osg::Vec2(0.0, 1.0));
	geometry->setTexCoordArray(0, vt.get());

	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	if (vImage.get())
	{
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(vImage.get());
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

		//启用混合
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		//关闭光照
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

		geometry->setStateSet(stateset.get());
	}

	//创建Billboard对象
	osg::ref_ptr<osg::Billboard> billboard1 = new osg::Billboard;
	//设置旋转模式为绕视点
	billboard1->setMode(osg::Billboard::POINT_ROT_EYE);
	billboard1->addDrawable(geometry.get(), osg::Vec3(5.0, 0.0, 0.0));

	osg::ref_ptr<osg::Billboard> billboard2 = new osg::Billboard;
	billboard2->setMode(osg::Billboard::AXIAL_ROT);
	billboard2->setAxis(osg::Vec3(0.0, 0.0, 1.0));
	billboard2->addDrawable(geometry.get(), osg::Vec3(10.0, 0.0, 0.0));

	osg::ref_ptr<osg::Group> billboard = new osg::Group;
	billboard->addChild(billboard1.get());
	billboard->addChild(billboard2.get());

	return billboard.get();
}

int main(int argc, char** argv)
{
	installMemoryLeakDetector();

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../OSGData/Images/tree0.rgba");

	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->setScale(osg::Vec3f(5.0, 5.0, 5.0));
	pat->addChild(createBillboardTree(image.get()));
	
	root->addChild(pat.get());
	root->addChild(osgDB::readNodeFile("../OSGData/cow.osg"));

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}