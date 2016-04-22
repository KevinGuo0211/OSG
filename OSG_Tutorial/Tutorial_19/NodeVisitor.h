#pragma once

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>
#include <iostream>

class CNodeVisitor : public osg::NodeCallback
{
public:
	CNodeVisitor();
	~CNodeVisitor ();

	virtual void operator()(osg::Node* vNode, osg::NodeVisitor* vNodeVisitor)
	{
		m_Path = vNodeVisitor->getNodePath();
	}

private:
	osg::NodePath m_Path;
};