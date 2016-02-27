#include <iostream>
#include <string>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

class A
{
public:
	explicit A(int a) : value(a) {}
private:
	int value;
};


int main()
{
	A a = static_cast<A>(10);
	int test = 1;

	osg::ref_ptr<osg::Group> root = new osg::Group();
		
	if (root.get())
	{
		std::cout << 1;
	}

	{
		osg::ref_ptr<osg::Group> root2 = root;

		if (root.valid())
		{
			std::cout << 1;
		}
	}

		int test2 = 1;

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/cow.osg");

	root->addChild(node.get());

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

	return 1;
}