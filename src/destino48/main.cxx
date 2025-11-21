// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <OgreCameraMan.h>
#include <OgreRoot.h>
#include <pujOgre/BaseApplication.h>
#include <pujOgre/BaseListener.h>

#define _to_lower( _s )                                                 \
  std::transform(                                                       \
    _s.begin( ), _s.end( ), _s.begin( ),                                \
    []( unsigned char c )                                               \
    {                                                                   \
      return( static_cast< char >( std::tolower( c ) ) );               \
    }                                                                   \
    )

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

  void _load_sky( const boost::property_tree::ptree& sky )
    {
      const auto& sky_atr = sky.get_child_optional( "<xmlattr>" );
      std::string sky_typ = "dome";
      if( sky_atr )
        sky_typ = sky_atr->get< std::string >( "type", sky_typ );
      _to_lower( sky_typ );

      if( sky_typ == "dome" )
      {
        std::string sky_mat = "";
        double sky_cur = 10;
        double sky_til = 8;
        double sky_dis = 4000;
        int sky_xse = 16;
        int sky_yse = 16;
        int sky_ysk = -1;
        if( sky_atr )
        {
          sky_mat = sky_atr->get< std::string >( "material", sky_mat );
          sky_cur = sky_atr->get< double >( "curvature", sky_cur );
          sky_til = sky_atr->get< double >( "tiling", sky_til );
          sky_dis = sky_atr->get< double >( "distance", sky_dis );
          sky_xse = sky_atr->get< int >( "xsegments", sky_xse );
          sky_yse = sky_atr->get< int >( "ysegments", sky_yse );
          sky_ysk = sky_atr->get< int >( "ysegments_keep", sky_ysk );
        } // end if
        if( sky_mat != "" )
          this->m_SceneMgr->
            setSkyDome(
              true,
              sky_mat, sky_cur, sky_til, sky_dis,
              true,
              Ogre::Quaternion::IDENTITY,
              sky_xse, sky_yse, sky_ysk,
              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
              );
      }
      else
      {
        // TODO: no sky?
      } // end if
    }

  void _load_camera( const boost::property_tree::ptree& camera )
    {
      double nearClipDistance = 1e-3;
      bool aspectRatio = true;
      double position[ 3 ] = { 10, 10, 10 };
      double lookAt[ 3 ] = { 0, 10, 0 };
      double topSpeed = 10;
      bool fixedYaw = true;

      this->m_Camera = this->m_SceneMgr->createCamera( "MainCamera" );
      this->m_Camera->setNearClipDistance( nearClipDistance );
      this->m_Camera->setAutoAspectRatio( aspectRatio );

      auto node = this->m_SceneMgr->getRootSceneNode( )->createChildSceneNode( );
      node->setPosition( position[ 0 ], position[ 1 ], position[ 2 ] );
      node->lookAt( Ogre::Vector3( lookAt[ 0 ], lookAt[ 1 ], lookAt[ 2 ] ), Ogre::Node::TS_WORLD );
      node->attachObject( this->m_Camera );

      this->m_CameraMan = new OgreBites::CameraMan( node );
      this->m_CameraMan->setStyle( OgreBites::CS_FREELOOK );
      this->m_CameraMan->setTopSpeed( topSpeed );
      this->m_CameraMan->setFixedYaw( fixedYaw );
      this->addInputListener( this->m_CameraMan );
    }

  void _load_world( const boost::property_tree::ptree& world )
    {
      this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1.f, 1.f, 1.f ) );

      auto* sun = this->m_SceneMgr->createLight("Sun");
      sun->setType(Ogre::Light::LT_DIRECTIONAL);
      sun->setDiffuseColour(1.0f, 0.95f, 0.85f);

      auto* sun_node = this->m_SceneMgr->getRootSceneNode( )->createChildSceneNode( );
      sun_node->attachObject( sun );
      sun_node->setPosition( 0, 100, 0 );
      sun_node->setDirection( 0, -1, 0 );

      if( auto sky = world.get_child_optional( "sky" ) )
        this->_load_sky( *sky );
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
      if( auto c = xml_tree.get_child_optional( "camera" ) )
        this->_load_camera( *c );
    }

protected:
  Ogre::Camera*         m_Camera    { nullptr };
  OgreBites::CameraMan* m_CameraMan { nullptr };
};

int main( int argc, char** argv )
{
  auto path = std::filesystem::canonical( argv[ 0 ] ).parent_path( );
  destino48 d48( path / "destino48_resources.cfg" );
  d48.go( );
  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
