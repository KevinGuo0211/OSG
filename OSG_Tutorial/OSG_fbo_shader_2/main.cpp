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

osg::ref_ptr<osg::Texture2D> g_Tex;

osg::Node* createScene();
osg::ref_ptr<osg::Node> createQuadNode();
osg::Camera* createRenderTargetCamera(osg::Texture2D* vRenderTexture, osg::Node* vSubGraph);

osg::Program* createPerpixelShadingProgram();
osg::Program* createCopy2ScreenProgram();

void PerpixelShading(osg::Group *vRoot);

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
// 	pGraphicsContext->getState()->setUseModelViewAndProjectionUniforms(true);   //activate osg specified shader uniforms, such as osg_ModelViewProjectionMatrix
// 	pGraphicsContext->getState()->setUseVertexAttributeAliasing(true);			 //activate something like: layout (location = 0) in vec4 _VertexPosition;

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
	PerpixelShading(Root.get());

 	osg::ref_ptr<osg::Node> QuadNode = createQuadNode();

 	QuadNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, g_Tex.get(), osg::StateAttribute::ON);
 	QuadNode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	osg::Program *pCopy2ScreenProgram = createCopy2ScreenProgram();
// 	pCopy2ScreenProgram->addBindFragDataLocation("_VertexPosition",0);  
// 	pCopy2ScreenProgram->addBindFragDataLocation("_VertexTexCoord0",1);

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
osg::Node* createScene()
{
	osg::Group *pRoot = new osg::Group;

	osg::Group *pPrimaryPassNode = new osg::Group;
	pRoot->addChild(pPrimaryPassNode);
	osg::MatrixTransform *pTransform = new osg::MatrixTransform;
	pPrimaryPassNode->addChild(pTransform);

	osg::Node *pModel = osgDB::readNodeFile("../Model/bunny.ive");
	pModel->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	pModel->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

	pTransform->addChild(pModel);
	return pRoot;
}

//***********************************************************
//FUNCTION:
osg::Program* createPerpixelShadingProgram()
{
	osg::Shader *pVertexShader = new osg::Shader;
	pVertexShader->loadShaderSourceFromFile("Shaders/vertPerpixelShading.glsl");
	pVertexShader->setType(osg::Shader::VERTEX);
	osg::Shader *pFragmentShader = new osg::Shader;
	pFragmentShader->loadShaderSourceFromFile("Shaders/fragPerpixelShading.glsl");
	pFragmentShader->setType(osg::Shader::FRAGMENT);

	osg::Program *pProgram = new osg::Program;
	pProgram->addShader(pVertexShader);
	pProgram->addShader(pFragmentShader);

	return pProgram;
}

//***********************************************************
//FUNCTION:
osg::Program* createCopy2ScreenProgram()
{
	osg::Shader *pVertexShader = new osg::Shader;
	pVertexShader->loadShaderSourceFromFile("Shaders/vertDrawScreenQuad.glsl");
	pVertexShader->setType(osg::Shader::VERTEX);
	osg::Shader *pFragmentShader = new osg::Shader;
	pFragmentShader->loadShaderSourceFromFile("Shaders/fragDirectCopy.glsl");
	pFragmentShader->setType(osg::Shader::FRAGMENT);

	osg::Program *pProgram = new osg::Program;
	pProgram->addShader(pVertexShader);
	pProgram->addShader(pFragmentShader);

	return pProgram;
}

//***********************************************************
//FUNCTION:
osg::Camera* createRenderTargetCamera(osg::Texture2D* vRenderTexture, osg::Node* vSubGraph)
{
	osg::Camera* pRTTCamera = new osg::Camera;
	_ASSERT(pRTTCamera && vRenderTexture && vSubGraph);

	pRTTCamera->setClearColor(osg::Vec4f(1.0, 1.0, 1.0, 1.0));
	pRTTCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pRTTCamera->setViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	pRTTCamera->setRenderOrder(osg::Camera::PRE_RENDER);

	const osg::BoundingSphere& BoundingSphere = vSubGraph->getBound();
	pRTTCamera->setProjectionMatrixAsPerspective(60.0, SCREEN_WIDTH/SCREEN_HEIGHT, 0.1, 100);

	pRTTCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	pRTTCamera->setViewMatrixAsLookAt(BoundingSphere.center()+osg::Vec3(0.0f,-2.0f,0.0f)*BoundingSphere.radius(), BoundingSphere.center(),osg::Vec3(0.0f,0.0f,1.0f));
	pRTTCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	pRTTCamera->attach(osg::Camera::COLOR_BUFFER0, vRenderTexture);
	pRTTCamera->addChild(vSubGraph);

	return pRTTCamera;
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
//	Geom->setVertexArray(vc.get());
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

//***********************************************************
//FUNCTION:
void PerpixelShading(osg::Group *vRoot)
{
	g_Tex = new osg::Texture2D;
	g_Tex->setTextureSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	g_Tex->setInternalFormat(GL_RGBA);
	g_Tex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	g_Tex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	g_Tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	g_Tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

	osg::Node *pSubGraph = createScene();
	osg::Program *pProgram = createPerpixelShadingProgram();
	pSubGraph->getOrCreateStateSet()->setAttribute(pProgram, osg::StateAttribute::ON);

	osg::Camera* pRTTCamera = createRenderTargetCamera(g_Tex.get(), pSubGraph);
	vRoot->addChild(pRTTCamera);
}
