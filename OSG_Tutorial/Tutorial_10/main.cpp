/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-17
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: ¼ÆËã°üÎ§ºÐ
*/

#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/PolygonMode>
#include <osg/LineWidth>

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

class BoundingBoxCallback : public osg::NodeCallback
{
public:
	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::BoundingBox bb;
		for ( unsigned int i=0; i<_nodesToCompute.size(); ++i )
		{
			osg::Node* node = _nodesToCompute[i];
			osg::ComputeBoundsVisitor cbbv;
			node->accept( cbbv );

			osg::BoundingBox localBB = cbbv.getBoundingBox();
			osg::Matrix localToWorld = osg::computeLocalToWorld( node->getParent(0)->getParentalNodePaths()[0] );
			for ( unsigned int i=0; i<8; ++i )
				bb.expandBy( localBB.corner(i) * localToWorld );
		}

		m_Center = bb.center();
		m_X = bb.xMax() - bb.xMin();
		m_Y = bb.yMax() - bb.yMin();
		m_Z = bb.zMax() - bb.zMin();

		osg::MatrixTransform* trans = static_cast<osg::MatrixTransform*>(node);
		trans->setMatrix(
			osg::Matrix::scale(bb.xMax()-bb.xMin(), bb.yMax()-bb.yMin(), bb.zMax()-bb.zMin()) *
			osg::Matrix::translate(bb.center()) );

		traverse(node, nv);
	}
	osg::NodePath _nodesToCompute;

	void calcBoundingBox(osg::Vec3f& vCenter, float& vX, float& vY, float& vZ)
	{
		vCenter = m_Center;
		vX = m_X;
		vY = m_Y;
		vZ = m_Z;
	}

private:
	 osg::Vec3f m_Center;
	 float m_X;
	 float m_Y;
	 float m_Z;
};

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> createCude()
{
	osg::ref_ptr<osg::Geode> cube = new osg::Geode;

	float Width = 1.0;
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.5);
	cube->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), Width), hints.get()));

	//const osg::BoundingSphere bs = cube->getBound();
	//osg::BoundingBox bb = cube->getBoundingBox();
	//bb.expandBy(bs);

	return cube.get();
}

//***********************************************************
//FUNCTION:
osg::ref_ptr<osg::Node> drawLine(const osg::Vec3f& vCenter, float vX, float vY, float vZ)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array();
	vert->push_back(vCenter);
	vert->push_back(osg::Vec3(vCenter.x()+vX, vCenter.y(), vCenter.z()));
	vert->push_back(osg::Vec3(vCenter.x(), vCenter.y()+vY, vCenter.z()));
	vert->push_back(osg::Vec3(vCenter.x(), vCenter.y(), vCenter.z()+vZ));
	geom->setVertexArray(vert.get());

	osg::ref_ptr<osg::DrawElementsUInt> quad = new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	quad->push_back(0);
	quad->push_back(1);
	quad->push_back(0);
	quad->push_back(2);
	quad->push_back(0);
	quad->push_back(3);
	geom->addPrimitiveSet(quad.get());

	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
	color->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	color->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	geom->setColorArray(color.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::LineWidth> LineSize = new osg::LineWidth();
	LineSize->setWidth(5);
	geode->getOrCreateStateSet()->setAttributeAndModes(LineSize.get(), osg::StateAttribute::ON);

	geode->addDrawable(geom.get());
	return geode.get();
}

int main()
{
	//osg::ref_ptr<osg::Node> cowNode = osgDB::readNodeFile("../OSGData/cow.osg");
	osg::ref_ptr<osg::MatrixTransform> cessna = new osg::MatrixTransform;
	cessna->addChild( osgDB::readNodeFile("../OSGData/cessna.osg.0,0,90.rot") );
	
	osg::ref_ptr<osg::MatrixTransform> dumptruck = new osg::MatrixTransform;
	dumptruck->addChild( osgDB::readNodeFile("../OSGData/dumptruck.osg") );
	dumptruck->setMatrix( osg::Matrix::translate(0.0f, 0.0f, -100.0f) );

	osg::ref_ptr<osg::MatrixTransform> models = new osg::MatrixTransform;
	models->addChild( cessna.get() );
	models->addChild( dumptruck.get() );
	models->setMatrix( osg::Matrix::translate(0.0f, 0.0f, 200.0f) );

	// World bounding box callback & node
	osg::ref_ptr<BoundingBoxCallback> bbcb = new BoundingBoxCallback;
	bbcb->_nodesToCompute.push_back( cessna.get() );
	bbcb->_nodesToCompute.push_back( dumptruck.get() );
	//osg::Vec3f Center;
	//float X = 0.0, Y = 0.0, Z = 0.0;
	//bbcb->calcBoundingBox(Center, X, Y , Z);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable( new osg::ShapeDrawable(new osg::Box) );

	osg::ref_ptr<osg::MatrixTransform> boundingBoxNode = new osg::MatrixTransform;
	boundingBoxNode->addChild( geode.get() );
	boundingBoxNode->addUpdateCallback( bbcb.get() );
	boundingBoxNode->getOrCreateStateSet()->setAttributeAndModes(
		new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE) );
	boundingBoxNode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(boundingBoxNode.get());
	root->addChild( models.get() );
	//root->addChild(drawLine(Center, X, Y, Z));

	osgUtil::Optimizer optimizier;
	optimizier.optimize(root.get());
	
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();

	return 1;
}