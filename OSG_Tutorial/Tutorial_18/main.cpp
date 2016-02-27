/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include "PickHandler.h"

int main()
{
	//创建Viewer对象，场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读取地形模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/lz.osg");

	osg::ref_ptr<osgText::Text> updatetext = new osgText::Text();
	CreateHUD *hudText= new CreateHUD();

	//添加到场景
	root->addChild(node.get());
	root->addChild(hudText->createHUD(updatetext.get()));

	//添加PICK事件处理器
	viewer->addEventHandler(new CPickHandler(updatetext.get()));

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}