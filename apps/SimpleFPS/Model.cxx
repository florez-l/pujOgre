// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include "Model.h"
#include <cmath>
#include <queue>

// -------------------------------------------------------------------------
template< typename T >
bool getUserValue(
  const Ogre::UserObjectBindings& ud, const Ogre::String& key, T& out
  )
{
  const Ogre::Any& a = ud.getUserAny( key );
  if( const T* p = Ogre::any_cast< T >( &a ) )
  {
    out = *p;
    return( true );
  } // end if
  if( const Ogre::String* s = Ogre::any_cast< Ogre::String >( &a ) )
    return( Ogre::StringConverter::parse( *s, out ) );
  return( false );
}

// -------------------------------------------------------------------------
Model::
Model( )
{
}

// -------------------------------------------------------------------------
Model::
~Model( )
{
}

// -------------------------------------------------------------------------
void Model::
build( Ogre::SceneNode* root )
{
  const Ogre::UserObjectBindings& sd = root->getUserObjectBindings( );

  // Gravity
  Ogre::Vector3 gravity( 0, -9.8, 0 );
  const Ogre::Any& gd = sd.getUserAny( "gravity" );
  if( gd.has_value( ) )
  {
    double gx, gy, gz;
    std::istringstream( Ogre::any_cast< Ogre::String >( gd ) )
      >> gx >> gy >> gz;
    gravity = Ogre::Vector3( gx, gy, gz );
  } // end if

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
}

// -------------------------------------------------------------------------
void Model::
update( double t )
{
  this->m_DynWorld->getBtWorld( )->stepSimulation( t, 10 );
}

// -------------------------------------------------------------------------
#include <iostream>
void Model::
_configure( Ogre::Entity* e, const Ogre::UserObjectBindings& b )
{
  if( e == nullptr )
    return;

  auto value = []( const Ogre::Any& data, const float& def ) -> float
    {
      if( data.has_value( ) )
        return( Ogre::any_cast< Ogre::Real >( data ) );
      else
        return( def );
    };
  float mass = value( b.getUserAny( "mass" ), 0 );
  float restitution = value( b.getUserAny( "restitution" ), 5e-1 );
  float friction = value( b.getUserAny( "friction" ), 1e-1 );
  float rolling_friction = value( b.getUserAny( "rolling_friction" ), 1e-1 );
  float spinning_friction = value( b.getUserAny( "spinning_friction" ), 1e-1 );
  float linear_damping = value( b.getUserAny( "linear_damping" ), 1e-1 );
  float angular_damping = value( b.getUserAny( "angular_damping" ), 1e-1 );
  const Ogre::Any& collider_d = b.getUserAny( "collider" );
  Ogre::Bullet::ColliderType collider = Ogre::Bullet::CT_TRIMESH;
  if( collider_d.has_value( ) )
  {
    Ogre::String str = Ogre::any_cast< Ogre::String >( collider_d );
    Ogre::StringUtil::toLowerCase( str );
    std::cout << "***** " << str << std::endl;
    if     ( str ==      "box" ) collider = Ogre::Bullet::CT_BOX;
    else if( str ==  "capsule" ) collider = Ogre::Bullet::CT_CAPSULE;
    else if( str == "compound" ) collider = Ogre::Bullet::CT_COMPOUND;
    else if( str == "cylinder" ) collider = Ogre::Bullet::CT_CYLINDER;
    else if( str ==     "hull" ) collider = Ogre::Bullet::CT_HULL;
    else if( str ==   "sphere" ) collider = Ogre::Bullet::CT_SPHERE;
    else                         collider = Ogre::Bullet::CT_TRIMESH;
  } // end if
  std::cout << "***** " << collider << " " << Ogre::Bullet::CT_TRIMESH << std::endl;

  btRigidBody* body = this->m_DynWorld->addRigidBody( mass, e, collider );
  body->setRestitution( restitution );
  body->setFriction( friction );
  body->setRollingFriction( rolling_friction );
  body->setSpinningFriction( spinning_friction );
  body->setDamping( linear_damping, angular_damping );
}

// eof - Model.cxx
