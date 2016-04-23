#include <osg/Camera>
#include <osg/Group>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Texture2D>
#include <osgViewer/ViewerEventHandlers>
#include <osg/GraphicsContext>

#include <iostream>


using namespace std;


void CreateTexture(osgViewer::Viewer *viewer){
  if(!viewer)
    return;

  osg::GraphicsContext::WindowingSystemInterface *wsi= osg::GraphicsContext::getWindowingSystemInterface();
  if(!wsi){
    cout<<"wsi is NULL"<<endl;
    return;
  }
  unsigned int width,height;
  wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

  osg::ref_ptr<osg::GraphicsContext::Traits>traits=new osg::GraphicsContext::Traits;
  traits->x=0;
  traits->y=0;
  traits->width=width;
  traits->height=height;
  traits->windowDecoration=false;
  traits->doubleBuffer=true;
  traits->sharedContext=0;

  osg::ref_ptr<osg::GraphicsContext>gc=osg::GraphicsContext::createGraphicsContext(traits);
  if(!gc.valid()){
    cout<<"gc is NULL" <<endl;
    return;
  }
  osg::ref_ptr<osg::Camera>master=new osg::Camera;
  master->setGraphicsContext(gc);
  master->setViewport(0, 0, width, height);
 // viewer->addSlave(master.get());
 // viewer->setCamera(master);
  
  cout<<height<<"  ,  "<<width<<endl;

  osg::ref_ptr<osg::Camera> rttCamera = new osg::Camera;
  rttCamera->setRenderOrder(osg::Camera::PRE_RENDER);//×îÏÈäÖÈ¾
  rttCamera->setGraphicsContext(gc);
  rttCamera->setViewport(0, 0, width, height);
  rttCamera->addChild(osgDB::readNodeFile("../OSGData/cow.osg"));
//  viewer->addSlave(rttCamera,false);




  osg::ref_ptr<osg::Group>root=new osg::Group;
  osg::ref_ptr<osg::Geode>geode=new osg::Geode;
  root->addChild(geode);

  osg::ref_ptr<osg::Geometry>gm=new osg::Geometry;
  geode->addDrawable(gm);

  osg::ref_ptr<osg::Vec3Array>vertex=new osg::Vec3Array;
  vertex->push_back(osg::Vec3(-400,200,-300));
  vertex->push_back(osg::Vec3(400,200,-300));
  vertex->push_back(osg::Vec3(400,200,300));
  vertex->push_back(osg::Vec3(-400,200,300));
  gm->setVertexArray(vertex);

  osg::ref_ptr<osg::Vec3Array>norm=new osg::Vec3Array;
  norm->push_back(osg::Vec3(0,-1,0));
  gm->setNormalArray(norm);
  gm->setNormalBinding(osg::Geometry::BIND_OVERALL);

  osg::ref_ptr<osg::Vec2Array>coord=new osg::Vec2Array;
  coord->push_back(osg::Vec2(0,0));
  coord->push_back(osg::Vec2(1,0));
  coord->push_back(osg::Vec2(1,1));
  coord->push_back(osg::Vec2(0,1));
  gm->setTexCoordArray(0, coord);
  gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
  gm->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

  osg::ref_ptr<osg::Texture2D> t2d = new osg::Texture2D;
  osg::ref_ptr<osg::Image>image=osgDB::readImageFile("../OSGData/Images/blueFlowers.png");
  if(!image){
    cout<<"image is NULL"<<endl;
    return;
  }
  t2d->setImage(0,image);


  t2d->setInternalFormat(GL_RGBA);
//  viewer->getCamera()->attach(osg::Camera::COLOR_BUFFER, t2d);

  rttCamera->attach(osg::Camera::COLOR_BUFFER,t2d);

  gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, t2d,osg::StateAttribute::ON);

 // root->addChild(osgDB::readNodeFile("../OSGData/cow.osg"));
  root->addChild(rttCamera);
  //root->addChild(master);
  
  viewer->setSceneData(root);
  return;

}
int main(int argc, char *argv[])
{
  osg::ref_ptr<osgViewer::Viewer> viewer =new osgViewer::Viewer;
  CreateTexture(viewer);

 // viewer->addEventHandler(new osgViewer::WindowSizeHandler);
  //viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
  return viewer->run();
  return 0;
}
