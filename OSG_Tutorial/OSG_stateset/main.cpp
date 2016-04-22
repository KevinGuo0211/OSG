/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/ClipNode>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>

osg::ref_ptr<osg::Node> createClipNode(osg::ref_ptr<osg::Node> subgraph)
{
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//多边形线形绘制模式，正面和反面都绘制
	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode();
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);

	//启用多边形线形绘制模式，并指定状态继承属性为OVERRIDE
	stateset->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

	//多边形线形绘制节点
	osg::ref_ptr<osg::Group> wireframe_subgraph = new osg::Group;
	//设置渲染状态
	wireframe_subgraph->setStateSet(stateset.get());
	wireframe_subgraph->addChild(subgraph.get());

	root->addChild(wireframe_subgraph.get());

	osg::ref_ptr<osg::MatrixTransform> transform= new osg::MatrixTransform;
	//更新回调，实现动态裁剪
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(subgraph->getBound().center(),osg::Vec3(0.0f,0.0f,1.0f),osg::inDegrees(45.0f));
	transform->setUpdateCallback(nc.get());

	//创建裁剪节点
	osg::ref_ptr<osg::ClipNode> clipnode = new osg::ClipNode;
	osg::BoundingSphere bs = subgraph->getBound();
	bs.radius()*= 0.4f;

	//设置裁剪节点的包围盒
	osg::BoundingBox bb;
	bb.expandBy(bs);

	//根据前面指定的包围盒创建六个裁剪平面
	clipnode->createClipBox(bb);
	//禁用拣选
	clipnode->setCullingActive(false);

	transform->addChild(clipnode.get());
	root->addChild(transform.get());

	//创建未被裁剪的节点
	osg::ref_ptr<osg::Group> clippedNode = new osg::Group;
	clippedNode->setStateSet(clipnode->getStateSet());
	clippedNode->addChild(subgraph.get());
	root->addChild(clippedNode.get());

	return root.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Node> root = new osg::Node() ;

	//加载模型
	osg::ref_ptr<osg::Node> node =osgDB::readNodeFile("../OSGData/cessna.osg");

	root = createClipNode(node.get());

	//优化场景数据
	osgUtil::Optimizer optimzer;
	optimzer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}
