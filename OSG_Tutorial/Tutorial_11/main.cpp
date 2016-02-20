/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-18
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: 坐标系统刻度表示
*/

#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/OccluderNode>
#include <osg/StateSet>
#include <osg/ConvexPlanarOccluder>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/Point>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <iostream>

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> drawLine(const osg::Vec3f& vCenter, float vRadius)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array();

	vert->push_back(vCenter);
	vert->push_back(osg::Vec3(vCenter.x()+vRadius, vCenter.y(), vCenter.z()));
	vert->push_back(vCenter);
	vert->push_back(osg::Vec3(vCenter.x(), vCenter.y()+vRadius, vCenter.z()));
	vert->push_back(vCenter);
	vert->push_back(osg::Vec3(vCenter.x(), vCenter.y(), vCenter.z()+vRadius));
	geom->setVertexArray(vert.get());
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6));

	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
	color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	color->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	geom->setColorArray(color.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::LineWidth> LineSize = new osg::LineWidth();
	LineSize->setWidth(5);
	geode->getOrCreateStateSet()->setAttributeAndModes(LineSize.get(), osg::StateAttribute::ON);

	geode->addDrawable(geom.get());
	return geode.get();
}

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> drawMark(const osg::Vec3f& vCenter, float vRadius)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array();
	
	for (unsigned int i=1; i<=10; i++)
	{
		float Offfset = vRadius / 10.0 * i;
		vert->push_back(osg::Vec3(vCenter.x()+Offfset, vCenter.y(), vCenter.z()));
		vert->push_back(osg::Vec3(vCenter.x(), vCenter.y()+Offfset, vCenter.z()));
		vert->push_back(osg::Vec3(vCenter.x(), vCenter.y(), vCenter.z()+Offfset));
	}
	osg::ref_ptr<osg::Vec4Array> ColorArray = new osg::Vec4Array;
	ColorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	geom->setVertexArray(vert.get());
	geom->setColorArray(ColorArray);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 30));
	geode->addDrawable(geom.get());

	osg::ref_ptr<osg::Point> point = new osg::Point(5.0);
	osg::ref_ptr<osg::StateSet> ss = geode->getOrCreateStateSet();
	ss->setAttribute(point);

	return geode.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/glider.osg");
	osg::BoundingSphere bs = node->getBound();

	root->addChild(node.get());
	root->addChild(drawLine(bs.center(), bs.radius()));
	root->addChild(drawMark(bs.center(), bs.radius()));

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}