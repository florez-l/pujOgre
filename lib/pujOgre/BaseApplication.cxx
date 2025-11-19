// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/BaseApplication.h>

#include <filesystem>
#include <OgreConfigFile.h>
#include <OgreRoot.h>
#include <pujOgre/BaseListener.h>

// -------------------------------------------------------------------------
pujOgre::BaseApplication::
BaseApplication( const std::string& title, const std::string& res_file )
  : Superclass( title ),
    m_ResourcesFile( res_file )
{
}

// -------------------------------------------------------------------------
pujOgre::BaseApplication::
~BaseApplication( )
{
  if( this->m_Listener != nullptr )
    delete this->m_Listener;
}

// -------------------------------------------------------------------------
OgreBites::InputListener* pujOgre::BaseApplication::
getListener( ) const
{
  return( this->m_Listener );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
setListener( OgreBites::InputListener* l )
{
  if( this->m_Listener != nullptr )
    delete this->m_Listener;
  this->m_Listener = l;
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
locateResources( )
{
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
loadResources( )
{
  auto path = std::filesystem::canonical( this->m_ResourcesFile ).parent_path( );

  this->enableShaderCache( );

  // Load all resources from given locations
  auto res_mgr = Ogre::ResourceGroupManager::getSingletonPtr( );

  Ogre::ConfigFile cf = Ogre::ConfigFile( );
  cf.loadDirect( this->m_ResourcesFile );

  auto settings = cf.getSettingsBySection( );
  for( auto s = settings.begin( ); s != settings.end( ); ++s )
    for( auto f = s->second.begin( ); f != s->second.end( ); ++f )
      res_mgr->addResourceLocation(
        std::filesystem::canonical( path / f->second ).string( ),
        f->first,
        s->first
        );
  try
  {
    res_mgr->initialiseAllResourceGroups( );
    res_mgr->loadResourceGroup( "General" );
  }
  catch( ... )
  {
    // Do nothing
  }
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
setup( )
{
  this->setup( "DefaultSceneManager" );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
setup( const std::string& mgr_type )
{
  this->Superclass::setup( );
  this->_initSceneManager( mgr_type );
  this->setWindowGrab( true );
  this->_loadScene( );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
go( )
{
  this->initApp( );
  this->getRoot( )->startRendering( );
  this->closeApp( );
}

// -------------------------------------------------------------------------
void pujOgre::BaseApplication::
_initSceneManager( const std::string& mgr_type )
{
  auto root = this->getRoot( );

  if( this->m_Listener == nullptr )
    this->m_Listener = new pujOgre::BaseListener( );
  auto* l = dynamic_cast< pujOgre::BaseListener* >( this->m_Listener );
  if( l != nullptr )
    l->setRoot( root );
  this->addInputListener( this->m_Listener );

  this->m_SceneMgr = root->createSceneManager( mgr_type );

  auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
  shadergen->addSceneManager( this->m_SceneMgr );
}

// eof - $RCSfile$
