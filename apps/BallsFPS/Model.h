// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __BallsFPS__Model__h__
#define __BallsFPS__Model__h__

#include <pujOgre/BulletModel.h>

/**
 */
class Model
  : public pujOgre::BulletModel
{
public:
  using Self       = Model;
  using Superclass = pujOgre::BulletModel;

public:
  Model( );
  virtual ~Model( );
};

#endif // __BallsFPS__Model__h__

// eof - Model.h
