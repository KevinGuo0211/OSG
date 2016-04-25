// /* -*-c++-*- OpenSceneGraph Cookbook
//  * Chapter 6 Recipe 4
//  * Author: Wang Rui <wangray84 at gmail dot com>
// */
// 
// #include <osg/Texture2D>
// #include <osg/Group>
// #include <osgDB/ReadFile>
// #include <osgViewer/Viewer>
// #include <osg/PolygonMode>
// //#include "CommonFunctions"
// 
// //***********************************************************
// //FUNCTION:
// osg::Geode* createScreenQuad( float width, float height,
// 							 float scale )
// {
// 	osg::Geometry* geom = osg::createTexturedQuadGeometry(
// 		osg::Vec3(), osg::Vec3(width,0.0f,0.0f),
// 		osg::Vec3(0.0f,height,0.0f),
// 		0.0f, 0.0f, width*scale, height*scale );
// 	osg::ref_ptr<osg::Geode> quad = new osg::Geode;
// 	quad->addDrawable( geom );
// 	int values = osg::StateAttribute::OFF|
// 		osg::StateAttribute::PROTECTED;
// 	quad->getOrCreateStateSet()->setAttribute(
// 		new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
// 		osg::PolygonMode::FILL), values );
// 	quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
// 	return quad.release();
// }
// 
// //***********************************************************
// //FUNCTION:
// osg::Camera* createHUDCamera( double left, double right, double
// 							 bottom, double top )
// {
// 	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
// 	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
// 	camera->setClearMask( GL_DEPTH_BUFFER_BIT );
// 	camera->setRenderOrder( osg::Camera::POST_RENDER );
// 	camera->setAllowEventFocus( false );
// 	camera->setProjectionMatrix(
// 		osg::Matrix::ortho2D(left, right, bottom, top) );
// 	camera->getOrCreateStateSet()->setMode(
// 		GL_LIGHTING, osg::StateAttribute::OFF );
// 	return camera.release();
// }
// 
// //***********************************************************
// //FUNCTION:
// osg::Camera* createRTTCamera( osg::Camera::BufferComponent
// 							 buffer, osg::Texture* tex, bool isAbsolute )
// {
// 	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
// 	camera->setClearColor( osg::Vec4() );
// 	camera->setClearMask(
// 		GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
// 	camera->setRenderTargetImplementation(
// 		osg::Camera::FRAME_BUFFER_OBJECT );
// 	camera->setRenderOrder( osg::Camera::PRE_RENDER );
// 	if ( tex )
// 	{
// 		tex->setFilter( osg::Texture2D::MIN_FILTER,
// 			osg::Texture2D::LINEAR );
// 		tex->setFilter( osg::Texture2D::MAG_FILTER,
// 			osg::Texture2D::LINEAR );
// 		camera->setViewport( 0, 0, tex->getTextureWidth(),
// 			tex->getTextureHeight() );
// 		camera->attach( buffer, tex );
// 	}
// 	if ( isAbsolute )
// 	{
// 		camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
// 		camera->setProjectionMatrix( osg::Matrix::ortho2D(
// 			0.0, 1.0, 0.0, 1.0) );
// 		camera->setViewMatrix( osg::Matrix::identity() );
// 		camera->addChild( createScreenQuad(1.0f, 1.0f, 1.0) );
// 	}
// 	return camera.release();
// }
// 
// 
// int main( int argc, char** argv )
// {
//     osg::ArgumentParser arguments( &argc, argv );
//     osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles( arguments );
//     if ( !scene ) scene = osgDB::readNodeFile("cessna.osg");
//     
//     osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
//     tex2D->setTextureSize( 1024, 1024 );
//     tex2D->setInternalFormat( GL_DEPTH_COMPONENT24 );
//     tex2D->setSourceFormat( GL_DEPTH_COMPONENT );
//     tex2D->setSourceType( GL_FLOAT );
//     
//     osg::ref_ptr<osg::Camera> rttCamera = createRTTCamera(osg::Camera::DEPTH_BUFFER, tex2D.get(), false);
//     rttCamera->addChild( scene.get() );
//     
//     osg::ref_ptr<osg::Camera> hudCamera = createHUDCamera(0.0, 1.0, 0.0, 1.0);
//     hudCamera->addChild( createScreenQuad(0.5f, 1.0f,1.0) );
//     hudCamera->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex2D.get() );
//     
//     osg::ref_ptr<osg::Group> root = new osg::Group;
//     root->addChild( rttCamera.get() );
//     root->addChild( hudCamera.get() );
//     root->addChild( scene.get() );
//     
//     osgViewer::Viewer viewer;
//     viewer.getCamera()->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
//     viewer.setSceneData( root.get() );
//     return viewer.run();
// }


#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Shape>
#include <osg/Shapedrawable>
#include <osg/MatrixTransform>
#include <osg/Texture2D>

static char * vertexShader= {  
	"#version 400 \n"  
	"layout (location=0) in vec3 VertexPosition;\n"
	"layout (location=1) in vec2 TexCorrd;\n"	
	"uniform mat4 MVP;"	
	"out vec2 Corrd;\n"  
	"void main()\n"  
	"{\n"
	"   Corrd = TexCorrd;"
	"   gl_Position = MVP * vec4(VertexPosition,1.0);\n"  
	"}\n"  
};

static char * fragShader ={  
	"#version 400 \n" 
	"uniform sampler2D sampler2d_0;\n"
	"uniform sampler2D sampler2d_1;\n"
	"layout (location=0) out vec4 FragColor;\n"  
	"in vec2 Corrd;\n"  
	"void main() {\n"
	"   vec4 tmpColor = texture(sampler2d_0, Corrd);"
	"   if(Corrd[1] > 0.5 ) \n"
	"		   tmpColor = texture(sampler2d_1, Corrd); \n"
	"   FragColor = tmpColor ;\n"  
	"}\n"
};

class MVPCallback: public osg::Uniform::Callback  
{
public:  
	MVPCallback(osg::Camera * camera):mCamera(camera){  
	}  
	virtual void operator()( osg::Uniform* uniform, osg::NodeVisitor* nv){  
		osg::Matrix modelView = mCamera->getViewMatrix();  
		osg::Matrix projectM = mCamera->getProjectionMatrix();  
		uniform->set(modelView * projectM);  
	}  

private:  
	osg::Camera * mCamera;  
}; 

osg::Node * createNode()
{
	osg::Geode * geode = new osg::Geode;

	osg::Geometry * gm = new osg::Geometry;

	osg::Vec3Array * vertexArray = new osg::Vec3Array;
	vertexArray->push_back(osg::Vec3(0,0,0));
	vertexArray->push_back(osg::Vec3(1,0,0));
	vertexArray->push_back(osg::Vec3(1,0,1));
	vertexArray->push_back(osg::Vec3(0,0,1));
	gm->setVertexArray(vertexArray);

	osg::Vec3Array * normalArray = new osg::Vec3Array;
	normalArray->push_back(osg::Vec3(0,-1,0));
	gm->setNormalArray(normalArray);
	gm->setNormalBinding(osg::Geometry::BIND_OVERALL);


	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(1.0f,0.0f);
	(*texcoords)[2].set(1.0f,1.0f);
	(*texcoords)[3].set(0.0f,1.0f);
	gm->setTexCoordArray(0,texcoords);


	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,vertexArray->size()));
	geode->addDrawable(gm);

	osg::StateSet* stateset = gm->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osg::Texture2D * texture0 = new osg::Texture2D;
	texture0->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
	texture0->setImage(osgDB::readImageFile("../OSGData/Images/forestWall.png"));	
	stateset->setTextureAttributeAndModes(0,texture0,osg::StateAttribute::ON);


	osg::Texture2D * texture1 = new osg::Texture2D;
	texture1->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
	texture1->setImage(osgDB::readImageFile("../OSGData/Images/purpleFlowers.png"));
	stateset->setTextureAttributeAndModes(1,texture1,osg::StateAttribute::ON);

	gm->setVertexAttribArray(0,vertexArray);  
	gm->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX);

	gm->setVertexAttribArray(1,texcoords);       
	gm->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);

	return geode;		
}
int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer  = new osgViewer::Viewer;

	osg::Group * root = new osg::Group;

	osg::Node * node = createNode();
	root->addChild(node);	

	osg::StateSet * ss = node->getOrCreateStateSet();

	osg::Program * program = new osg::Program;

	osg::Shader * vS = new osg::Shader(osg::Shader::FRAGMENT,fragShader);
	osg::Shader * fS = new osg::Shader(osg::Shader::VERTEX,vertexShader);
	program->addShader(vS);  
	program->addShader(fS);  
	ss->setAttributeAndModes(program,osg::StateAttribute::ON);

	program->addBindFragDataLocation("VertexPosition",0);
	program->addBindFragDataLocation("TexCorrd",1);



	osg::Uniform* MVPUniform = new osg::Uniform( "MVP",osg::Matrix());
	MVPUniform->setUpdateCallback(new MVPCallback(viewer->getCamera()));
	ss->addUniform(MVPUniform);
	osg::Uniform* sample0 = new osg::Uniform("sampler2d_0",0);
	ss->addUniform(sample0);

	osg::Uniform* sample1 = new osg::Uniform("sampler2d_1",1);
	ss->addUniform(sample1);

	viewer->realize();
	viewer->setSceneData(root);
	viewer->setUpViewInWindow(35, 35, 1024, 800);


	return viewer->run();
}