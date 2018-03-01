#ifndef  TEXTUREVISITOR
#define  TEXTUREVISITOR

#include <string>

#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LOD>
#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgUtil/TangentSpaceGenerator>
#include <osg/MatrixTransform>
#include <osg/TextureRectangle>
#include <osgTerrain/Terrain>

#include <osg/StateAttribute>


class TextureVisitor : public osg::NodeVisitor { 
public: 

   TextureVisitor(); 

   TextureVisitor(const std::string &searchName) ;

      
   //void apply( osg::MatrixTransform &node );
   void apply(osg::Geode& geode);
   void apply( osg::Group &group );
   void apply(osg::Geometry& geom);
   //void apply(osg::Node &searchNode);
   
  
    
public: 

   std::string searchForName; 
   osg::Vec3 lightPos;
   osg::ref_ptr<osg::Group> group;
   osg::ref_ptr<osg::Uniform> lightUniform;
   bool parallaxmap;
  
}; 


#endif

