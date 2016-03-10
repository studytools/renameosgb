#include "renameosgb.hpp"

RenameVisitor::RenameVisitor(std::string file_path,std::string out_path)
      :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
      ,file_path_(file_path)
      ,out_path_(out_path)
  {}

void RenameVisitor::apply(osg::Node& node)
{
  if(node.getStateSet()) apply(*node.getStateSet());
  traverse(node);
}


void RenameVisitor::apply(osg::Geode& node)
{
  if(node.getStateSet()) apply(*node.getStateSet());

  for(unsigned int i=0; i<node.getNumDrawables(); ++i)
  {
    osg::Drawable* drawable = node.getDrawable(i);
    if(drawable && drawable->getStateSet())
      apply(*drawable->getStateSet());
  }

  traverse(node);
}

void RenameVisitor::apply(osg::PagedLOD& node)
{
  node.setDatabasePath("");
  for(unsigned int i = 0; i < node.getNumFileNames(); i++)
  {
    std::string filename = node.getFileName(i);
    if(filename.empty())
      continue;
    Renameosgb::read(file_path_+filename,out_path_);
    Renameosgb::deletechar(filename,"+");
    node.setFileName(i,filename);
  }
  traverse(node);

}

void RenameVisitor::apply(osg::StateSet& stateset)
{
  for(unsigned int i=0; i<stateset.getTextureAttributeList().size(); ++i)
  {
    osg::Texture* texture =
      dynamic_cast<osg::Texture*>(stateset.getTextureAttribute(i,
      osg::StateAttribute::TEXTURE));
    if(texture)
    {
      osg::Texture2D* texture2D = dynamic_cast<osg::Texture2D*>(texture);
      if(texture2D)
      {
        osg::Image*  img = texture2D->getImage();
        if(img)
        {
          std::string img_name = img->getFileName();
          Renameosgb::deletechar(img_name,"+");
          img->setFileName(img_name);
        }
      }
    }
  }

}


int Renameosgb::read(const std::string& full_file,
                const std::string& out_dir)
{
  std::string file_path;
  std::string file_name;
  size_t pos = full_file.rfind("\\");
  if(pos != std::string::npos)
  {
    file_path = full_file.substr(0,pos+1);
    file_name = full_file.substr(pos+1,full_file.size()-pos);
  }
  else
  {
    pos = full_file.rfind("/");
    if(pos != std::string::npos)
    {
      file_path = full_file.substr(0,pos+1);
      file_name = full_file.substr(pos+1,full_file.size()-pos);
    }
  }

  osgDB::ReaderWriter* rw =
    osgDB::Registry::instance()->getReaderWriterForExtension("osgb");

  osgDB::ReaderWriter::ReadResult rr = rw->readNode(full_file,
    new osgDB::Options());

  osg::Node* root = rr.takeNode();
  if(!root)
    return -1;

  RenameVisitor rename_visitor(file_path,out_dir);
  root->accept(rename_visitor);

  std::string re_file_name;
  if(!out_dir.empty())
  {
    char end_f = out_dir[out_dir.size()-1];
    if(end_f == '\\' || end_f == '/')
    {
      re_file_name = out_dir  + file_name;
    }
    else
    {
      re_file_name = out_dir  + "/" + file_name;
    }
  }

  deletechar(re_file_name,"+");

  rw->writeNode(*root,re_file_name,
  new osgDB::Options("WriteImageHint=IncludeData Compressor=zlib"));

  return 0;
}

void Renameosgb::deletechar(std::string& str,std::string c)
{
  std::size_t found = str.find_first_of(c);
  while(found != std::string::npos)
  {
    str.erase(found,1);
    found = str.find_first_of(c);
  }
}