// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <pujOgre/BulletModel.h>

#include <queue>

// -------------------------------------------------------------------------
pujOgre::BulletModel::
BulletModel( )
{
}

// -------------------------------------------------------------------------
pujOgre::BulletModel::
~BulletModel( )
{
}

// -------------------------------------------------------------------------
void pujOgre::BulletModel::
toggleSimulation( )
{
  this->m_Simulating = !( this->m_Simulating );
}

// -------------------------------------------------------------------------
void pujOgre::BulletModel::
build( Ogre::SceneNode* root )
{
  const Ogre::UserObjectBindings& sd = root->getUserObjectBindings( );

  // Gravity
  Ogre::Vector3 gravity( 0, -9.8, 0 );
  const Ogre::Any& gd = sd.getUserAny( "gravity" );
  if( gd.has_value( ) )
    gravity = Ogre::any_cast< Ogre::Vector3 >( gd );

  // Create dynamic world
  this->m_DynWorld = std::make_unique< Ogre::Bullet::DynamicsWorld>( gravity );

  // Physics world properties
  std::queue< Ogre::SceneNode* > q;
  q.push( root );
  while( q.size( ) > 0 )
  {
    Ogre::SceneNode* node = q.front( );
    q.pop( );
    if( node != nullptr )
    {
      // Configure each entity's physical properties
      for( Ogre::MovableObject* obj: node->getAttachedObjects( ) )
        this->_configure(
          dynamic_cast< Ogre::Entity* >( obj ),
          node->getUserObjectBindings( )
          );

      // Add children
      for( Ogre::Node* child: node->getChildren( ) )
        q.push( static_cast< Ogre::SceneNode* >( child ) );
    } // end if
  } // end while

  this->m_Simulating = false;
}

// -------------------------------------------------------------------------
void pujOgre::BulletModel::
update( double t )
{
  if( this->m_Simulating )
  {
    this->m_DynWorld->getBtWorld( )->stepSimulation( t, 10 );
  } // end if
}

// -------------------------------------------------------------------------
btRigidBody* pujOgre::BulletModel::
_configure( Ogre::Entity* e, const Ogre::UserObjectBindings& b )
{
  if( e == nullptr )
    return( nullptr );

  bool ok = true;
  Ogre::Bullet::ColliderType collider = Ogre::Bullet::CT_TRIMESH;
  const Ogre::Any& collider_d = b.getUserAny( "collider" );
  if( collider_d.has_value( ) )
  {
    Ogre::String str = Ogre::any_cast< Ogre::String >( collider_d );
    Ogre::StringUtil::toLowerCase( str );
    if     ( str ==      "box" ) collider = Ogre::Bullet::CT_BOX;
    else if( str ==  "capsule" ) collider = Ogre::Bullet::CT_CAPSULE;
    else if( str == "compound" ) collider = Ogre::Bullet::CT_COMPOUND;
    else if( str == "cylinder" ) collider = Ogre::Bullet::CT_CYLINDER;
    else if( str ==     "hull" ) collider = Ogre::Bullet::CT_HULL;
    else if( str ==   "sphere" ) collider = Ogre::Bullet::CT_SPHERE;
    else                         collider = Ogre::Bullet::CT_TRIMESH;
  }
  else
    ok = false;

  auto value = []( float& ret, const Ogre::Any& data ) -> bool
    {
      if( data.has_value( ) )
      {
        ret = Ogre::any_cast< Ogre::Real >( data );
        return( true );
      }
      else
        return( false );
    };

  float
    mass = 0,
    restitution = 5e-1,
    friction = 1e-1,
    rolling_friction = 1e-1,
    spinning_friction = 1e-1,
    linear_damping = 1e-1,
    angular_damping = 1e-1;

  ok |= value( mass, b.getUserAny( "mass" ) );
  ok |= value( restitution, b.getUserAny( "restitution" ) );
  ok |= value( friction, b.getUserAny( "friction" ) );
  ok |= value( rolling_friction, b.getUserAny( "rolling_friction" ) );
  ok |= value( spinning_friction, b.getUserAny( "spinning_friction" ) );
  ok |= value( linear_damping, b.getUserAny( "linear_damping" ) );
  ok |= value( angular_damping, b.getUserAny( "angular_damping" ) );

  if( ok )
  {
    btRigidBody* body = this->m_DynWorld->addRigidBody( mass, e, collider );
    body->setRestitution( restitution );
    body->setFriction( friction );
    body->setRollingFriction( rolling_friction );
    body->setSpinningFriction( spinning_friction );
    body->setDamping( linear_damping, angular_damping );
    return( body );
  }
  else
    return( nullptr );
}

// eof - BulletModel.cxx
