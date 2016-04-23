#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/StateSet>

#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include <osgUtil/Optimizer>

osg::ref_ptr<osg::Node> createQuadNode()
{
	osg::ref_ptr<osg::Geode> Geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> Geom = new osg::Geometry();

	//设置顶点
	osg::ref_ptr<osg::Vec3Array> vc= new osg::Vec3Array();
	vc->push_back(osg::Vec3(-1.0f,0.0f,-1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,-1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(-1.0f,0.0f,1.0f));
	Geom->setVertexArray(vc.get());

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));
	Geom->setTexCoordArray(0,vt.get());

	//添加图元
	Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	//绘制
	Geode->addDrawable(Geom.get());

	return Geode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> Viewer = new osgViewer::Viewer();
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits;
	pTraits->x = 100;
	pTraits->y = 100;
	pTraits->width = 1024;
	pTraits->height = 768;
	pTraits->windowName = "Hive Application";
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);

	osg::ref_ptr<osg::Camera> pCamera = Viewer->getCamera();
	pCamera->setGraphicsContext(pGraphicsContext);
	osgGA::CameraManipulator* pCameraManipulator = new osgGA::TrackballManipulator();
	Viewer->setCameraManipulator(pCameraManipulator);
//	pCameraManipulator->setByMatrix(osg::Matrix::identity());
// 	pCameraManipulator->setAutoComputeHomePosition(false);
	pCamera->setClearColor(osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	pCamera->setViewport(0, 0, 1024, 768);
	pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pCamera->setViewMatrix(osg::Matrix::identity());
	pCamera->setProjectionMatrix(osg::Matrix::identity());
	pCameraManipulator->updateCamera(*pCamera);

	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = createQuadNode();

	osg::ref_ptr<osg::Texture2D> t2d = new osg::Texture2D;
	osg::ref_ptr<osg::Image>image=osgDB::readImageFile("../OSGData/Images/blueFlowers.png");
	t2d->setImage(0,image);

	node->getOrCreateStateSet()->setTextureAttributeAndModes(0, t2d,osg::StateAttribute::ON);
	node->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	root->addChild(node.get());

// 	osgUtil::Optimizer optimizer ;
// 	optimizer.optimize(root.get()) ;

	Viewer->setSceneData(root.get());

	Viewer->realize();
	//Viewer->run();
	while (!Viewer->done())
	{
		Viewer->frame();
	}

	return 0 ;
}