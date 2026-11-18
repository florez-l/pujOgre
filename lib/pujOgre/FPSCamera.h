// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__FPSCamera__h__
#define __pujOgre__FPSCamera__h__

#include <pujOgre/Export.h>

#include <Ogre.h>
#include <OgreInput.h>

namespace pujOgre
{
  /**
   * First-person camera that walks on a floor node's local XZ plane.
   *
   * The floor node may be moved, turned and scaled (also non-uniformly,
   * and also at run time): the player follows its position and
   * orientation and walks over its scaled size, but keeps its own size.
   * Eye height, speeds, jump and edge margin are in world units, and the
   * camera (and anything attached to it) is never stretched by the floor's
   * scale.
   */
  class pujOgre_EXPORT FPSCamera
    : public OgreBites::InputListener
  {
  public:
    FPSCamera( Ogre::Camera* camera, Ogre::SceneNode* floor = nullptr );
    virtual ~FPSCamera( ) override;

    void setCamera( Ogre::Camera* camera, Ogre::SceneNode* floor = nullptr );

    /// (x, z): world units from the floor's origin, along its axes
    void setPosition(
      Ogre::Real x, Ogre::Real z, Ogre::Radian yaw = Ogre::Radian( 0 )
      );

    /// Re-reads the floor's size from the entities attached to it
    void refreshFloorBounds( );

    Ogre::Camera* getCamera( ) const;
    Ogre::SceneNode* getFloor( ) const;
    Ogre::SceneNode* getBodyNode( ) const;
    Ogre::SceneNode* getHeadNode( ) const;

    void setEyeHeight( Ogre::Real h );
    void setWalkSpeed( Ogre::Real speed );
    void setRunSpeed( Ogre::Real speed );
    void setTopSpeed( Ogre::Real speed );
    void setJumpSpeed( Ogre::Real speed );
    void setGravity( Ogre::Real g );
    void setMouseSensitivity( Ogre::Real degreesPerPixel );
    void setEdgeMargin( Ogre::Real margin );
    void setEnabled( bool enabled );
    void manualStop( );

    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
    virtual bool keyReleased( const OgreBites::KeyboardEvent& evt ) override;
    virtual bool mouseMoved( const OgreBites::MouseMotionEvent& evt ) override;
    virtual void frameRendered( const Ogre::FrameEvent& evt ) override;

  protected:
    bool _setKey( OgreBites::Keycode key, bool down );
    void _applyOrientation( );
    Ogre::Vector3 _floorScale( ) const;
    void _applyPosition( );
    static Ogre::AxisAlignedBox _floorBounds( Ogre::SceneNode* floor );
    void _clampToFloor( );

  protected:
    Ogre::Camera* m_Camera   { nullptr };
    Ogre::SceneNode* m_Floor { nullptr };
    Ogre::SceneNode* m_Body  { nullptr };
    Ogre::SceneNode* m_Head  { nullptr };

    /// Floor size, in the floor's local (unscaled) units
    Ogre::AxisAlignedBox m_Bounds;

    /// Player position, in world units along the floor's axes;
    /// y = eye height above the floor (plus the jump)
    Ogre::Vector3 m_Pos { Ogre::Vector3::ZERO };

    Ogre::Real m_EyeHeight        { 1.7 };
    Ogre::Real m_WalkSpeed        { 3.0 };
    Ogre::Real m_RunSpeed         { 7.0 };
    Ogre::Real m_JumpSpeed        { 4.5 };
    Ogre::Real m_Gravity          { 9.81 };
    Ogre::Real m_MouseSensitivity { 0.15 };
    Ogre::Real m_EdgeMargin       { 0.3 };

    Ogre::Radian
      m_Yaw   { 0 },
      m_Pitch { 0 };
    Ogre::Real m_VerticalSpeed { 0 };
    bool
      m_Enabled { true },
      m_Forward { false },
      m_Back    { false },
      m_Left    { false },
      m_Right   { false },
      m_Run     { false },
      m_Jump    { false };
  };
} // end namespace

#endif // __pujOgre__FPSCamera__h__

// eof - FPSCamera.h
