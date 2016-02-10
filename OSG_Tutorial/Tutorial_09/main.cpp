#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/AutoTransform>
#include <osgText/Text>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
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
	//_crtBreakAlloc = 17864;
#endif
}

//******************************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> createAutoTransform(osg::Vec3& vPosition, float vSize, std::string& vLabel, osg::AutoTransform::AutoRotateMode vAutoMode, 
											osgText::Text::AxisAlignment vAxisAlignment)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	std::string font("../OSGData/cour.ttf");

	//设置Text对象
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	geode->addDrawable(text.get());

	text->setFont(font);
	text->setFontResolution(128.0, 128.0);//设置分辨率，默认32*32
	text->setCharacterSize(vSize);		//设置字体的大小
	text->setAlignment(osgText::Text::CENTER_CENTER); //设置方向
	text->setText(vLabel); //设置文本
	
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform;
	at->addChild(geode.get());
	at->setAutoRotateMode(vAutoMode);
	at->setAutoScaleToScreen(false);
	at->setMinimumScale(0.0);
	at->setPosition(vPosition);

	return at.get();
}

int main(int argc, char** argv)
{
	installMemoryLeakDetector();
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	
	std::string text("Fly To Sky");
	root->addChild(createAutoTransform(osg::Vec3(0.0, 0.0, 0.0), 60.0, text,
					osg::AutoTransform::ROTATE_TO_SCREEN, osgText::Text::XY_PLANE));

	root->addChild(createAutoTransform(osg::Vec3(0.0, 0.0, 0.0), 60.0, text,
					osg::AutoTransform::NO_ROTATION, osgText::Text::YZ_PLANE));
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}