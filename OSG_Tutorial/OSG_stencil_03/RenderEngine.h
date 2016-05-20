#pragma once
#include <osg/Texture2D>
#include <osgViewer/Viewer>

class CRenderEngine
{
public:
	CRenderEngine(void);
	~CRenderEngine(void);

	void init();
	void run();

private:
	osg::ref_ptr<osg::Texture2D> m_pRenderTargetTex;
	osg::ref_ptr<osg::Group> m_pSceneGraphRoot;
	osg::ref_ptr<osg::Group> m_pPrimaryPassNode;
	osg::Camera *m_pShadingPassCamera;
	osg::Camera *m_pCopy2ScreenPassCamera;
	osgViewer::Viewer m_Viewer;

	osg::GraphicsContext* __createGraphicsContext();

	osg::Node* __createScene();
	osg::Node* __createQuadNode();
	osg::Camera* __createShadingPass();
	osg::Camera* __createCopy2ScreenPass();
	osg::Program* __createPerpixelShadingProgram();
	osg::Program* __createCopy2ScreenProgram();

	void __createRenderTargetTexture();
	void __initMainCamera();
};

