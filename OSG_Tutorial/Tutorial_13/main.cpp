/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-20
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: node callback
*/
#include "SpaceShipCallBack.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

int main()
{
	osg::ref_ptr<osgViewer::Viewer> OsgViewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> RootGroup = new osg::Group;
	osg::ref_ptr<osg::Node> CowNode = osgDB::readNodeFile("../OSGData/cow.osg");

	osg::ref_ptr<osg::MatrixTransform> MatrixTran = new osg::MatrixTransform;
	MatrixTran->addChild(CowNode.get());
	MatrixTran->setUpdateCallback(new CSpaceShipCallBack);

	RootGroup->addChild(MatrixTran.get());
	osgUtil::Optimizer Opti;
	Opti.optimize(RootGroup.get());
	OsgViewer->setSceneData(RootGroup.get());
	OsgViewer->realize();
	OsgViewer->run();
}