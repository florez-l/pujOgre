// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/FPSCamera.h>

#include <algorithm>
#include <cmath>

// -------------------------------------------------------------------------
pujOgre::FPSCamera::
FPSCamera( Ogre::Camera* camera, Ogre::SceneNode* floor )
{
  this->setCamera( camera, floor );
}

// -------------------------------------------------------------------------
pujOgre::FPSCamera::
~FPSCamera( )
{
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setCamera( Ogre::Camera* camera, Ogre::SceneNode* floor )
{
  OgreAssert( camera, "FPSCamera needs a camera" );
  Ogre::SceneManager* scnMgr = camera->_getManager( );
  if( !floor )
    floor = scnMgr->getRootSceneNode( );

  // Current camera pose, in the floor's local space
  Ogre::Vector3 worldPos = Ogre::Vector3::ZERO;
  Ogre::Vector3 worldDir = Ogre::Vector3::NEGATIVE_UNIT_Z;
  if( Ogre::SceneNode* old = camera->getParentSceneNode( ) )
  {
    worldPos = old->_getDerivedPosition( );
    worldDir = old->_getDerivedOrientation( ) * Ogre::Vector3::NEGATIVE_UNIT_Z;
    camera->detachFromParent( );
  }
  Ogre::Vector3 pos = floor->convertWorldToLocalPosition( worldPos );
  Ogre::Vector3 dir
    =
    floor->convertWorldToLocalDirection( worldDir, false ).normalisedCopy( );

  if( this->m_Body )
  {
    scnMgr->destroySceneNode( this->m_Head );
    scnMgr->destroySceneNode( this->m_Body );
  } // end if
  this->m_Floor = floor;
  this->m_Body = floor->createChildSceneNode( );
  this->m_Head = this->m_Body->createChildSceneNode( );
  this->m_Head->attachObject( camera );
  this->m_Camera = camera;

  this->m_Yaw = Ogre::Math::ATan2( -dir.x, -dir.z );
  this->m_Pitch = Ogre::Math::ASin(
    std::clamp( dir.y, Ogre::Real( -1 ), Ogre::Real( 1 ) )
    );
  this->m_Pitch = std::clamp(
    this->m_Pitch,
    Ogre::Radian( Ogre::Degree( -89 ) ), Ogre::Radian( Ogre::Degree( 89 ) )
    );
  this->_applyOrientation( );

  this->m_Body->setPosition( pos.x, this->m_EyeHeight, pos.z );
  this->m_VerticalSpeed = 0;
  this->m_Bounds = this->_floorBounds( floor );
  this->_clampToFloor( );
}

// -------------------------------------------------------------------------
Ogre::Camera* pujOgre::FPSCamera::
getCamera( ) const
{
  return( this->m_Camera );
}

// -------------------------------------------------------------------------
Ogre::SceneNode* pujOgre::FPSCamera::
getFloor( ) const
{
  return( this->m_Floor );
}

// -------------------------------------------------------------------------
Ogre::SceneNode* pujOgre::FPSCamera::
getBodyNode( ) const
{
  return( this->m_Body );
}

// -------------------------------------------------------------------------
Ogre::SceneNode* pujOgre::FPSCamera::
getHeadNode( ) const
{
  return( this->m_Head );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setEyeHeight( Ogre::Real h )
{
  this->m_EyeHeight = h;
  Ogre::Vector3 p = this->m_Body->getPosition( );
  this->m_Body->setPosition( p.x, std::max( p.y, h ), p.z );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setWalkSpeed( Ogre::Real speed )
{
  this->m_WalkSpeed = speed;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setRunSpeed( Ogre::Real speed )
{
  this->m_RunSpeed = speed;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setTopSpeed( Ogre::Real speed )
{
  this->m_WalkSpeed = speed;
  this->m_RunSpeed = speed * 2.5f;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setJumpSpeed( Ogre::Real speed )
{
  this->m_JumpSpeed = speed;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setGravity( Ogre::Real g )
{
  this->m_Gravity = g;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setMouseSensitivity( Ogre::Real degreesPerPixel )
{
  this->m_MouseSensitivity = degreesPerPixel;
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setEdgeMargin( Ogre::Real margin )
{
  this->m_EdgeMargin = margin;
  this->_clampToFloor( );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setEnabled( bool enabled )
{
  this->m_Enabled = enabled;
  if( !enabled )
    this->manualStop( );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
manualStop( )
{
  this->m_Forward
    = this->m_Back
    = this->m_Left
    = this->m_Right
    = this->m_Run
    = this->m_Jump = false;
}

// -------------------------------------------------------------------------
bool pujOgre::FPSCamera::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  return( this->_setKey( evt.keysym.sym, true ) );
}

// -------------------------------------------------------------------------
bool pujOgre::FPSCamera::
keyReleased( const OgreBites::KeyboardEvent& evt )
{
  return( this->_setKey( evt.keysym.sym, false ) );
}

// -------------------------------------------------------------------------
bool pujOgre::FPSCamera::
mouseMoved( const OgreBites::MouseMotionEvent& evt )
{
  if( !this->m_Enabled )
    return( false );
  this->m_Yaw -= Ogre::Degree( evt.xrel * this->m_MouseSensitivity );
  this->m_Pitch -= Ogre::Degree( evt.yrel * this->m_MouseSensitivity );
  this->m_Pitch = std::clamp(
    this->m_Pitch,
    Ogre::Radian( Ogre::Degree( -89 ) ), Ogre::Radian( Ogre::Degree( 89 ) )
    );
  this->_applyOrientation( );
  return( true );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
frameRendered( const Ogre::FrameEvent& evt )
{
  if( !this->m_Enabled )
    return;
  const Ogre::Real dt = std::min( evt.timeSinceLastFrame, 0.1f );

  Ogre::Vector3 dir(
    Ogre::Real( this->m_Right - this->m_Left ), 0,
    Ogre::Real( this->m_Back - this->m_Forward )
    );
  if( dir.squaredLength( ) > 0 )
  {
    dir.normalise( );
    this->m_Body->translate(
      dir * ( ( this->m_Run )? this->m_RunSpeed: this->m_WalkSpeed ) * dt,
      Ogre::Node::TS_LOCAL
      );
  } // end if

  Ogre::Vector3 p = this->m_Body->getPosition( );
  if( this->m_Jump && p.y <= this->m_EyeHeight + 1e-4f )
    this->m_VerticalSpeed = this->m_JumpSpeed;
  this->m_VerticalSpeed -= this->m_Gravity * dt;
  p.y += this->m_VerticalSpeed * dt;
  if( p.y <= this->m_EyeHeight )
  {
    p.y = this->m_EyeHeight;
    this->m_VerticalSpeed = 0;
  } // end if
  this->m_Body->setPosition( p );
  this->_clampToFloor( );
}

// -------------------------------------------------------------------------
bool pujOgre::FPSCamera::
_setKey( OgreBites::Keycode key, bool down )
{
  if( !this->m_Enabled )
    return( false );
  switch ( key )
  {
  case 'w': case OgreBites::SDLK_UP:    this->m_Forward = down; return( true );
  case 's': case OgreBites::SDLK_DOWN:  this->m_Back = down;    return( true );
  case 'a': case OgreBites::SDLK_LEFT:  this->m_Left = down;    return( true );
  case 'd': case OgreBites::SDLK_RIGHT: this->m_Right = down;   return( true );
  case OgreBites::SDLK_LSHIFT:          this->m_Run = down;     return( true );
  case OgreBites::SDLK_SPACE:           this->m_Jump = down;    return( true );
  default: return( false );
  } // end switch
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
_applyOrientation( )
{
  this->m_Body->setOrientation(
    Ogre::Quaternion( this->m_Yaw, Ogre::Vector3::UNIT_Y )
    );
  this->m_Head->setOrientation(
    Ogre::Quaternion( this->m_Pitch, Ogre::Vector3::UNIT_X )
    );
}

// -------------------------------------------------------------------------
Ogre::AxisAlignedBox pujOgre::FPSCamera::
_floorBounds( Ogre::SceneNode* floor )
{
  Ogre::AxisAlignedBox box;
  for( Ogre::MovableObject* obj : floor->getAttachedObjects( ) )
    if( auto* ent = dynamic_cast< Ogre::Entity* >( obj ) )
      box.merge( ent->getBoundingBox( ) );
  return( box );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
_clampToFloor( )
{
  if(
    !this->m_Body
    ||
    this->m_Bounds.isNull( )
    ||
    this->m_Bounds.isInfinite( )
    )
    return;
  const Ogre::Vector3
    mn = this->m_Bounds.getMinimum( ),
    mx = this->m_Bounds.getMaximum( );
  const Ogre::Real m = this->m_EdgeMargin;
  Ogre::Vector3 p = this->m_Body->getPosition( );
  p.x = std::clamp(
    p.x, std::min( mn.x + m, mx.x - m ), std::max( mn.x + m, mx.x - m )
    );
  p.z = std::clamp(
    p.z, std::min( mn.z + m, mx.z - m ), std::max( mn.z + m, mx.z - m )
    );
  this->m_Body->setPosition( p );
}

// eof - FPSCamera.cxx
