#pragma once
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/Point>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgGA/GUIEventHandler>
#include <iostream>

class CPickNode : public osgGA::GUIEventHandler
{
public:
	CPickNode(void);
	~CPickNode(void);
	
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;

private:
	bool __pickNode(osg::ref_ptr<osgViewer::View> vView, float vX, float vY);
	bool __releaseNode(osg::ref_ptr<osgViewer::View> vView, float vX, float vY);

	void __drawCoordinateSystem(const osg::Vec3f& vOrigin, float vLength);
	void __drawCoordinateLine(const osg::Vec3f& vOrigin, float vLength);
	void __drawCoordinateMask(const osg::Vec3f& vOrigin, float vLength);

	osg::ref_ptr<osg::Node> m_PrevSelectedNode;
	osg::ref_ptr<osg::Group> m_PrevParentGroup;
	osg::ref_ptr<osg::Geode> m_LineNode;
	osg::ref_ptr<osg::Geode> m_MaskNode;

	float m_X;
	float m_Y;
};