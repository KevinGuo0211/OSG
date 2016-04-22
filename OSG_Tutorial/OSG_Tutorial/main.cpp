/* -*-c++-*- OpenSceneGraph Cookbook
 * Chapter 6 Recipe 4
 * Author: Wang Rui <wangray84 at gmail dot com>
*/

#include <osg/Texture2D>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>
//#include "CommonFunctions"

//***********************************************************
//FUNCTION:
osg::Geode* createScreenQuad( float width, float height,
							 float scale )
{
	osg::Geometry* geom = osg::createTexturedQuadGeometry(
		osg::Vec3(), osg::Vec3(width,0.0f,0.0f),
		osg::Vec3(0.0f,height,0.0f),
		0.0f, 0.0f, width*scale, height*scale );
	osg::ref_ptr<osg::Geode> quad = new osg::Geode;
	quad->addDrawable( geom );
	int values = osg::StateAttribute::OFF|
		osg::StateAttribute::PROTECTED;
	quad->getOrCreateStateSet()->setAttribute(
		new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
		osg::PolygonMode::FILL), values );
	quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
	return quad.release();
}

//***********************************************************
//FUNCTION:
osg::Camera* createHUDCamera( double left, double right, double
							 bottom, double top )
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	camera->setClearMask( GL_DEPTH_BUFFER_BIT );
	camera->setRenderOrder( osg::Camera::POST_RENDER );
	camera->setAllowEventFocus( false );
	camera->setProjectionMatrix(
		osg::Matrix::ortho2D(left, right, bottom, top) );
	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING, osg::StateAttribute::OFF );
	return camera.release();
}

//***********************************************************
//FUNCTION:
osg::Camera* createRTTCamera( osg::Camera::BufferComponent
							 buffer, osg::Texture* tex, bool isAbsolute )
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setClearColor( osg::Vec4() );
	camera->setClearMask(
		GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	camera->setRenderTargetImplementation(
		osg::Camera::FRAME_BUFFER_OBJECT );
	camera->setRenderOrder( osg::Camera::PRE_RENDER );
	if ( tex )
	{
		tex->setFilter( osg::Texture2D::MIN_FILTER,
			osg::Texture2D::LINEAR );
		tex->setFilter( osg::Texture2D::MAG_FILTER,
			osg::Texture2D::LINEAR );
		camera->setViewport( 0, 0, tex->getTextureWidth(),
			tex->getTextureHeight() );
		camera->attach( buffer, tex );
	}
	if ( isAbsolute )
	{
		camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
		camera->setProjectionMatrix( osg::Matrix::ortho2D(
			0.0, 1.0, 0.0, 1.0) );
		camera->setViewMatrix( osg::Matrix::identity() );
		camera->addChild( createScreenQuad(1.0f, 1.0f, 1.0) );
	}
	return camera.release();
}


int main( int argc, char** argv )
{
    osg::ArgumentParser arguments( &argc, argv );
    osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles( arguments );
    if ( !scene ) scene = osgDB::readNodeFile("cessna.osg");
    
    osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
    tex2D->setTextureSize( 1024, 1024 );
    tex2D->setInternalFormat( GL_DEPTH_COMPONENT24 );
    tex2D->setSourceFormat( GL_DEPTH_COMPONENT );
    tex2D->setSourceType( GL_FLOAT );
    
    osg::ref_ptr<osg::Camera> rttCamera = createRTTCamera(osg::Camera::DEPTH_BUFFER, tex2D.get(), false);
    rttCamera->addChild( scene.get() );
    
    osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera(0.0, 1.0, 0.0, 1.0);
    hudCamera->addChild( createScreenQuad(0.5f, 1.0f,1.0) );
    hudCamera->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2D.get() );
    
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( rttCamera.get() );
    root->addChild( hudCamera.get() );
    root->addChild( scene.get() );
    
    osgViewer::Viewer viewer;
    viewer.getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
    viewer.setSceneData( root.get() );
    return viewer.run();
}
