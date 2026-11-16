// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__BulletModel__h__
#define __pujOgre__BulletModel__h__

#include <pujOgre/Export.h>

#include <memory>
#include <OgreBullet.h>

namespace pujOgre
{
  /**
   */
  class pujOgre_EXPORT BulletModel
  {
  public:
    using Self = BulletModel;

  public:
    BulletModel( );
    virtual ~BulletModel( );

    virtual void toggleSimulation( );

    virtual void build( Ogre::SceneNode* root );
    virtual void update( double t );

  protected:
    virtual void _configure(
      Ogre::Entity* e, const Ogre::UserObjectBindings& b
      );

  protected:
    std::unique_ptr< Ogre::Bullet::DynamicsWorld > m_DynWorld;
    // std::unique_ptr< Ogre::Bullet::DebugDrawer > m_DbgDraw;
    bool m_Simulating { false };
  };
} // end namespace

#endif // __pujOgre__BulletModel__h__

// eof - BulletModel.h
