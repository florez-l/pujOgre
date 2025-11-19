// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/BaseListener.h>

// -------------------------------------------------------------------------
pujOgre::BaseListener::
BaseListener( )
  : Superclass( )
{
}

// -------------------------------------------------------------------------
Ogre::Root* pujOgre::BaseListener::
getRoot( ) const
{
  return( this->m_Root );
}

// -------------------------------------------------------------------------
void pujOgre::BaseListener::
setRoot( Ogre::Root* root )
{
  this->m_Root = root;
}

// eof - $RCSfile$
