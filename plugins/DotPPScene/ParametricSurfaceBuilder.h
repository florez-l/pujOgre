// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __pujOgre__Plugin__ParametricSurfaceBuilder__h__
#define __pujOgre__Plugin__ParametricSurfaceBuilder__h__

#include <vtkSmartPointer.h>

// Forward declarations
class vtkPolyData;
namespace pugi
{
  class xml_document;
  class xml_node;
}

/**
 * Builds a vtkPolyData from a <parametricSurface> element of a .scenepp file,
 * using vtkParametricFunctionSource. Throws std::runtime_error on invalid
 * input.
 *
 *   <parametricSurface name="..." type="expression|torus|ellipsoid|superEllipsoid"
 *                      material="..." uResolution="50" vResolution="50"
 *                      textureCoordinates="true" flipWinding="false"
 *                      castShadows="true" visible="true">
 *       <uRange min="0" max="6.283185" join="true"/>     optional
 *       <vRange min="0" max="6.283185" join="true"/>     optional
 *       <param name="R" value="2"/>                      zero or more
 *       <point  x="..." y="..." z="..."/>                type="expression" only
 *       <normal x="..." y="..." z="..."/>                optional, type="expression" only
 *       <userData> ... </userData>                       optional
 *   </parametricSurface>
 */
vtkSmartPointer< vtkPolyData > buildParametricPolyData(
  const pugi::xml_node& xml
  );

/**
 * True if a <parametricSurface ref="..."> element changes the geometry of
 * the surface it refers to: it has <param>, <point>, <normal>, <uRange> or
 * <vRange> children, or a type, uResolution, vResolution, textureCoordinates
 * or flipWinding attribute.
 */
bool hasParametricOverrides( const pugi::xml_node& instance );

/**
 * Writes a copy of `definition` into `out` with the overrides of `instance`
 * applied:
 * - a <param> replaces the definition's <param> of the same name, or is added;
 * - <point>, <normal>, <uRange>, <vRange> replace the definition's element;
 * - the geometry attributes listed above, and material, replace the
 *   definition's.
 * Returns the merged <parametricSurface> element (the first child of `out`).
 */
pugi::xml_node mergeParametricSurface(
  const pugi::xml_node& definition,
  const pugi::xml_node& instance,
  pugi::xml_document& out
  );

#endif // __pujOgre__Plugin__ParametricSurfaceBuilder__h__

// eof - ParametricSurfaceBuilder.h
