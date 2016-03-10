#ifndef _HS_GRAPHICS_GRAPHICS_RENAMEOSGB_HPP_
#define _HS_GRAPHICS_GRAPHICS_RENAMEOSGB_HPP_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/Image>
#include <osg/LOD>
#include <osg/PagedLOD>
#include <osg/Geometry>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/Texture3D>
#include <osg/NodeVisitor>
#include <osg/StateSet>
#include <osg/PrimitiveSet>

#include <OpenThreads/Atomic>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgDB/ReaderWriter>
#include <osgDB/DatabasePager>
#include <osgDB/Options>

#include <osgDB/WriteFile>

#include "osgDB/Serializer"

class RenameVisitor: public osg::NodeVisitor
{
public:
  RenameVisitor(std::string file_path,std::string out_path);

  virtual void apply(osg::Node& node);

  virtual void apply(osg::Geode& node);

  virtual void apply(osg::PagedLOD& node);

  virtual void apply(osg::StateSet& stateset);

private:
  std::string file_path_;
  std::string out_path_;

};

class Renameosgb
{
public:
  static int read(const std::string& full_file,const std::string& out_file);
  static void deletechar(std::string& str,std::string c);
protected:
private:
};

#endif

