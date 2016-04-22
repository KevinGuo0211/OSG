#include <iostream>
#include <string>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>
#include <osg/BlendFunc>
#include <osg/ShapeDrawable>

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
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

// 	osg::ref_ptr<osg::ShapeDrawable> doorShape1 =  new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f) );  
// 	osg::ref_ptr<osg::ShapeDrawable> doorShape =  new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 2.0f, 2.0f, 2.0f) );  
// 	doorShape->setColor( osg::Vec4(0.0f, 1.0f, 1.0f, 0.5f) );   // alpha value  
// 	doorShape1->setColor( osg::Vec4(0.0f, 1.0f, 0.0f, 0.5f) ); 
// 
// 	osg::StateSet* stateset = doorShape->getOrCreateStateSet();  
// 	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
// 	osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();  // blend func      
// 	blendFunc->setSource(osg::BlendFunc::SRC_ALPHA);         
// 	blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_SRC_ALPHA);          
// 	stateset->setAttributeAndModes( blendFunc );  
// 	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);    
// 
// 	osg::ref_ptr<osg::Geode> geode = new osg::Geode;  
// 	geode->addDrawable( doorShape.get() );  
// 	osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;  
// 	geode1->addDrawable( doorShape1.get() );  
// 
// 	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform;  
// 	trans->addChild( geode.get() );  
// 	trans->addChild( geode1.get() );  
// 	osg::ref_ptr<osg::MatrixTransform> RootGroup = new osg::MatrixTransform;

	osg::ref_ptr<osg::Node> CowNode = osgDB::readNodeFile("cow.osg");
	osg::StateSet* pStateSet = CowNode->getOrCreateStateSet();
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	osg::ref_ptr<osg::BlendFunc> BlendFun = new osg::BlendFunc;
	BlendFun->setSource(osg::BlendFunc::SRC_ALPHA);
	BlendFun->setDestination(osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	pStateSet->setAttributeAndModes(BlendFun);
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

//	osg::Group* pGroup = dynamic_cast<osg::Group*>(CowNode.get());
	osg::ref_ptr<osg::Node> CoordNode = osgDB::readNodeFile("test.ive");
	osg::ref_ptr<osg::MatrixTransform> CoordNodeMatrix = new osg::MatrixTransform;
	osg::Matrix Mat;
	Mat.makeScale(0.1, 0.1, 0.1);
	CoordNodeMatrix->addChild(CoordNode);
	CoordNodeMatrix->setMatrix(Mat);
	
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(CoordNodeMatrix.get());
	root->addChild(CowNode.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

	return 1;
}