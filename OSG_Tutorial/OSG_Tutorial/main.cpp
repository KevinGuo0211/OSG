#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>

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

void main( )
{
	installMemoryLeakDetector();

	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("../Model/cow.ive");
	osgViewer::Viewer viewer;
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 40;
		traits->y = 40;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0,0, traits->width, traits->height));
		//GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		//camera->setDrawBuffer(buffer);
		//camera->setReadBuffer(buffer);

		//// add this slave camera to the viewer, with a shift left of the projection matrix
		viewer.addSlave(camera.get());
	}
	osgUtil::Optimizer optimizer;
	optimizer.optimize(loadedModel.get());
	viewer.setSceneData(loadedModel.get());
	viewer.run();
}