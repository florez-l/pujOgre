// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__vtkParametricExpression__h__
#define __pujOgre__vtkParametricExpression__h__

#include <pujOgre/Export.h>

#include <map>
#include <string>

#include <vtkParametricFunction.h>
#include <vtkSmartPointer.h>

// Forward declarations
class vtkFunctionParser;

namespace pujOgre
{
  /**
   * A vtkParametricFunction whose surface is defined by text expressions
   * in u and v,
   * e.g.  x = "(R + r*cos(v))*cos(u)".
   *
   * - Point expressions x(u,v), y(u,v), z(u,v) are required.
   * - Normal expressions nx(u,v), ny(u,v), nz(u,v) are optional. If they
   *   are not given, the partial derivatives are computed numerically and
   *   vtkParametricFunctionSource  builds the normals from them.
   * - Named constants (e.g. R, r) can be used in every expression; "pi" is
   *   predefined.
   *
   * Expressions use vtkFunctionParser syntax: + - * / ^, abs, exp, ln,
   * log, sqrt, sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, ceil,
   * floor, min, max, sign, if.
   */
  class pujOgre_EXPORT vtkParametricExpression
    : public vtkParametricFunction
  {
  public:
    static vtkParametricExpression* New( );
    vtkTypeMacro( vtkParametricExpression, vtkParametricFunction );

    virtual int GetDimension( ) override;

    void SetPointExpressions(
      const std::string& x, const std::string& y, const std::string& z
      );
    void SetNormalExpressions(
      const std::string& nx, const std::string& ny, const std::string& nz
      );
    void SetConstant( const std::string& name, double value );

    void Compile();

    virtual void Evaluate(
      double uvw[ 3 ], double Pt[ 3 ], double Duvw[ 9 ]
      ) override;
    virtual double EvaluateScalar( double*, double*, double* ) override;

  protected:
    vtkParametricExpression( );
    virtual ~vtkParametricExpression( ) override;

    void _Point( double u, double v, double P[ 3 ] );
    static double _Scalar( vtkFunctionParser* parser, double u, double v );

  private:
    vtkParametricExpression( const vtkParametricExpression& ) = delete;
    void operator=( const vtkParametricExpression& ) = delete;

  protected:
    std::string m_PointExpr[ 3 ];
    std::string m_NormalExpr[ 3 ];
    bool m_HasNormals = false;
    bool m_Compiled = false;
    std::map< std::string, double > m_Constants;

    vtkSmartPointer< vtkFunctionParser > m_Point[ 3 ];
    vtkSmartPointer< vtkFunctionParser > m_Normal[ 3 ];
  };
} // end namespace
#endif // __pujOgre__vtkParametricExpression__h__

// eof - vtkParametricExpression.h
