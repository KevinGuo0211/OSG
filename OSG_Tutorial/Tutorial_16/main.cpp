/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-21
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: ����ͼ�����
*/

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil//Optimizer>
#include <iostream>

//�����ڶ������Ⱦ
void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	//��������ϵͳ�ӿ�
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
	if (!wsi) 
	{
		osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
		return;
	}

	//�õ����ڷֱ���
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//����ͼ�λ���
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid())
	{
		osg::notify(osg::INFO)<<"  GraphicsWindow has been created successfully."<<std::endl;

		//ȷ����������ɾ�
		gc->setClearColor(osg::Vec4f(0.2f,0.2f,0.6f,1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE)<<"  GraphicsWindow has not been created successfully."<<std::endl;
	}

	//�õ�cameraMaster(�����)
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//����ͼ�λ���
	cameraMaster->setGraphicsContext(gc.get());

	//���ݷֱ�����ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
	double fovy, aspectRatio, zNear, zFar;
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0);
	}

	//�����ӿ�
	cameraMaster->setViewport(new osg::Viewport(0,0, width, height));
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);

	//�����������
	osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	cameraClient->setGraphicsContext(gc.get());
	cameraClient->setViewport(new osg::Viewport(0,0, 400, 400));
	GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	cameraClient->setDrawBuffer(bufferClient);
	cameraClient->setReadBuffer(bufferClient);

	//��Ӵ������
	viewer->addSlave(cameraClient,osg::Matrix::scale(aspectRatio,1.0,1.0),osg::Matrix()) ;
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer =new osgViewer::Viewer();
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	
	singleWindowMultipleCameras(viewer.get());

	osgUtil::Optimizer optimizer ;
	optimizer.optimize(cow.get());
	viewer->setSceneData(cow.get());
	viewer->realize();
	viewer->run();

	return 0;
}