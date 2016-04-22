#include "PickNode.h"

CPickNode::CPickNode(void) : m_X(0.0), m_Y(0.0)
{
}

CPickNode::~CPickNode(void)
{
}

//***********************************************************
//FUNCTION:
bool CPickNode::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*>(&aa);
	if (!view)	return false;
	
	switch (ea.getEventType())
	{
	case (osgGA::GUIEventAdapter::DOUBLECLICK):
		{	
			static bool Flag = true;
			m_X = ea.getX();
			m_Y = ea.getY();
			Flag = __pickNode(view.get(), m_X, m_Y);			
			break;
		}
	default:
		break;
	}
	
	return false;
}

//***********************************************************
//FUNCTION:
bool CPickNode::__pickNode(osg::ref_ptr<osgViewer::View> vView, float vX, float vY)
{
	osg::ref_ptr<osg::Node> SelectedNode;
	osg::ref_ptr<osg::Group> ParentNode;

	osgUtil::LineSegmentIntersector::Intersections intersections;
	if (vView->computeIntersections(vX, vY, intersections))
	{
		osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
		osg::NodePath& nodePath = intersection.nodePath;

		SelectedNode = (nodePath.size() > 1) ? nodePath[nodePath.size()-1] : 0;
		ParentNode = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]) : nullptr;
	}

	if (SelectedNode.get() && ParentNode.get())
	{
		if (SelectedNode == m_PrevSelectedNode) return false;

		if (SelectedNode != m_PrevSelectedNode && m_PrevSelectedNode)
		{
			m_PrevParentGroup->replaceChild(m_PrevMatrixTransform.get(), m_PrevSelectedNode.get());
		}

		m_PrevSelectedNode = SelectedNode;
		m_PrevParentGroup = ParentNode;

		const osg::BoundingSphere& NodeBoundingSphere = SelectedNode->getBound();
		osg::ref_ptr<osg::Geode> LineNode = new osg::Geode;
		osg::ref_ptr<osg::Geode> MaskNode = new osg::Geode;

		__drawCoordinateSystem(NodeBoundingSphere.center(), NodeBoundingSphere.radius(), LineNode, MaskNode);
		osg::ref_ptr<osg::MatrixTransform> CurMatrixTransform = new osg::MatrixTransform;

		ParentNode->replaceChild(SelectedNode.get(), CurMatrixTransform.get());
		CurMatrixTransform->addChild(SelectedNode.get());
		CurMatrixTransform->addChild(LineNode.get());
		CurMatrixTransform->addChild(MaskNode.get());
 		m_PrevMatrixTransform = CurMatrixTransform;

		return true;
	}

	return false;
}

//***********************************************************
//FUNCTION:
void CPickNode::__drawCoordinateSystem(const osg::Vec3f& vOrigin, float vLength, osg::ref_ptr<osg::Geode>& vLineNode,  osg::ref_ptr<osg::Geode>& vMaskNode)
{
	__drawCoordinateLine(vOrigin, vLength, vLineNode);
	__drawCoordinateMask(vOrigin, vLength, vMaskNode);
}

//***********************************************************
//FUNCTION:
void CPickNode::__drawCoordinateLine(const osg::Vec3f& vOrigin, float vLength, osg::ref_ptr<osg::Geode>& vLineNode)
{
	osg::ref_ptr<osg::Geometry> CoordGeometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> CoordVertex = new osg::Vec3Array();

	CoordVertex->push_back(vOrigin);
	CoordVertex->push_back(osg::Vec3(vOrigin.x()+vLength, vOrigin.y(), vOrigin.z()));
	CoordVertex->push_back(vOrigin);
	CoordVertex->push_back(osg::Vec3(vOrigin.x(), vOrigin.y()+vLength, vOrigin.z()));
	CoordVertex->push_back(vOrigin);
	CoordVertex->push_back(osg::Vec3(vOrigin.x(), vOrigin.y(), vOrigin.z()+vLength));
	CoordGeometry->setVertexArray(CoordVertex.get());
	CoordGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6));

	osg::ref_ptr<osg::Vec4Array> VertColor = new osg::Vec4Array();
	VertColor->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	VertColor->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	VertColor->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	VertColor->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	VertColor->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	VertColor->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	CoordGeometry->setColorArray(VertColor.get());
	CoordGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::LineWidth> LineSize = new osg::LineWidth();
	LineSize->setWidth(5);
	vLineNode->getOrCreateStateSet()->setAttributeAndModes(LineSize.get(), osg::StateAttribute::ON);
	vLineNode->addDrawable(CoordGeometry.get());
}

//***********************************************************
//FUNCTION:
void CPickNode::__drawCoordinateMask(const osg::Vec3f& vOrigin, float vLength, osg::ref_ptr<osg::Geode>& vMaskNode)
{
	osg::ref_ptr<osg::Geometry> MakGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> MaskVert = new osg::Vec3Array();

	for (unsigned int i=1; i<=10; i++)
	{
		float Offfset = vLength / 10.0 * i;
		MaskVert->push_back(osg::Vec3(vOrigin.x()+Offfset, vOrigin.y(), vOrigin.z()));
		MaskVert->push_back(osg::Vec3(vOrigin.x(), vOrigin.y()+Offfset, vOrigin.z()));
		MaskVert->push_back(osg::Vec3(vOrigin.x(), vOrigin.y(), vOrigin.z()+Offfset));
	}
	osg::ref_ptr<osg::Vec4Array> ColorArray = new osg::Vec4Array;
	ColorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	MakGeom->setVertexArray(MaskVert.get());
	MakGeom->setColorArray(ColorArray);
	MakGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

	MakGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 30));
	vMaskNode->addDrawable(MakGeom.get());

	osg::ref_ptr<osg::Point> PointSize = new osg::Point(5.0);
	osg::ref_ptr<osg::StateSet> PointStateSet = vMaskNode->getOrCreateStateSet();
	PointStateSet->setAttribute(PointSize);
}