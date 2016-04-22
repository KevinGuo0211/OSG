/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org   
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//创建二维纹理属性
osg::ref_ptr<osg::StateSet> createTexture1DState()
{
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("../OSGData/Images/primitives.gif");

	//创建二维纹理
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	//设置边界处理
	texture->setWrap(osg::Texture2D::WRAP_S,osg::Texture2D::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_T,osg::Texture2D::REPEAT);
	//设置滤波
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	//设置贴图
	texture->setImage(image.get());

	//设置自动纹理坐标，并指定相关的平面
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S,osg::Plane(0.0f,0.0f,1.0f,0.0f));

	//创建属性集
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

	//启用二维纹理
	stateset->setTextureAttribute(0,texture.get(),osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	//启用纹理坐标生成器
	stateset->setTextureAttribute(0,texgen.get(),osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_T,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

	return stateset.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../OSGData/cessna.osg");

	//自动生成纹理坐标属性
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset =createTexture1DState();

	node->setStateSet(stateset.get());
	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer ;
	optimizer.optimize(root.get()) ;

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0 ;
}