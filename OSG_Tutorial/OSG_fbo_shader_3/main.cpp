#include <osg/GL>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Viewport>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osg/FrameBufferObject>
#include <osgViewer/Viewer>

#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

osg::ref_ptr<osg::Node> createQuadNode();
osg::Program* createCopy2ScreenProgram();

int main()
{
	osgViewer::Viewer Viewer;
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits;
	pTraits->x = 100;
	pTraits->y = 100;
	pTraits->width = 1024;
	pTraits->height = 768;
	pTraits->windowName = "Hive Application";
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);
//	pGraphicsContext->getState()->setUseModelViewAndProjectionUniforms(true);   //activate osg specified shader uniforms, such as osg_ModelViewProjectionMatrix
//	pGraphicsContext->getState()->setUseVertexAttributeAliasing(true);			 //activate something like: layout (location = 0) in vec4 _VertexPosition;

	osg::ref_ptr<osg::Camera> pCamera = Viewer.getCamera();
	pCamera->setGraphicsContext(pGraphicsContext);	

	pCamera->setClearColor(osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	pCamera->setViewport(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);
	pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCamera->setViewMatrix(osg::Matrix::identity());
	pCamera->setProjectionMatrix(osg::Matrix::identity());
	
	osgGA::CameraManipulator* pCameraManipulator = new osgGA::TrackballManipulator();
	Viewer.setCameraManipulator(pCameraManipulator);	
	pCameraManipulator->updateCamera(*pCamera);

	osg::ref_ptr<osg::Group> Root = new osg::Group;

 	osg::ref_ptr<osg::Node> QuadNode = createQuadNode();
	osg::ref_ptr<osg::Texture2D> FlowersTex = new osg::Texture2D;
	//FlowersTex->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Image>image=osgDB::readImageFile("../OSGData/Images/blueFlowers.png");
	FlowersTex->setImage(0,image);

	QuadNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, FlowersTex.get(), osg::StateAttribute::ON);
  	QuadNode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	osg::Program *pCopy2ScreenProgram = createCopy2ScreenProgram();
// 	pCopy2ScreenProgram->addBindFragDataLocation("_VertexPosition", 0);  
// 	pCopy2ScreenProgram->addBindFragDataLocation("_VertexTexCoord0", 1);

	QuadNode->getOrCreateStateSet()->setAttribute(pCopy2ScreenProgram, osg::StateAttribute::ON);
	osg::Uniform* pUniform = new osg::Uniform("uColorBuf", 0);
	QuadNode->getOrCreateStateSet()->addUniform(pUniform);

	Root->addChild(QuadNode);
	Viewer.setSceneData(Root.get());
	Viewer.realize();
	Viewer.run();

	return 0;
}

//***********************************************************
//FUNCTION:
osg::Program* createCopy2ScreenProgram()
{
	osg::Shader *pVertexShader = new osg::Shader;
	pVertexShader->loadShaderSourceFromFile("vertDrawScreenQuad.glsl");
	pVertexShader->setType(osg::Shader::VERTEX);
	osg::Shader *pFragmentShader = new osg::Shader;
	pFragmentShader->loadShaderSourceFromFile("fragDirectCopy.glsl");
	pFragmentShader->setType(osg::Shader::FRAGMENT);

	osg::Program *pProgram = new osg::Program;
	pProgram->addShader(pVertexShader);
	pProgram->addShader(pFragmentShader);

	return pProgram;
}

//***********************************************************
//FUNCTION:
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
	//Geom->setVertexArray(vc.get());
	Geom->setVertexAttribArray(0, vc.get());
	Geom->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX); 

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));
//	Geom->setTexCoordArray(0,vt.get());
	Geom->setVertexAttribArray(1, vt.get());
	Geom->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);  

	//添加图元
	Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	//绘制
	Geode->addDrawable(Geom.get());

	return Geode.get();
}

