#pragma once
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>

class CSpaceShipCallBack : public osg::NodeCallback
{
public:
	CSpaceShipCallBack(void);
	~CSpaceShipCallBack(void);

	virtual void operator()(osg::Node* vNode, osg::NodeVisitor* vNodeVis) override
	{
		osg::ref_ptr<osg::MatrixTransform> MatrixTran = dynamic_cast<osg::MatrixTransform*>(vNode);
		osg::Matrix Mat;
		Mat.makeRotate(m_Angle, osg::Vec3(0.0, 0.0, 1.0));
		MatrixTran->setMatrix(Mat);

		m_Angle = 0.01;

		traverse(vNode, vNodeVis);
	}

	double m_Angle;
};

