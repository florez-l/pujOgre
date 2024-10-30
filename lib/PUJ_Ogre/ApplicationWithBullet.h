// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __PUJ_Ogre__ApplicationWithBullet__h__
#define __PUJ_Ogre__ApplicationWithBullet__h__

#include <PUJ_Ogre/Application.h>

namespace Ogre { namespace Bullet { class DynamicsWorld; } }

namespace PUJ_Ogre
{
  /**
   */
  class PUJ_Ogre_EXPORT ApplicationWithBullet
    : public PUJ_Ogre::Application
  {
  public:
    using Superclass  = PUJ_Ogre::Application;

  public:
    ApplicationWithBullet(
      const std::string& app_exec, const std::string& resources
      );
    virtual ~ApplicationWithBullet( ) override;

  protected:
    virtual void _initSceneManager( ) override;
    virtual void _simulateOneStep(
      const double& time,
      const int& maxSubSteps = 1,
      const double& fixedTimeStep = 1.0 / 60.0
      );

  protected:
    Ogre::Bullet::DynamicsWorld* m_DynamicsWorld { nullptr };
  };
} // end namespace

#endif // __PUJ_Ogre__Application__h__

// eof - $RCSfile$
