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

#include <osgUtil/Optimizer>

#include "ManipulatorTravel.h"

int main()
{
	//����Viewer���󣬳��������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	//�����������뵽������
	TravelManipulator::TravelToScene(viewer.get());

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//��ȡ����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/lz.osg");

	//��ӵ�����
	root->addChild(node.get());

	//�Ż���������
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}