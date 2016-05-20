/************************************************************************/
/* G-Buffer                                                             */
/************************************************************************/

#include <osg/GL>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osgGA/TrackballManipulator>
#include <osg/FrameBufferObject>
#include <osg/Depth>

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

osg::ref_ptr<osg::Node> createQuad()
{
	osg::ref_ptr<osg::Geode> Geode = new osg::Geode();
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
	Geode->addDrawable(Geom.get());
	Geode->setName("NODE");
	return Geode.get();
}

osg::Program* createShaderProgram(const std::string& vVertexShaderFile, const std::string& vFragmentShaderFile)
{
	osg::Shader *pVertexShader = new osg::Shader;
	pVertexShader->loadShaderSourceFromFile(vVertexShaderFile);
	pVertexShader->setType(osg::Shader::VERTEX);
	osg::Shader *pFragmentShader = new osg::Shader;
	pFragmentShader->loadShaderSourceFromFile(vFragmentShaderFile);
	pFragmentShader->setType(osg::Shader::FRAGMENT);

	osg::Program *pProgram = new osg::Program;
	pProgram->addShader(pVertexShader);
	pProgram->addShader(pFragmentShader);

	return pProgram;
}

int main(int argc, char** argv)
{
	osgViewer::Viewer Viewer;
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits;
	pTraits->x = 50;
	pTraits->y = 50;
	pTraits->width = WIN_WIDTH;
	pTraits->height = WIN_HEIGHT;
	pTraits->windowName = "OSG Tutorial 06 - GBuffer";
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	osg::GraphicsContext *pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);

	osg::ref_ptr<osg::Camera> pCamera = Viewer.getCamera();
	pCamera->setGraphicsContext(pGraphicsContext);	
	pCamera->setClearColor(osg::Vec4f(0.0, 0.0, 0.0, 0.0));
	pCamera->setViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	pCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCamera->setViewMatrix(osg::Matrix::identity());
	pCamera->setProjectionMatrix(osg::Matrix::identity());

	osg::State* state = pCamera->getGraphicsContext()->getState(); 
	state->setUseModelViewAndProjectionUniforms(true); 
	state->setUseVertexAttributeAliasing(true);

	osg::ref_ptr<osg::Group> pRoot = new osg::Group;

	osg::Group *pPrimaryPass = new osg::Group;
	osg::MatrixTransform *pTransform = new osg::MatrixTransform;
	pPrimaryPass->addChild(pTransform);

	osg::Program *pPerPixelShader = createShaderProgram("vertPerpixelShading.glsl", "fragPerpixelShading.glsl");
	pPrimaryPass->getOrCreateStateSet()->setAttribute(pPerPixelShader, osg::StateAttribute::ON);

	osg::Node *pModel = osgDB::readNodeFile("../model/bunny.ive");
	pModel->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	pModel->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
	pTransform->addChild(pModel);

	osg::Group *pCopyPass = new osg::Group;
	osg::MatrixTransform *pCopyTransform = new osg::MatrixTransform;
	pCopyPass->addChild(pCopyTransform);
	osg::ref_ptr<osg::Node> pQuad = createQuad();
	pCopyTransform->addChild(pQuad.get());
	osg::Program *pCopyShader = createShaderProgram("vertDrawScreenQuad.glsl", "fragDirectCopy.glsl");
	pCopyPass->getOrCreateStateSet()->setAttribute(pCopyShader, osg::StateAttribute::ON);

// 	osg::ref_ptr<osg::Texture2D> pDepthBufferTex = new osg::Texture2D;
// 	pDepthBufferTex->setTextureSize(WIN_WIDTH, WIN_HEIGHT);
// 	pDepthBufferTex->setInternalFormat(GL_DEPTH_COMPONENT);
// 	pDepthBufferTex->setSourceFormat(GL_DEPTH_COMPONENT);
// 	pDepthBufferTex->setSourceType(GL_FLOAT);

	osg::ref_ptr<osg::Texture2D> pPositionTex = new osg::Texture2D;
	pPositionTex->setTextureSize(WIN_WIDTH, WIN_HEIGHT);
	pPositionTex->setInternalFormat(GL_RGBA);
	pPositionTex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	pPositionTex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	pPositionTex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	pPositionTex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	//	pPositionTex->setSourceFormat(GL_RGBA);
	//pPositionTex->setSourceType(GL_FLOAT);

	osg::ref_ptr<osg::Texture2D> pNormalTex = new osg::Texture2D;
	pNormalTex->setTextureSize(WIN_WIDTH, WIN_HEIGHT);
	pNormalTex->setInternalFormat(GL_RGBA);
	pNormalTex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	pNormalTex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	pNormalTex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	pNormalTex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	//	pNormalTex->setSourceFormat(GL_RGBA);
	//pNormalTex->setSourceType(GL_FLOAT);

	osg::Camera *pRTTCamera = new osg::Camera();

//	osg::FrameBufferObject *pFBO = new osg::FrameBufferObject();
// 	pFBO->setAttachment(osg::Camera::DEPTH_BUFFER, osg::FrameBufferAttachment(pDepthBufferTex));
// 	pFBO->setAttachment(osg::Camera::DEPTH_BUFFER, osg::FrameBufferAttachment(new osg::RenderBuffer(WIN_WIDTH, WIN_HEIGHT, GL_DEPTH_COMPONENT24)));
// 	pFBO->setAttachment(osg::Camera::COLOR_BUFFER0,	osg::FrameBufferAttachment(pPositionTex));
// 	pFBO->setAttachment(osg::Camera::COLOR_BUFFER1, osg::FrameBufferAttachment(pNormalTex));
// 	osg::StateSet *pRTTStateSet = pRTTCamera->getOrCreateStateSet();
//	pRTTStateSet->setAttribute(pFBO, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE);

// 	osg::Depth *pDepth = new osg::Depth;
// 	pDepth->setWriteMask(true);
// 	pDepth->setFunction(osg::Depth::LESS);
// 	pRTTCamera->getOrCreateStateSet()->setAttribute(pDepth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pRTTCamera->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 0.0));
	pRTTCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pRTTCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	pRTTCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	pRTTCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	pRTTCamera->setViewMatrixAsLookAt(osg::Vec3(0.0, 0.5, 0.0), osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
	pRTTCamera->setProjectionMatrixAsPerspective(60.0, (double)WIN_WIDTH/WIN_HEIGHT, 0.01, 100);
	pRTTCamera->attach(osg::Camera::COLOR_BUFFER0, pPositionTex);
	pRTTCamera->attach(osg::Camera::COLOR_BUFFER1, pNormalTex);

	pRTTCamera->addChild(pPrimaryPass);
	pRoot->addChild(pRTTCamera);

	osg::Camera *pCopyCamera = new osg::Camera;
	pCopyCamera->setClearColor(osg::Vec4(0.2, 0.2, 0.0, 0.0));
	pCopyCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCopyCamera->setViewMatrix(osg::Matrix::identity());
	pCopyCamera->setProjectionMatrix(osg::Matrix::identity());
	osg::StateSet *pStateSet = pCopyCamera->getOrCreateStateSet();
	pStateSet->setTextureAttribute(0, pPositionTex, osg::StateAttribute::ON);
	pStateSet->setTextureAttribute(1, pNormalTex, osg::StateAttribute::ON);

	osg::Uniform *pSampler1 = new osg::Uniform("uColorBuf0", 0);
	osg::Uniform *pSampler2 = new osg::Uniform("uColorBuf1", 1);
	pStateSet->addUniform(pSampler1);
	pStateSet->addUniform(pSampler2);
	pCopyCamera->addChild(pCopyPass);
	pRoot->addChild(pCopyCamera);

	Viewer.setSceneData(pRoot);
	Viewer.run();
}