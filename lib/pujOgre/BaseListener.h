// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__BaseListener__h__
#define __pujOgre__BaseListener__h__

#include <pujOgre/Config.h>
#include <OgreApplicationContext.h>

namespace pujOgre
{
  /**
   */
  class pujOgre_EXPORT BaseListener
    : public OgreBites::InputListener
  {
  public:
    using Self = BaseListener;
    using Superclass = OgreBites::InputListener;

  public:
    BaseListener( );
    virtual ~BaseListener( ) = default;

    Ogre::Root* getRoot( ) const;
    void setRoot( Ogre::Root* root );

    /* Overridable methods
       void frameRendered( const Ogre::FrameEvent& evt );
       bool keyPressed( const OgreBites::KeyboardEvent& evt );
       bool keyReleased( const OgreBites::KeyboardEvent& evt );
       bool mouseMoved( const OgreBites::MouseMotionEvent& evt );
       bool mousePressed( const OgreBites::MouseButtonEvent& evt );
       bool mouseReleased( const OgreBites::MouseButtonEvent& evt );
       bool mouseWheelRolled( const OgreBites::MouseWheelEvent& evt );
       bool touchMoved( const OgreBites::TouchFingerEvent& evt );
       bool touchPressed( const OgreBites::TouchFingerEvent& evt );
       bool touchReleased( const OgreBites::TouchFingerEvent& evt );
    */

  protected:
    Ogre::Root* m_Root { nullptr };
  };
} // end namespace

#endif // __pujOgre__BaseListener__h__

// eof - $RCSfile$
