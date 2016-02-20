#include "VertexVistor.h"

CVertexVistor::CVertexVistor(void) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	m_ExtractedVerts = new osg::Vec3Array;
}

CVertexVistor::~CVertexVistor(void)
{
}

//***********************************************************
//FUNCTION:
void CVertexVistor::apply(osg::Geode& vGeode)
{
	for (unsigned int i=0; i<vGeode.getNumDrawables(); i++)
	{
		osg::Geometry* pGeom = dynamic_cast<osg::Geometry*>(vGeode.getDrawable(i));
		
		if (!pGeom)
		{
			std::cout << "Geometry error" << std::endl;
			continue;
		}
		osg::Vec3Array* pVerts = dynamic_cast<osg::Vec3Array*>(pGeom->getVertexArray());
		if (!pVerts)
		{
			std::cout << "Vextex array error" << std::endl;
			continue;
		}

		m_ExtractedVerts->insert(m_ExtractedVerts->end(), pVerts->begin(), pVerts->end());
	}
}
