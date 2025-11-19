// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <filesystem>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <OgreRoot.h>
#include <pujOgre/BaseApplication.h>
#include <pujOgre/BaseListener.h>

/**
 */
class destino48Listener
  : public pujOgre::BaseListener
{
public:
  using Self       = destino48Listener;
  using Superclass = pujOgre::BaseListener;

public:
  destino48Listener( )
    : Superclass( )
    {
    }
  virtual ~destino48Listener( ) = default;

  virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override
    {
      if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
        this->m_Root->queueEndRendering( );
      return( true );
    }
};

/**
 */
class destino48
  : public pujOgre::BaseApplication
{
public:
  using Self       = destino48;
  using Superclass = pujOgre::BaseApplication;

public:
  destino48( const std::string& res_file )
    : Superclass( "Destino48!", res_file )
    {
      this->setListener( new destino48Listener( ) );
    }
  virtual ~destino48( ) = default;

protected:

  void _load_world( const boost::property_tree::ptree& world )
    {
      /* TODO
         std::string material;
         double curvature = 10;
         double tiling = 8;
         double distance = 4000;
         int xsegments = 16;
         int ysegments = 16;
         int ysegments_keep = -1;
      */
      auto sky = world.get_child_optional( "sky" );
      if( sky )
      {
        /* TODO
           std::string sky_type = sky->get< std::string >( "sky.type", "none" );
           std::cout << sky_type << std::endl;
        */
        for( const auto& t: *sky )
          std::cout << "----------------- " << t.first << std::endl;
      } // end if
      else
        std::cout << "error" << std::endl;
      /* TODO
         this->m_SceneMgr
         virtual void Ogre::SceneManager::setSkyDome     (       bool    enable,
         const String &  materialName,
         Real    curvature = 10,
         Real    tiling = 8,
         Real    distance = 4000,
         bool    drawFirst = true,
         const Quaternion &      orientation = Quaternion::IDENTITY,
         int     xsegments = 16,
         int     ysegments = 16,
         int     ysegments_keep = -1,
         const String &  groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME 
         )               
      */
    }

  virtual void _loadScene( ) override
    {
      Ogre::DataStreamPtr stream
        =
        Ogre::ResourceGroupManager::getSingleton( ).openResource(
          "destino48.xml",
          "General"
          );
      std::string xml_str = stream->getAsString( );
      std::istringstream xml( xml_str );
      stream->close( );

      boost::property_tree::ptree xml_tree;
      boost::property_tree::read_xml( xml, xml_tree );
      if( auto w = xml_tree.get_child_optional( "world" ) )
        this->_load_world( *w );
      std::exit( 1 );
    };
};

int main( int argc, char** argv )
{
  auto path = std::filesystem::canonical( argv[ 0 ] ).parent_path( );
  destino48 d48( path / "destino48_resources.cfg" );
  d48.go( );
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
