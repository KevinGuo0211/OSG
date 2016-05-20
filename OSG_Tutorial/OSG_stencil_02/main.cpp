#include <osg/GL>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/Stencil>

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::Geometry* pLineStripGeometry = new osg::Geometry;
	osg::Vec3Array* pLineVertexArray = new osg::Vec3Array;
	double dRadius = 0.1;
	for (float angle = 0.0; angle < 400.0; angle += 0.1)
	{
		pLineVertexArray->push_back(osg::Vec3(dRadius * cos(angle), dRadius * sin(angle), 0));
		dRadius *= 1.002;
	}
	osg::Vec4Array* pLineColorArray = new osg::Vec4Array;
	pLineColorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	pLineStripGeometry->setVertexArray(pLineVertexArray);
	pLineStripGeometry->setColorArray(pLineColorArray);
	pLineStripGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	pLineStripGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, pLineVertexArray->size()));
	pLineStripGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::Stencil* pLineStripStencil = new osg::Stencil;
	pLineStripStencil->setFunction(osg::Stencil::NEVER, 0x0, 0x0);
	pLineStripStencil->setOperation(osg::Stencil::INCR, osg::Stencil::INCR, osg::Stencil::INCR);
	pLineStripGeometry->getOrCreateStateSet()->setAttributeAndModes(pLineStripStencil);

	osg::Geometry* pQuadGeometry = new osg::Geometry;
	osg::Vec3Array* pQuadVertexArray = new osg::Vec3Array;
	pQuadVertexArray->push_back(osg::Vec3(0, 0, 0));
	pQuadVertexArray->push_back(osg::Vec3(25, 0, 0));
	pQuadVertexArray->push_back(osg::Vec3(25, 25, 0));
	pQuadVertexArray->push_back(osg::Vec3(0, 25, 0));

	osg::Vec4Array* pQuadColorArray = new osg::Vec4Array;
	pQuadColorArray->push_back(osg::Vec4(0.0, 1.0, 1.0, 1.0));

	pQuadGeometry->setVertexArray(pQuadVertexArray);
	pQuadGeometry->setColorArray(pQuadColorArray);
	pQuadGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	pQuadGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	pQuadGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::Stencil* pQuadStencil = new osg::Stencil;
	pQuadStencil->setFunction(osg::Stencil::NOTEQUAL, 0x1, 0x1);
	pQuadStencil->setOperation(osg::Stencil::KEEP, osg::Stencil::KEEP, osg::Stencil::KEEP);
	pQuadGeometry->getOrCreateStateSet()->setAttributeAndModes(pQuadStencil);

	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable(pLineStripGeometry);
	pGeode->addDrawable(pQuadGeometry);

	root->addChild(pGeode);
	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();
}