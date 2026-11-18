// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/FPSCamera.h>

#include <algorithm>
#include <cmath>
#include <vector>

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
  std::vector< Ogre::Node* > followers;
  Ogre::SceneNode* old = camera->getParentSceneNode( );
  if( old )
  {
    worldPos = old->_getDerivedPosition( );
    worldDir = old->_getDerivedOrientation( ) * Ogre::Vector3::NEGATIVE_UNIT_Z;
    camera->detachFromParent( );
    followers.assign(
      old->getChildren( ).begin( ), old->getChildren( ).end( )
      );
    for( Ogre::Node* follower: followers )
      old->removeChild( follower );
  } // end if

  // Position in the floor's local (scaled) units; direction without scale
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

  // The floor's scale must not stretch the player, the camera, or anything
  // attached to them
  this->m_Body->setInheritScale( false );
  this->m_Head = this->m_Body->createChildSceneNode( );
  this->m_Head->attachObject( camera );
  this->m_Camera = camera;
  for( Ogre::Node* follower: followers )
    this->m_Head->addChild( follower );

  this->m_Yaw = Ogre::Math::ATan2( -dir.x, -dir.z );
  this->m_Pitch = Ogre::Math::ASin(
    std::clamp( dir.y, Ogre::Real( -1 ), Ogre::Real( 1 ) )
    );
  this->m_Pitch = std::clamp(
    this->m_Pitch,
    Ogre::Radian( Ogre::Degree( -89 ) ), Ogre::Radian( Ogre::Degree( 89 ) )
    );
  this->_applyOrientation( );

  // From the floor's local units to world units along the floor's axes
  const Ogre::Vector3 scale = this->_floorScale( );
  this->m_Pos =
    Ogre::Vector3( pos.x * scale.x, this->m_EyeHeight, pos.z * scale.z );
  this->m_VerticalSpeed = 0;
  this->m_Bounds = this->_floorBounds( floor );
  this->_clampToFloor( );
  this->_applyPosition( );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
setPosition( Ogre::Real x, Ogre::Real z, Ogre::Radian yaw )
{
  this->m_Pos = Ogre::Vector3( x, this->m_EyeHeight, z );
  this->m_VerticalSpeed = 0;
  this->m_Yaw = yaw;
  this->_applyOrientation( );
  this->_clampToFloor( );
  this->_applyPosition( );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
refreshFloorBounds( )
{
  this->m_Bounds = this->_floorBounds( this->m_Floor );
  this->_clampToFloor( );
  this->_applyPosition( );
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
  this->m_Pos.y = std::max( this->m_Pos.y, h );
  this->_applyPosition( );
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
  this->_applyPosition( );
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

  // Walk in the facing direction, flat on the floor, in world units
  Ogre::Vector3 dir(
    Ogre::Real( this->m_Right - this->m_Left ), 0,
    Ogre::Real( this->m_Back - this->m_Forward )
    );
  if( dir.squaredLength( ) > 0 )
  {
    dir.normalise( );
    this->m_Pos +=
      Ogre::Quaternion( this->m_Yaw, Ogre::Vector3::UNIT_Y )
      *
      dir
      *
      ( ( this->m_Run )? this->m_RunSpeed: this->m_WalkSpeed ) * dt;
  } // end if

  // Jump along the floor's up axis, falling back to eye height
  if( this->m_Jump && this->m_Pos.y <= this->m_EyeHeight + 1e-4f )
    this->m_VerticalSpeed = this->m_JumpSpeed;
  this->m_VerticalSpeed -= this->m_Gravity * dt;
  this->m_Pos.y += this->m_VerticalSpeed * dt;
  if( this->m_Pos.y <= this->m_EyeHeight )
  {
    this->m_Pos.y = this->m_EyeHeight;
    this->m_VerticalSpeed = 0;
  } // end if

  this->_clampToFloor( );

  // Every frame: follows the floor's scale, even if it changes
  this->_applyPosition( );
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
Ogre::Vector3 pujOgre::FPSCamera::
_floorScale( ) const
{
  // World scale of the floor (all its parents included); never 0
  Ogre::Vector3 s =
    ( this->m_Floor )?
    this->m_Floor->_getDerivedScale( ):
    Ogre::Vector3::UNIT_SCALE;
  for( int i = 0; i < 3; ++i )
    if( std::abs( s[ i ] ) < 1e-6f )
      s[ i ] = 1e-6f;
  return( s );
}

// -------------------------------------------------------------------------
void pujOgre::FPSCamera::
_applyPosition( )
{
  // Ogre always multiplies a child's position by its parent's scale (even
  // with inheritScale off), so divide by it to place the body in world units
  if( this->m_Body )
    this->m_Body->setPosition( this->m_Pos / this->_floorScale( ) );
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
  if( this->m_Bounds.isNull( ) || this->m_Bounds.isInfinite( ) )
    return;

  // Floor bounds in world units (min/max: a negative scale mirrors the box)
  const Ogre::Vector3 s = this->_floorScale( );
  const Ogre::Vector3
    a = this->m_Bounds.getMinimum( ) * s,
    b = this->m_Bounds.getMaximum( ) * s;
  const Ogre::Vector3
    mn( std::min( a.x, b.x ), 0, std::min( a.z, b.z ) ),
    mx( std::max( a.x, b.x ), 0, std::max( a.z, b.z ) );
  const Ogre::Real m = this->m_EdgeMargin;
  this->m_Pos.x = std::clamp(
    this->m_Pos.x, std::min( mn.x + m, mx.x - m ), std::max( mn.x + m, mx.x - m )
    );
  this->m_Pos.z = std::clamp(
    this->m_Pos.z, std::min( mn.z + m, mx.z - m ), std::max( mn.z + m, mx.z - m )
    );
}

// eof - FPSCamera.cxx
