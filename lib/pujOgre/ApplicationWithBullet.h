// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__ApplicationWithBullet__h__
#define __pujOgre__ApplicationWithBullet__h__

#include <pujOgre/BaseApplication.h>
#include <pujOgre/BulletModel.h>

namespace pujOgre
{
  /**
   */
  class pujOgre_EXPORT ApplicationWithBullet
    : public pujOgre::BaseApplication
  {
  public:
    using Self       = ApplicationWithBullet;
    using Superclass = pujOgre::BaseApplication;

  public:
    ApplicationWithBullet( int& ac, char** av );
    virtual ~ApplicationWithBullet( ) override;

    pujOgre::BulletModel* model( ) const;
    void setModel( pujOgre::BulletModel* m, bool keep_ownership = false );

  protected:
    virtual void _loadScene( ) override;

  protected:
    pujOgre::BulletModel* m_Model { nullptr };
    bool m_ModelOwned { true };
  };
} // end namespace

#endif // __pujOgre__ApplicationWithBullet__h__

// eof - ApplicationWithBullet.h
