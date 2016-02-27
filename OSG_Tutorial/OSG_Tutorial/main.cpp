/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/AnimationPathManipulator>

#include <osgUtil/Optimizer>

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group() ;

	//读入cow模型
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("../OSGData/lz.osg");

	//申请一个操作器
	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator("animation.path") ;

	//启用操作器
	viewer->setCameraManipulator(apm.get()) ;

	root->addChild(cow.get()) ;

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get()) ;

	viewer->realize() ;

	viewer->run() ;

	return 0 ;
}