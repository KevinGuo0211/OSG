#pragma once
#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/Geometry>
#include <iostream>

class CVertexVistor : public osg::NodeVisitor
{
public:
	CVertexVistor(void);
	~CVertexVistor(void);

	virtual void apply(osg::Geode& vGeode) override;
	osg::ref_ptr<osg::Vec3Array> m_ExtractedVerts;
};

