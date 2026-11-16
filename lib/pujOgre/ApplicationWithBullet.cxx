// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/ApplicationWithBullet.h>

// -------------------------------------------------------------------------
pujOgre::ApplicationWithBullet::
ApplicationWithBullet( int& ac, char** av )
  : Superclass( ac, av )
{
  this->m_Model = new BulletModel( );
  this->m_ModelOwned = true;
}

// -------------------------------------------------------------------------
pujOgre::ApplicationWithBullet::
~ApplicationWithBullet( )
{
  if( this->m_ModelOwned )
    delete this->m_Model;
}
 
// -------------------------------------------------------------------------
pujOgre::BulletModel* pujOgre::ApplicationWithBullet::
model( ) const
{
  return( this->m_Model );
}

// -------------------------------------------------------------------------
void pujOgre::ApplicationWithBullet::
setModel( pujOgre::BulletModel* m, bool keep_ownership )
{
  this->m_Model = m;
  this->m_ModelOwned = keep_ownership;
}

// -------------------------------------------------------------------------
void pujOgre::ApplicationWithBullet::
_loadScene( )
{
  if( this->m_Model != nullptr )
    this->m_Model->build(
      this->getRoot( )->getSceneManager( "scnMgr" )->getRootSceneNode( )
      );
}

// eof - ApplicationWithBullet.cxx
