#ifndef SOFTWARE_RENDER_H
#define SOFTWARE_RENDER_H

#include "gSP.h"
#include "GraphicsDrawer.h"
#include "Graphics/Context.h"

/* Software render triangles to N64 depth buffer
* Coordinates of vertices must be in screen space.
* No coordinate clipping applied.
* Return max vertex Y.
*/
f32 renderScreenSpaceTriangles(const SPVertex *_pVertices, u32 _numElements);

/* Software render triangles to N64 depth buffer
* Coordinates of vertices can be in screen space or in homogeneous space
* Return max vertex Y.
*/
f32 renderTriangles(const SPVertex *_pVertices, const u16 *_pElements, u32 _numElements);

/* Software render triangles to N64 depth buffer and draw them with GPU
* Software clipping is used before rendering and drawing.
* Return max vertex Y.
*/
f32 renderAndDrawTriangles(const SPVertex *_pVertices,
	const u16 *_pElements,
	u32 _numElements,
	bool _flatColors,
	GraphicsDrawer::Statistics & _statistics);

#endif // SOFTWARE_RENDER_H
