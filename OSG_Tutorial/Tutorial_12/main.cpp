/* -*-c++-*- OpenSceneGraph
 * Date: 2016-02-20
 * Author: Hanyou Yu <freehyan.gmail.com>
 * Function: Vertex Vistor
*/

#include "VertexVistor.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

//FUNCTION: detect the memory leak in DEBUG mode
void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 26175;
#endif
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> OsgViewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> RootGroup = new osg::Group;
	osg::ref_ptr<osg::Node> CowNode = osgDB::readNodeFile("../OSGData/cow.osg");
	RootGroup->addChild(CowNode);

	CVertexVistor VertVistor;
	CowNode->accept(VertVistor);

	std::ofstream Fout("cow.vertexs");
	unsigned int Size = VertVistor.m_ExtractedVerts->size();

	std::vector<osg::Vec3>::iterator It = VertVistor.m_ExtractedVerts->begin();
	for (unsigned int i=0; i<Size; i++)
	{
		Fout << It->x() << " " << It->y() << " " << It->z() << " " << std::endl;
		It++;
	}

	osgUtil::Optimizer Opti;
	Opti.optimize(RootGroup.get());
	OsgViewer->setSceneData(RootGroup.get());
	OsgViewer->realize();
	OsgViewer->run();

	return 1;
}