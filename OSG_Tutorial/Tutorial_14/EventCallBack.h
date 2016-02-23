#pragma once
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

class CEventCallBack : public osg::NodeCallback
{
public:
	CEventCallBack(void);
	~CEventCallBack(void);

	virtual void operator()(osg::Node* vNode, osg::NodeVisitor* vNodeVis) override
	{
		if (vNodeVis->getVisitorType() == osg::NodeVisitor::EVENT_VISITOR)
		{
			osg::ref_ptr<osgGA::EventVisitor> EventVis = dynamic_cast<osgGA::EventVisitor*>(vNodeVis);
			if (EventVis)
			{
				osgGA::GUIActionAdapter* pActionAda = EventVis->getActionAdapter();
				osgGA::EventQueue::Events& Events = EventVis->getEvents();

				for (osgGA::EventQueue::Events::iterator It = Events.begin(); It!=Events.end(); It++)
				{
					handle(*(*It), *(pActionAda));
				}
			}
		}
	}

	virtual bool handle(const osgGA::GUIEventAdapter& vEventAda, osgGA::GUIActionAdapter& vActionAda)
	{
		osg::ref_ptr<osgViewer::Viewer> View = dynamic_cast<osgViewer::Viewer*>(&vActionAda);
		osg::ref_ptr<osg::MatrixTransform> MaxtrixTran = dynamic_cast<osg::MatrixTransform*>(View->getSceneData());

		switch (vEventAda.getEventType())
		{
		case (osgGA::GUIEventAdapter::KEYDOWN):
			{
				if (vEventAda.getKey() == osgGA::GUIEventAdapter::KEY_Up)
				{
					osg::Matrix Mat;
					Mat.makeTranslate(0.0, 0.0, 1.0);
					MaxtrixTran->setMatrix(Mat);
				}

				if (vEventAda.getKey() == osgGA::GUIEventAdapter::KEY_Down)
				{
					osg::Matrix Mat;
					Mat.makeTranslate(0.0, 0.0, -1.0);
					MaxtrixTran->setMatrix(Mat);
				}
			}
			break;
		default:
			break;
		}

		return false;
	}
};

