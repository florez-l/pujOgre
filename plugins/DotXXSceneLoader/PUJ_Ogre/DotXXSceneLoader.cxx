// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <PUJ_Ogre/DotXXSceneLoader.h>
#include <Ogre.h>
#include <OgreBullet.h>
#include <pugixml.hpp>

// -------------------------------------------------------------------------
PUJ_Ogre::DotXXSceneLoader::
DotXXSceneLoader( )
{
}

// -------------------------------------------------------------------------
PUJ_Ogre::DotXXSceneLoader::
~DotXXSceneLoader( )
{
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
load(
  Ogre::DataStreamPtr& stream,
  const Ogre::String& groupName,
  Ogre::SceneNode* rootNode
  )
{
  this->m_sGroupName = groupName;
  this->m_SceneMgr = rootNode->getCreator( );

  pugi::xml_document XMLDoc;
  auto result
    =
    XMLDoc.load_buffer( stream->getAsString( ).c_str( ), stream->size( )
      );
  if( !result )
  {
    Ogre::LogManager::getSingleton( )
      .logError(
        "DotXXSceneLoader - " + Ogre::String( result.description( ) )
        );
    return;
  } // end if

  // Grab the scene node
  auto XMLRoot = XMLDoc.child( "scenexx" );

  // Validate the File
  if( !XMLRoot.attribute( "formatVersion" ) )
  {
    Ogre::LogManager::getSingleton( )
      .logError(
        "DotXXSceneLoader - Invalid .scene File. Missing <scene formatVersion='x.y' >"
        );
    return;
  } // end if

  // figure out where to attach any nodes we create
  this->m_AttachNode = rootNode;

  // Process the scene
  Ogre::String physics = Self::_attrib( XMLRoot, "physics", "off" );
  if( physics == "on" )
  {
    auto binds = rootNode->getUserObjectBindings( );
    auto dynworld = binds.getUserAny( "DynamicsWorld" );
    if( dynworld.has_value( ) )
      this->m_DynamicsWorld
        =
        Ogre::any_cast< Ogre::Bullet::DynamicsWorld* >( dynworld );
    else
      this->m_DynamicsWorld = nullptr;
  } // end if
  this->_process( XMLRoot );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
exportScene(
  Ogre::SceneNode* rootNode,
  const Ogre::String& outFileName
  )
{
}

// -------------------------------------------------------------------------
const Ogre::ColourValue& PUJ_Ogre::DotXXSceneLoader::
getBackgroundColour( ) const
{
  return( this->m_BackgroundColour );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_process( pugi::xml_node& XMLRoot )
{
  // Process the scene parameters
  Ogre::String version = Self::_attrib( XMLRoot, "formatVersion", "unknown" );
  Ogre::String physics = Self::_attrib( XMLRoot, "physics", "off" );
  Ogre::String message
    =
    "[DotXXSceneLoader] Parsing dotScene++ file with version "
    +
    version
    +
    " (physics=" + physics + ").";
  if( XMLRoot.attribute("sceneManager" ) )
    message
      +=
      ", scene manager "
      +
      Ogre::String( XMLRoot.attribute("sceneManager" ).value( ) );
  if( XMLRoot.attribute( "author" ) )
    message += ", author " + Ogre::String( XMLRoot.attribute( "author" ).value( ) );
  Ogre::LogManager::getSingleton( ).logMessage( message );

  // Process nodes
  if( auto e = XMLRoot.child( "nodes" ) )
    this->_nodes( e );

  // Process camera
  if( auto e = XMLRoot.child( "camera" ) )
    this->_camera( e );

  // Process colourAmbient
  if( auto e = XMLRoot.child( "ambientLight" ) )
    this->m_SceneMgr->setAmbientLight( Self::_colour( e, "colour" ) );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_nodes( pugi::xml_node& XMLNode )
{
  for( auto e: XMLNode.children( "node" ) )
    this->_node( e );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_node( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::String name = Self::_attrib( XMLNode, "name" );

  Ogre::SceneNode* n;
  if( p != 0 )
    n = p->createChildSceneNode( name );
  else
    n = this->m_AttachNode->createChildSceneNode( name );

  // Process transformations
  Ogre::Vector3 pos( 0, 0, 0 ), scale( 1, 1, 1 ), lookAt( 0, 0, 0 );
  Ogre::Quaternion rot( 1, 0, 0, 0 );
  if( auto e = XMLNode.child( "position" ) )
    pos = Self::_vector( e, "point", pos );
  if( auto e = XMLNode.child( "scale" ) )
    scale = Self::_vector( e, "scale", scale );
  if( auto e = XMLNode.child( "lookAt" ) )
    lookAt = Self::_vector( e, "point", lookAt );
  if( auto e = XMLNode.child( "orientation" ) )
    rot = Self::_quaternion( e, "quaternion", rot );
  n->setPosition( pos );
  n->setScale( scale );
  n->setOrientation( rot );
  n->lookAt( lookAt, Ogre::Node::TransformSpace::TS_WORLD );

  /* TODO
     if (auto pElement = XMLNode.child("lookTarget"))
     processLookTarget(pElement, pNode);
     if (auto pElement = XMLNode.child("trackTarget"))
     processTrackTarget(pElement, pNode);
  */

  // Process children
  for( auto e: XMLNode.children( "node" ) )   this->_node( e, n );
  for( auto e: XMLNode.children( "entity" ) ) this->_entity( e, n );
  for( auto e: XMLNode.children( "camera" ) ) this->_camera( e, n );
  for( auto e: XMLNode.children( "light" ) )  this->_light( e, n );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_entity( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::Real mass = Self::_real( XMLNode, "mass", 0 );
  Ogre::String collider = Self::_attrib( XMLNode, "collider", "bbox" );

  Ogre::Entity* ent = nullptr;
  if( auto e = XMLNode.child( "mesh" ) )
    ent = this->_mesh( e, p );
  if( auto e = XMLNode.child( "parametric2" ) )
    ent = this->_parametric2( e, p );
  if( auto e = XMLNode.child( "vtk" ) )
    ent = this->_vtk( e, p );

  Ogre::Bullet::ColliderType        ct = Ogre::Bullet::CT_BOX;
  if     ( collider == "sphere" )   ct = Ogre::Bullet::CT_SPHERE;
  else if( collider == "cylinder" ) ct = Ogre::Bullet::CT_CYLINDER;
  else if( collider == "capsule" )  ct = Ogre::Bullet::CT_CAPSULE;
  else if( collider == "mesh" )     ct = Ogre::Bullet::CT_TRIMESH;
  else if( collider == "hull" )     ct = Ogre::Bullet::CT_HULL;

  if( ent != nullptr && this->m_DynamicsWorld != nullptr )
  {
    btRigidBody* body = this->m_DynamicsWorld->addRigidBody( mass, ent, ct );
    ent->getUserObjectBindings( )
      .setUserAny( "btRigidBody", Ogre::Any( body ) );
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_camera( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::Degree fovy = Ogre::Degree( Self::_real( XMLNode, "fovy", 45 ) );
  Ogre::String proj
    =
    Self::_attrib( XMLNode, "projectionType", "perspective" );
  Ogre::String position = Self::_attrib( XMLNode, "position", "" );

  // Node
  Ogre::SceneNode* n = p;
  if( n == 0 )
    n = this->m_AttachNode->createChildSceneNode( "unnamed_camera" );

  // Create the camera
  Ogre::Camera* cam
    =
    this->m_SceneMgr->createCamera( n->getName( ) + "_camera" );
  n->attachObject( cam );

  // Set parameters
  cam->setFOVy( fovy );
  cam->setAutoAspectRatio( true );
  if( proj == "perspective" )
    cam->setProjectionType( Ogre::PT_PERSPECTIVE );
  else if( proj == "orthographic" )
    cam->setProjectionType( Ogre::PT_ORTHOGRAPHIC );
  if( auto e = XMLNode.child( "clipping" ) )
  {
    cam->setNearClipDistance( Self::_real( e, "near" ) );
    cam->setFarClipDistance( Self::_real( e, "far" ) );
  } // end if
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXSceneLoader::
_light( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  // Node
  Ogre::SceneNode* n = p;
  if( n == 0 )
    n = this->m_AttachNode->createChildSceneNode( "unnamed_light" );

  // Create the light
  Ogre::Light* l = this->m_SceneMgr->createLight( n->getName( ) + "_light" );
  n->attachObject( l );

  // Parameters
  Ogre::String s = Self::_attrib( XMLNode, "type", "point" );
  if      ( s == "point" )       l->setType( Ogre::Light::LT_POINT );
  else if ( s == "directional" ) l->setType( Ogre::Light::LT_DIRECTIONAL );
  else if ( s == "spot" )        l->setType( Ogre::Light::LT_SPOTLIGHT );
  else if ( s == "radPoint" )    l->setType( Ogre::Light::LT_POINT );
  else if ( s == "rect" )        l->setType( Ogre::Light::LT_RECTLIGHT );
  l->setVisible( Self::_bool( XMLNode, "visible", true ) );
  l->setCastShadows( Self::_bool( XMLNode, "castShadows", true ) );
  l->setPowerScale( Self::_real( XMLNode, "powerScale", 1 ) );

  // Colours
  if( auto e = XMLNode.child( "diffuse" ) )
    l->setDiffuseColour( Self::_colour( e, "colour" ) );
  if( auto e = XMLNode.child( "specular" ) )
    l->setSpecularColour( Self::_colour( e, "colour" ) );

  // Properties
  if( s != "directional" )
  {
    if( auto e = XMLNode.child( "range" ) )
      l->setSpotlightRange(
        Ogre::Radian( Self::_real( e, "inner" ) ),
        Ogre::Radian( Self::_real( e, "outer" ) ),
        Self::_real( e, "falloff", 1 )
        );

    if( auto e = XMLNode.child( "attenuation" ) )
      l->setAttenuation(
        Self::_real( e, "range" ),
        Self::_real( e, "constant" ),
        Self::_real( e, "linear" ),
        Self::_real( e, "quadratic" )
        );
  } // end if
  /* TODO
     if (s == "rect")
     {
     // Process lightSourceSize (?)
     if (auto pElement = XMLNode.child("lightSourceSize"))
     processLightSourceSize(pElement, l);
     }
  */
}

// -------------------------------------------------------------------------
Ogre::Entity* PUJ_Ogre::DotXXSceneLoader::
_mesh( pugi::xml_node& XMLNode, Ogre::SceneNode* p )
{
  Ogre::String filename = Self::_attrib( XMLNode, "filename" );
  Ogre::String material = Self::_attrib( XMLNode, "material" );
  bool castShadows = ( Self::_attrib( XMLNode, "castShadows", "on" ) == "on" );
  bool visible = ( Self::_attrib( XMLNode, "visible", "on" ) == "on" );

  // Node
  Ogre::SceneNode* n = p;
  if( n == 0 )
    n = this->m_AttachNode->createChildSceneNode( "unnamed_mesh" );

  // Load mesh
  Ogre::Entity* ent
    =
    this->m_SceneMgr->
    createEntity( n->getName( ) + "_mesh", filename, this->m_sGroupName );
  ent->setCastShadows( castShadows );
  ent->setVisible( visible );
  if( material != "" )
    ent->setMaterialName( material );
  n->attachObject( ent );

  return( ent );
}

// -------------------------------------------------------------------------
Ogre::String PUJ_Ogre::DotXXSceneLoader::
_attrib(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  const Ogre::String& defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
    return( anode.value( ) );
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
Ogre::Real PUJ_Ogre::DotXXSceneLoader::
_real(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  Ogre::Real defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
    return( Ogre::StringConverter::parseReal( anode.value( ) ) );
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
bool PUJ_Ogre::DotXXSceneLoader::
_bool(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  bool defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
    return( anode.as_bool( ) );
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
Ogre::ColourValue PUJ_Ogre::DotXXSceneLoader::
_colour(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  const Ogre::ColourValue& defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
  {
    float r, g, b, a;
    std::istringstream( anode.value( ) ) >> r >> g >> b >> a;
    return( Ogre::ColourValue( r, g, b, a ) );
  }
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
Ogre::Vector3 PUJ_Ogre::DotXXSceneLoader::
_vector(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  const Ogre::Vector3& defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
  {
    float x, y, z;
    std::istringstream( anode.value( ) ) >> x >> y >> z;
    return( Ogre::Vector3( x, y, z ) );
  }
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
Ogre::Quaternion PUJ_Ogre::DotXXSceneLoader::
_quaternion(
  const pugi::xml_node& XMLNode, const Ogre::String& attrib,
  const Ogre::Quaternion& defaultValue
  )
{
  if( auto anode = XMLNode.attribute( attrib.c_str( ) ) )
  {
    float w, x, y, z;
    std::istringstream( anode.value( ) ) >> w >> x >> y >> z;
    return( Ogre::Quaternion( w, x, y, z ) );
  }
  else
    return( defaultValue );
}

// -------------------------------------------------------------------------
const Ogre::String& PUJ_Ogre::DotXXScenePlugin::
getName( ) const
{
  static Ogre::String name = "DotXXScene Loader";
  return( name );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXScenePlugin::
install( )
{
}

namespace PUJ_Ogre
{
  /**
   */
  struct DotXXSceneCodec
    : public Ogre::Codec
  {
    virtual Ogre::String magicNumberToFileExt(
      const char* magicNumberPtr, size_t maxbytes
      ) const override
      {
        return( "" );
      }

    virtual Ogre::String getType( ) const override
      {
        return( "scenexx" );
      }

    virtual void decode(
      const Ogre::DataStreamPtr& stream,
      const Ogre::Any& output
      ) const override
      {
        Ogre::DataStreamPtr _stream( stream );
        PUJ_Ogre::DotXXSceneLoader loader;
        loader.load(
          _stream,
          Ogre::ResourceGroupManager::getSingleton( )
          .getWorldResourceGroupName( ),
          Ogre::any_cast< Ogre::SceneNode* >( output )
          );
      }

    virtual void encodeToFile(
      const Ogre::Any& input,
      const Ogre::String& outFileName
      ) const override
      {
        PUJ_Ogre::DotXXSceneLoader loader;
        loader.exportScene(
          Ogre::any_cast< Ogre::SceneNode* >( input ), outFileName
          );
      }
  };
} // end namespace

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXScenePlugin::
initialise( )
{
  this->m_Codec = new PUJ_Ogre::DotXXSceneCodec( );
  Ogre::Codec::registerCodec( this->m_Codec );
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXScenePlugin::
shutdown( )
{
  Ogre::Codec::unregisterCodec( this->m_Codec );
  delete this->m_Codec;
}

// -------------------------------------------------------------------------
void PUJ_Ogre::DotXXScenePlugin::
uninstall( )
{
}

#ifndef OGRE_STATIC_LIB
extern "C" Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT void dllStartPlugin( );
extern "C" Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT void dllStopPlugin( );

static PUJ_Ogre::DotXXScenePlugin my_dotxxloader_plugin;

extern "C" Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT void dllStartPlugin( )
{
  Ogre::Root::getSingleton( ).installPlugin( &my_dotxxloader_plugin );
}
extern "C" Plugin_PUJ_Ogre_DotXXSceneLoader_EXPORT void dllStopPlugin( )
{
  Ogre::Root::getSingleton( ).uninstallPlugin( &my_dotxxloader_plugin );
}
#endif

// eof - $RCSfile$
