/************************************************************************/
/*Simple Deferred Shading                                               */
/************************************************************************/

#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/MatrixTransform>
#include <osg/Group>
#include <osgGA/TrackballManipulator>
#include <osg/Depth>

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768

osg::ref_ptr<osg::Node> createQuad()
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
	Geom->setVertexAttribArray(0, vc.get());
	Geom->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX); 

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt= new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));
	Geom->setTexCoordArray(0,vt.get());
	Geom->setVertexAttribArray(1, vt.get());
	Geom->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);  

	//添加图元
	Geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	//绘制
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
	pTraits->windowName = "OSG Tutorial 07";
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

	osg::ref_ptr<osg::Texture2D> pDepthTex = new osg::Texture2D;
	pDepthTex->setTextureSize(WIN_WIDTH, WIN_HEIGHT);
	pDepthTex->setInternalFormat(GL_DEPTH_COMPONENT24);
	pDepthTex->setSourceFormat(GL_DEPTH_COMPONENT);
	pDepthTex->setSourceType(GL_FLOAT);

	osg::Node *pBunnyModel = osgDB::readNodeFile("../model/bunny.ive");
//	pBunnyModel->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	pBunnyModel->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

	osg::Node *pBoxModel = osgDB::readNodeFile("../model/bunny.ive");
	pBoxModel->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

	osg::Program *pShader = createShaderProgram("vertGeom.glsl","fragLight.glsl");

	osg::ref_ptr<osg::Group> pRoot = new osg::Group;

	osg::Group *pBunnyPass = new osg::Group;
	osg::MatrixTransform *pBunnyTransform = new osg::MatrixTransform;
	osg::Matrix BunnyMatrix;
	pBunnyTransform->setMatrix(BunnyMatrix);
	pBunnyTransform->addChild(pBunnyModel);
	pBunnyPass->addChild(pBunnyTransform);
	pBunnyPass->getOrCreateStateSet()->setAttribute(pShader, osg::StateAttribute::ON);

	osg::Camera *pBunnyCamera = new osg::Camera;
	pBunnyCamera->setClearColor(osg::Vec4());
	pBunnyCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	pBunnyCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
//	pBunnyCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	pBunnyCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	pBunnyCamera->setViewMatrixAsLookAt(osg::Vec3(0.0, 2.0, 0.0), osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
	pBunnyCamera->setProjectionMatrixAsPerspective(60.0, (double)WIN_WIDTH/WIN_HEIGHT, 0.01, 100);
	pBunnyCamera->attach(osg::Camera::DEPTH_BUFFER, pDepthTex);
	pBunnyCamera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	pBunnyCamera->getOrCreateStateSet()->addUniform(new osg::Uniform("uIndex", 0));
	pBunnyCamera->addChild(pBunnyPass);
	pRoot->addChild(pBunnyCamera);


	osg::Group *pBoxPass = new osg::Group;
	osg::MatrixTransform *pBoxTransform = new osg::MatrixTransform;
	osg::Matrix BoxMatrix;
	BoxMatrix.makeTranslate(0.0, -2.0, 0.0);
	pBoxTransform->setMatrix(BoxMatrix);
	pBoxTransform->addChild(pBoxModel);
	pBoxPass->addChild(pBoxTransform);
	pBoxPass->getOrCreateStateSet()->setAttribute(pShader, osg::StateAttribute::ON);
	
	osg::Camera *pBoxCamera = new osg::Camera;
//	pBoxCamera->setClearColor(osg::Vec4());
	pBoxCamera->setClearMask(GL_NONE);
//	pBunnyCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
//	pBoxCamera->setRenderOrder(osg::Camera::PRE_RENDER);
	pBoxCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	pBoxCamera->setViewMatrixAsLookAt(osg::Vec3(0.0, 2.0, 0.0), osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0));
	pBoxCamera->setProjectionMatrixAsPerspective(60.0, (double)WIN_WIDTH/WIN_HEIGHT, 0.01, 100);
	pBoxCamera->attach(osg::Camera::DEPTH_BUFFER, pDepthTex);
	osg::Depth *pDepth = new osg::Depth;
	pDepth->setFunction(osg::Depth::LESS);
	pDepth->setWriteMask(false);
	pBoxCamera->getOrCreateStateSet()->setAttribute(pDepth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pBoxCamera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pBoxCamera->getOrCreateStateSet()->addUniform(new osg::Uniform("uIndex", 1));
	pBoxCamera->addChild(pBoxPass);
	pRoot->addChild(pBoxCamera);

	Viewer.setSceneData(pRoot);
	osg::State* state = Viewer.getCamera()->getGraphicsContext()->getState(); 
	state->setUseModelViewAndProjectionUniforms(true); 
	state->setUseVertexAttributeAliasing(true);
	Viewer.run();
}
