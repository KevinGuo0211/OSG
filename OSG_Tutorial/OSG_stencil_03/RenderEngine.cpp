#include "RenderEngine.h"
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Viewport>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

CRenderEngine::CRenderEngine(void) : m_pSceneGraphRoot(new osg::Group), m_pPrimaryPassNode(new osg::Group), m_pShadingPassCamera(nullptr)
{
	m_pSceneGraphRoot->addChild(m_pPrimaryPassNode);
}


CRenderEngine::~CRenderEngine(void)
{
}

//***********************************************************
//FUNCTION:
osg::GraphicsContext* CRenderEngine::__createGraphicsContext()
{
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits;
	pTraits->x = 100;
	pTraits->y = 100;
	pTraits->width = 1024;
	pTraits->height = 768;
	pTraits->windowName = "Hive Application";
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	return osg::GraphicsContext::createGraphicsContext(pTraits);
}

//******************************************************************
//FUNCTION:
void CRenderEngine::__createRenderTargetTexture()
{
	m_pRenderTargetTex = new osg::Texture2D;
	m_pRenderTargetTex->setTextureSize(1024, 768);
	m_pRenderTargetTex->setInternalFormat(GL_RGBA);
	m_pRenderTargetTex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	m_pRenderTargetTex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	m_pRenderTargetTex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	m_pRenderTargetTex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
}

//***********************************************************
//FUNCTION:
void CRenderEngine::init()
{
	__initMainCamera();

	osgGA::CameraManipulator* pCameraManipulator = new osgGA::TrackballManipulator();
//	pCameraManipulator->setAutoComputeHomePosition(false);
	m_Viewer.setCameraManipulator(pCameraManipulator);	
//	m_Viewer.setReleaseContextAtEndOfFrameHint(false);

	m_Viewer.setSceneData(m_pSceneGraphRoot);
	m_pSceneGraphRoot->dirtyBound();

 	pCameraManipulator->setHomePosition(osg::Vec3f(-0.0168404,-0.265215,-0.00153695), osg::Vec3f(-0.0168404,0.110154,-0.00153695), osg::Vec3f(0,0,1));
 	pCameraManipulator->home(0);
	pCameraManipulator->updateCamera(*m_Viewer.getCamera());
	
	if (!m_Viewer.isRealized()) m_Viewer.realize();
	m_Viewer.init();

	osg::Camera *pCamera = m_Viewer.getCamera();
	pCamera->setProjectionMatrixAsPerspective(60.0, 1024.0/768, 0.01, 10);
	pCamera->setViewport(0, 0, 1024, 768);

	m_pShadingPassCamera = __createShadingPass();
	__createRenderTargetTexture();

	m_pCopy2ScreenPassCamera = __createCopy2ScreenPass();
	m_pPrimaryPassNode->addChild(m_pCopy2ScreenPassCamera->getChild(0));
	m_pCopy2ScreenPassCamera->removeChild(0, 1);

	m_pShadingPassCamera->setRenderOrder(osg::Camera::PRE_RENDER, -1);
	m_pSceneGraphRoot->addChild(m_pShadingPassCamera);

	osg::Program *pProgram = __createPerpixelShadingProgram();
	m_pShadingPassCamera->getChild(0)->getOrCreateStateSet()->setAttribute(pProgram, osg::StateAttribute::ON);
	m_pShadingPassCamera->attach(osg::Camera::COLOR_BUFFER0, m_pRenderTargetTex);

	osg::Program *pCopy2ScreenProgram = __createCopy2ScreenProgram();
	m_pPrimaryPassNode->getChild(0)->getOrCreateStateSet()->setAttribute(pCopy2ScreenProgram, osg::StateAttribute::ON);
	osg::Uniform* pUniform = new osg::Uniform("uColorBuf", 0);
	m_pPrimaryPassNode->getChild(0)->getOrCreateStateSet()->addUniform(pUniform);
}

//******************************************************************
//FUNCTION:
void CRenderEngine::__initMainCamera()
{
	osg::Camera *pCamera = m_Viewer.getCamera();
//	pCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
//	pCamera->setAllowEventFocus(true);

	pCamera->setGraphicsContext(__createGraphicsContext());

	pCamera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	pCamera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
}

//***********************************************************
//FUNCTION:
void CRenderEngine::run()
{
// 	while (!m_Viewer.done())
// 	{
// 		m_Viewer.eventTraversal();
// 		m_Viewer.updateTraversal();
// 		m_Viewer.renderingTraversals();
// 		m_Viewer.advance();
// 	}
	m_Viewer.run();
}

//***********************************************************
//FUNCTION:
osg::Node* CRenderEngine::__createScene()
{
	osg::MatrixTransform *pTransform = new osg::MatrixTransform;

	osg::Node *pModel = osgDB::readNodeFile("../Model/bunny.ive");
	pModel->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);  //this line is important if the model will be scaled later, otherwise normal vector might be wrong

	pTransform->addChild(pModel);
	return pTransform;
}

//***********************************************************
//FUNCTION:
osg::Program* CRenderEngine::__createPerpixelShadingProgram()
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
osg::Program* CRenderEngine::__createCopy2ScreenProgram()
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
osg::Node* CRenderEngine::__createQuadNode()
{
	osg::ref_ptr<osg::Geometry> Geom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> vc= new osg::Vec3Array();
	vc->push_back(osg::Vec3(-1.0f,0.0f,-1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,-1.0f));
	vc->push_back(osg::Vec3(1.0f,0.0f,1.0f));
	vc->push_back(osg::Vec3(-1.0f,0.0f,1.0f));

	Geom->setVertexAttribArray(0, vc.get());
	Geom->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX); 

	osg::ref_ptr<osg::Vec2Array> vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));

	Geom->setVertexAttribArray(1, vt.get());
	Geom->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);  

	Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	osg::Geode *pGeode = new osg::Geode();
	pGeode->addDrawable(Geom.get());

	return pGeode;
}

//***********************************************************
//FUNCTION:
osg::Camera* CRenderEngine::__createShadingPass()
{
	osg::Camera *pCamera = new osg::Camera;
//	pCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	pCamera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);  //FIXME: should be controlled by configuration
	pCamera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));

	osg::Group *pPasNode = new osg::Group;
	pCamera->addChild(pPasNode);
	pPasNode->addChild(__createScene());

	pCamera->setViewport(0, 0, 1024, 768);

	return pCamera;
}

//***********************************************************
//FUNCTION:
osg::Camera* CRenderEngine::__createCopy2ScreenPass()
{
	osg::Camera *pCamera = new osg::Camera;
//	pCamera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
//	pCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	pCamera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);  //FIXME: should be controlled by configuration
	pCamera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));

	osg::Group *pPasNode = new osg::Group;
	pCamera->addChild(pPasNode);
	pPasNode->addChild(__createQuadNode());

	pCamera->setViewport(0, 0, 1024, 768);

	return pCamera;
}
