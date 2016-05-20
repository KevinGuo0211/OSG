#include <osg/Node>
#inc


osg::ref_ptr createQuadGrid(int twidth)
{
	osg::ref_ptr geode = new osg::Geode();
	osg::ref_ptr geome = new osg::Geometry();
	osg::ref_ptr nArr = new osg::Vec3Array();

	for(int i = -twidth; i <= twidth ; i += 10)
	{       
		nArr->push_back(osg::Vec3(i,-twidth , -1.0));
		nArr->push_back(osg::Vec3(i,twidth,-1.0));

		nArr->push_back(osg::Vec3(-twidth, i, -1.0));
		nArr->push_back(osg::Vec3(twidth, i,-1.0));
	}
	geome->setVertexArray(nArr.get());

	geome->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,nArr->size()));
	geode->addDrawable(geome.get());
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return geode.release();
}
//添加hud叠加shader;
void addShader(osg::ref_ptr _stateSet)
{
	osg::ref_ptr program = new osg::Program;
	osg::ref_ptr vertShd = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr fragShd = new osg::Shader(osg::Shader::FRAGMENT);
	program->addShader(vertShd);
	program->addShader(fragShd);

	if(!vertShd->loadShaderSourceFromFile("hud.vert"))
	{
		;
	}
	if(!fragShd->loadShaderSourceFromFile("hud.frag"))
	{
		;
	}

	_stateSet->setAttribute(program, osg::StateAttribute::ON);
}
//添加rtt传出数据shader;
void addShader2(osg::ref_ptr _stateSet)
{
	osg::ref_ptr program = new osg::Program;
	osg::ref_ptr vertShd = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr fragShd = new osg::Shader(osg::Shader::FRAGMENT);
	program->addShader(vertShd);
	program->addShader(fragShd);

	if(!vertShd->loadShaderSourceFromFile("rtt.vert"))
	{
		;
	}
	if(!fragShd->loadShaderSourceFromFile("rtt.frag"))
	{
		;
	}

	_stateSet->setAttribute(program, osg::StateAttribute::ON);
}

int main(int argc,  char *argv[])
{

	osg::ref_ptr _root = new osg::Group;
	osg::ref_ptr  _grid = createQuadGrid(30);
	osg::ref_ptr  _axes = osgDB::readNodeFile("axes.osg");
	_root->addChild(_grid);
	_root->addChild(_axes);

	osg::ref_ptr _cow = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr _mmCow = new osg::MatrixTransform;
	_mmCow->addChild(_cow);
	_mmCow->setMatrix(osg::Matrix::translate(osg::Vec3d(10.0,-10.0,0.0)));
	osg::ref_ptr _cep = osgDB::readNodeFile("glider.osg");
	osg::ref_ptr _mmCep = new osg::MatrixTransform;
	_mmCep->addChild(_cep);
	_mmCep->setMatrix(osg::Matrix::translate(osg::Vec3d(14.0, -10.0, 0.0)));


	addShader2(_mmCow->getOrCreateStateSet());
	addShader2(_mmCep->getOrCreateStateSet());

	_mmCow->getOrCreateStateSet()->addUniform(new osg::Uniform("zFar", float(9999.89)));
	_mmCow->getOrCreateStateSet()->addUniform(new osg::Uniform("zNear", float(0.1)));

	_mmCep->getOrCreateStateSet()->addUniform(new osg::Uniform("zFar", float(9999.89)));
	_mmCep->getOrCreateStateSet()->addUniform(new osg::Uniform("zNear", float(0.1)));

	//rttCamera;
	osg::ref_ptr _rttCow = Gavin::CCommon::createRTTCamera(0,0,1920,1080);
	osg::ref_ptr _rttCep = Gavin::CCommon::createRTTCamera(0,0,1920,1080);

	_rttCow->addChild(_mmCow);
	_rttCep->addChild(_mmCep);

	//hudCamera;
	osg::ref_ptr _hudCam = Gavin::CCommon::createHudCamera(0,0,1920,1080);
	osg::ref_ptr           _huGrid = Gavin::CCommon::createPlane(0,0,1920,1080);
	_huGrid->getOrCreateStateSet()->setMode(GL_BLEND, 1);
	_hudCam->addChild(_huGrid);

	_root->addChild(_rttCep);
	_root->addChild(_rttCow);
	_root->addChild(_hudCam);
	//stateSet;
	osg::ref_ptr _state = _huGrid->getOrCreateStateSet();
	_state->setMode(GL_LIGHTING, 0);
	addShader(_state);

	//render to texture;
	//cow;
	osg::ref_ptr _rttCowTex = new osg::Texture2D;
	_rttCowTex->setInternalFormat(GL_RGB);
	_rttCow->attach(osg::Camera::COLOR_BUFFER0, _rttCowTex);

	osg::ref_ptr _rttCowLen = new osg::Texture2D;
	_rttCowLen->setInternalFormat(GL_ALPHA);
	_rttCow->attach(osg::Camera::COLOR_BUFFER1, _rttCowLen);

	_state->setTextureAttributeAndModes(0, _rttCowTex, osg::StateAttribute::ON);
	_state->setTextureAttributeAndModes(1, _rttCowLen, osg::StateAttribute::ON);
	_state->addUniform(new osg::Uniform("_rttCowTex", 0));
	_state->addUniform(new osg::Uniform("_rttCowLen", 1));
	//cep;
	osg::ref_ptr _rttCepTex = new osg::Texture2D;
	_rttCepTex->setInternalFormat(GL_RGB);
	_rttCep->attach(osg::Camera::COLOR_BUFFER0, _rttCepTex);

	osg::ref_ptr _rttCepLen = new osg::Texture2D;
	_rttCepLen->setInternalFormat(GL_ALPHA);
	_rttCep->attach(osg::Camera::COLOR_BUFFER1, _rttCepLen);

	_state->setTextureAttributeAndModes(2, _rttCepTex, osg::StateAttribute::ON);
	_state->setTextureAttributeAndModes(3, _rttCepLen, osg::StateAttribute::ON);
	_state->addUniform(new osg::Uniform("_rttCepTex", 2));
	_state->addUniform(new osg::Uniform("_rttCepLen", 3));

	//viewer render;
	osg::ref_ptr viewer = new osgViewer::Viewer;
	viewer->setSceneData(_root);
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	//viewer->getCamera()->setClearColor(osg::Vec4(0.0,0.0,0.0,1.0));
	while(!viewer->done())
	{

		_rttCow->setViewMatrix(viewer->getCamera()->getViewMatrix());
		_rttCep->setViewMatrix(viewer->getCamera()->getViewMatrix());

		viewer->frame();
	}

	return 0;

}