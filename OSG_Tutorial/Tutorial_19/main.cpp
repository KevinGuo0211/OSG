#include "PickNode.h"

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

int main()
{
	installMemoryLeakDetector();
	osg::ref_ptr<osgViewer::Viewer> pViewer = new osgViewer::Viewer;

	osg::ref_ptr<osg::GraphicsContext::Traits> pTraits = new osg::GraphicsContext::Traits;
	pTraits->x = 100;
	pTraits->y = 100;
	pTraits->width = 1024;
	pTraits->height = 768;
	pTraits->windowName = "PickNode Application";
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	osg::ref_ptr<osg::GraphicsContext> pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);

	osg::ref_ptr<osg::Camera> pCamera = pViewer->getCamera();
	pCamera->setGraphicsContext(pGraphicsContext);
	pCamera->setViewport(new osg::Viewport(0, 0, pTraits->width, pTraits->height));
	pCamera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	pCamera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	pCamera->setProjectionMatrixAsPerspective(45, (float)pTraits->width/pTraits->height, 0.1, 100);
	pViewer->setCamera(pCamera); 

	osg::ref_ptr<osg::Group> RootGroup = new osg::Group;
	osg::ref_ptr<osg::MatrixTransform> MatrixCowNode = new osg::MatrixTransform;
	osg::ref_ptr<osg::Node> CowNode = osgDB::readNodeFile("cow.osg");
	osg::Matrix m;
	m.makeTranslate(-20, 0.0, 0.0);
	MatrixCowNode->setMatrix(m);
	MatrixCowNode->addChild(CowNode.get());

	osg::ref_ptr<osg::Node> BunnyNode = osgDB::readNodeFile("../OSGData/cessna.osg");
	RootGroup->addChild(CowNode.get());
	RootGroup->addChild(BunnyNode.get());

	pViewer->addEventHandler(new CPickNode);
	osgUtil::Optimizer optimizer;
	optimizer.optimize(RootGroup.get());

	pViewer->setSceneData(RootGroup.get());
	pViewer->realize();
	pViewer->run();

	return 1;
}