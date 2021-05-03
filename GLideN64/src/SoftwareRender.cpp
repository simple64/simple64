#include <assert.h>
#include <algorithm>
#include <vector>
#include "DepthBufferRender/ClipPolygon.h"
#include "DepthBufferRender/DepthBufferRender.h"
#include "gSP.h"
#include "RSP.h"
#include "SoftwareRender.h"
#include "DepthBuffer.h"
#include "Config.h"

static
bool calcScreenCoordinates(const SPVertex** _vsrc, vertexclip * _vclip, u32 _numVertex, bool & _clockwise)
{
	const f32 ySign = GBI.isNegativeY() ? -1.0f : 1.0f;
	for (u32 i = 0; i < _numVertex; ++i) {
		const SPVertex& v = *_vsrc[i];

		if ((v.modify & MODIFY_XY) == 0) {
			_vclip[i].x = gSP.viewport.vtrans[0] + (v.x / v.w) * gSP.viewport.vscale[0];
			_vclip[i].y = gSP.viewport.vtrans[1] + (v.y / v.w) * gSP.viewport.vscale[1] * ySign;
		} else {
			_vclip[i].x = v.x;
			_vclip[i].y = v.y;
		}

		if ((v.modify & MODIFY_Z) == 0) {
			_vclip[i].z = (gSP.viewport.vtrans[2] + (v.z / v.w) * gSP.viewport.vscale[2]) * 32767.0f;
		} else {
			_vclip[i].z = v.z * 32767.0f;
		}
	}

	// Check culling
	const float x1 = _vclip[0].x - _vclip[1].x;
	const float y1 = _vclip[0].y - _vclip[1].y;
	const float x2 = _vclip[2].x - _vclip[1].x;
	const float y2 = _vclip[2].y - _vclip[1].y;

	_clockwise = ((x1 * y2 - y1 * x2) * ySign < 0.0f);

	const u32 cullMode = (gSP.geometryMode & G_CULL_BOTH);
	if (cullMode == G_CULL_BOTH && GBI.isCullBoth()) {
		// Cull front and back
		return false;
	} else if (cullMode == G_CULL_FRONT) {
		if (_clockwise) //clockwise, negative
			return false;
	} else if (cullMode == G_CULL_BACK) {
		if (!_clockwise) //counter-clockwise, positive
			return false;
	}

	return true;
}

inline
void clipTest(vertexclip & _vtx)
{
	_vtx.clip = 0;
	if (_vtx.x > gSP.viewport.width)
		_vtx.clip |= RIGHT;
	if (_vtx.x < 0)
		_vtx.clip |= LEFT;
	if (_vtx.y > gSP.viewport.height)
		_vtx.clip |= TOP;
	if (_vtx.y < 0)
		_vtx.clip |= BOT;
	if (_vtx.clip != 0)
		int t = 0;
}

bool calcScreenCoordinates(const SPVertex * _vsrc, vertexclip * _vclip, size_t _numVertex, bool _cull, bool & _clockwise)
{
	const f32 ySign = GBI.isNegativeY() ? -1.0f : 1.0f;
	for (u32 i = 0; i < _numVertex; ++i) {
		const SPVertex & v = _vsrc[i];

		if ((v.modify & MODIFY_XY) == 0) {
			_vclip[i].x = gSP.viewport.vtrans[0] + (v.x / v.w) * gSP.viewport.vscale[0];
			_vclip[i].y = gSP.viewport.vtrans[1] + (v.y / v.w) * gSP.viewport.vscale[1] * ySign;
		} else {
			_vclip[i].x = v.x;
			_vclip[i].y = v.y;
		}

		if ((v.modify & MODIFY_Z) == 0) {
			_vclip[i].z = (gSP.viewport.vtrans[2] + (v.z / v.w) * gSP.viewport.vscale[2]) * 32767.0f;
		} else {
			_vclip[i].z = v.z * 32767.0f;
		}

		clipTest(_vclip[i]);
	}

	if (!_cull)
		return true;

	// Check culling
	const float x1 = _vclip[0].x - _vclip[1].x;
	const float y1 = _vclip[0].y - _vclip[1].y;
	const float x2 = _vclip[2].x - _vclip[1].x;
	const float y2 = _vclip[2].y - _vclip[1].y;

	_clockwise = ((x1 * y2 - y1 * x2) * ySign < 0.0f);

	const u32 cullMode = (gSP.geometryMode & G_CULL_BOTH);
	if (cullMode == G_CULL_BOTH && GBI.isCullBoth()) {
		// Cull front and back
		return false;
	} else if (cullMode == G_CULL_FRONT) {
		if (_clockwise) //clockwise, negative
			return false;
	} else if (cullMode == G_CULL_BACK) {
		if (!_clockwise) //counter-clockwise, positive
			return false;
	}

	return true;
}

static
bool isClockwise(const SPVertex ** _vsrc)
{
	// Check culling
	const float x1 = _vsrc[0]->x - _vsrc[1]->x;
	const float y1 = _vsrc[0]->y - _vsrc[1]->y;
	const float x2 = _vsrc[2]->x - _vsrc[1]->x;
	const float y2 = _vsrc[2]->y - _vsrc[1]->y;

	return (x1*y2 - y1 * x2) >= 0.0f;
}


inline
int floatToFixed16(double _v)
{
	return static_cast<int>(_v * 65536.0);
}

static
int calcDzDx(const vertexclip* _v)
{
	double X0 = _v[0].x;
	double Y0 = _v[0].y;
	double X1 = _v[1].x;
	double Y1 = _v[1].y;
	double X2 = _v[2].x;
	double Y2 = _v[2].y;
	double diffy_02 = Y0 - Y2;
	double diffy_12 = Y1 - Y2;
	double diffx_02 = X0 - X2;
	double diffx_12 = X1 - X2;

	double denom = (diffx_02 * diffy_12 - diffx_12 * diffy_02);
	if(denom*denom > 0.0) {
		double diffz_02 = _v[0].z - _v[2].z;
		double diffz_12 = _v[1].z - _v[2].z;
		double fdzdx = (diffz_02 * diffy_12 - diffz_12 * diffy_02) / denom;
		return floatToFixed16(fdzdx);
	}
	return 0;
}

static
int calcDzDx(const SPVertex ** _v)
{
	double X0 = _v[0]->x;
	double Y0 = _v[0]->y;
	double X1 = _v[1]->x;
	double Y1 = _v[1]->y;
	double X2 = _v[2]->x;
	double Y2 = _v[2]->y;
	double diffy_02 = Y0 - Y2;
	double diffy_12 = Y1 - Y2;
	double diffx_02 = X0 - X2;
	double diffx_12 = X1 - X2;

	double denom = (diffx_02 * diffy_12 - diffx_12 * diffy_02);
	if (denom*denom > 0.0) {
		double diffz_02 = _v[0]->z - _v[2]->z;
		double diffz_12 = _v[1]->z - _v[2]->z;
		double fdzdx = (diffz_02 * diffy_12 - diffz_12 * diffy_02) / denom;
		return floatToFixed16(fdzdx);
	}
	return 0;
}

static
int calcDzDx2(const SPVertex ** _vsrc)
{
	const SPVertex * v = _vsrc[0];
	double X0 = gSP.viewport.vtrans[0] + (v->x / v->w) * gSP.viewport.vscale[0];
	double Y0 = gSP.viewport.vtrans[1] + (v->y / v->w) * -gSP.viewport.vscale[1];
	double Z0 = (gSP.viewport.vtrans[2] + (v->z / v->w) * gSP.viewport.vscale[2]) * 32767.0f;
	v = _vsrc[1];
	double X1 = gSP.viewport.vtrans[0] + (v->x / v->w) * gSP.viewport.vscale[0];
	double Y1 = gSP.viewport.vtrans[1] + (v->y / v->w) * -gSP.viewport.vscale[1];
	double Z1 = (gSP.viewport.vtrans[2] + (v->z / v->w) * gSP.viewport.vscale[2]) * 32767.0f;
	v = _vsrc[2];
	double X2 = gSP.viewport.vtrans[0] + (v->x / v->w) * gSP.viewport.vscale[0];
	double Y2 = gSP.viewport.vtrans[1] + (v->y / v->w) * -gSP.viewport.vscale[1];
	double Z2 = (gSP.viewport.vtrans[2] + (v->z / v->w) * gSP.viewport.vscale[2]) * 32767.0f;
	double diffy_02 = Y0 - Y2;
	double diffy_12 = Y1 - Y2;
	double diffx_02 = X0 - X2;
	double diffx_12 = X1 - X2;

	double denom = (diffx_02 * diffy_12 - diffx_12 * diffy_02);
	if (denom*denom > 0.0) {
		double diffz_02 = Z0 - Z2;
		double diffz_12 = Z1 - Z2;
		double fdzdx = (diffz_02 * diffy_12 - diffz_12 * diffy_02) / denom;
		return floatToFixed16(fdzdx);
	}
	return 0;
}

f32 renderScreenSpaceTriangles(const SPVertex *_pVertices, u32 _numElements)
{
	vertexi vdraw[3];
	const SPVertex * vsrc[3];
	f32 maxY = 0.0f;
	const bool needResterise = (depthBufferList().getCurrent() != nullptr &&
		config.frameBufferEmulation.copyDepthToRDRAM == Config::cdSoftwareRender &&
		gDP.otherMode.depthUpdate != 0);

	for (u32 i = 0; i < _numElements; i += 3) {
		for (u32 j = 0; j < 3; ++j) {
			vsrc[j] = &_pVertices[i + j];
		}

		if (isClockwise(vsrc)) {
			for (int k = 0; k < 3; ++k) {
				maxY = std::max(maxY, vsrc[k]->y);
				vdraw[k].x = floatToFixed16(vsrc[k]->x);
				vdraw[k].y = floatToFixed16(vsrc[k]->y);
				vdraw[k].z = floatToFixed16(vsrc[k]->z);
			}
		} else {
			for (int k = 0; k < 3; ++k) {
				const u32 idx = 3 - k - 1;
				maxY = std::max(maxY, vsrc[idx]->y);
				vdraw[k].x = floatToFixed16(vsrc[idx]->x);
				vdraw[k].y = floatToFixed16(vsrc[idx]->y);
				vdraw[k].z = floatToFixed16(vsrc[idx]->z);
			}
		}

		//Current depth buffer can be null if we are loading from a save state
		if (needResterise) {
			const int dzdx = calcDzDx(vsrc);
			Rasterize(vdraw, 3, dzdx);
		}
	}
	return maxY;
}

f32 renderTriangles(const SPVertex * _pVertices, const u16 * _pElements, u32 _numElements)
{
	vertexclip vclip[16];
	vertexi vdraw[12];
	const SPVertex * vsrc[4];
	SPVertex vdata[6];
	f32 maxY = 0.0f;
	//Current depth buffer can be null if we are loading from a save state
	const bool needResterise = (depthBufferList().getCurrent() != nullptr &&
		config.frameBufferEmulation.copyDepthToRDRAM == Config::cdSoftwareRender &&
		gDP.otherMode.depthUpdate != 0);

	for (u32 i = 0; i < _numElements; i += 3) {
		u32 orbits = 0;
		if (_pElements != nullptr) {
			for (u32 j = 0; j < 3; ++j) {
				vsrc[j] = &_pVertices[_pElements[i + j]];
				orbits |= vsrc[j]->clip;
			}
		} else {
			for (u32 j = 0; j < 3; ++j) {
				vsrc[j] = &_pVertices[i + j];
				orbits |= vsrc[j]->clip;
			}
		}
		vsrc[3] = vsrc[0];

		u32 numVertex = clipW(vsrc, vdata);

		const bool wClipped = (orbits & CLIP_W) != 0;
		bool clockwise = true;
		if (!calcScreenCoordinates(vdata, vclip, numVertex, !wClipped, clockwise))
			continue;

		if (orbits == 0) {
			assert(numVertex == 3);
			if (clockwise) {
				for (int k = 0; k < 3; ++k) {
					maxY = std::max(maxY, vclip[k].y);
					vdraw[k].x = floatToFixed16(vclip[k].x);
					vdraw[k].y = floatToFixed16(vclip[k].y);
					vdraw[k].z = floatToFixed16(vclip[k].z);
				}
			} else {
				for (int k = 0; k < 3; ++k) {
					const u32 idx = 3 - k - 1;
					maxY = std::max(maxY, vclip[idx].y);
					vdraw[k].x = floatToFixed16(vclip[idx].x);
					vdraw[k].y = floatToFixed16(vclip[idx].y);
					vdraw[k].z = floatToFixed16(vclip[idx].z);
				}
			}
		} else {
			vertexclip ** vtx;
			numVertex = clipInScreenSpace(&vtx, vclip, numVertex);
			if (numVertex < 3)
				continue;

			if (clockwise) {
				for (u32 k = 0; k < numVertex; ++k) {
					maxY = std::max(maxY, vtx[k]->y);
					vdraw[k].x = floatToFixed16(vtx[k]->x);
					vdraw[k].y = floatToFixed16(vtx[k]->y);
					vdraw[k].z = floatToFixed16(vtx[k]->z);
				}
			} else {
				for (u32 k = 0; k < numVertex; ++k) {
					const u32 idx = numVertex - k - 1;
					maxY = std::max(maxY, vtx[idx]->y);
					vdraw[k].x = floatToFixed16(vtx[idx]->x);
					vdraw[k].y = floatToFixed16(vtx[idx]->y);
					vdraw[k].z = floatToFixed16(vtx[idx]->z);
				}
			}
		}

		if (needResterise) {
			const int dzdx = !wClipped ? calcDzDx(vclip) : calcDzDx2(vsrc);
			Rasterize(vdraw, numVertex, dzdx);
		}
	}
	return maxY;
}

f32 renderAndDrawTriangles(const SPVertex *_pVertices,
	const u16 *_pElements,
	u32 _numElements,
	bool _flatColors,
	GraphicsDrawer::Statistics & _statistics)
{
	f32 maxY = 0.0f;
	std::vector<SPVertex> vResult;
	vResult.reserve(_numElements * 3);

	//Current depth buffer can be null if we are loading from a save state
	const bool needResterise = (depthBufferList().getCurrent() != nullptr &&
		config.frameBufferEmulation.copyDepthToRDRAM == Config::cdSoftwareRender &&
		gDP.otherMode.depthUpdate != 0);

	auto rasterise = [&maxY](const vertexclip* pVtxClip, bool clockwise)
	{
		int dzdx = calcDzDx(pVtxClip);
		vertexi vdraw[3];
		for (u32 k = 0; k < 3; ++k) {
			const u32 idx = clockwise ? k : 3 - k - 1;
			maxY = std::max(maxY, pVtxClip[idx].y);
			vdraw[k].x = floatToFixed16(pVtxClip[idx].x);
			vdraw[k].y = floatToFixed16(pVtxClip[idx].y);
			vdraw[k].z = floatToFixed16(pVtxClip[idx].z);
		}
		Rasterize(vdraw, 3, dzdx);
	};

	for (u32 i = 0; i < _numElements; i += 3) {
		u32 orbits = 0;
		u32 modify = 0;
		const SPVertex* vsrc[3];
		for (u32 j = 0; j < 3; ++j) {
			vsrc[j] = _pElements != nullptr ? &_pVertices[_pElements[i + j]] : &_pVertices[i + j];
			orbits |= vsrc[j]->clip;
			modify |= vsrc[j]->modify & (MODIFY_XY | MODIFY_Z);
		}

		if (orbits == 0) {
			// No clipping is necessary.
			vertexclip vclip[3];
			bool clockwise = true;
			if (!calcScreenCoordinates(vsrc, vclip, 3, clockwise)) {
				// Culled
				_statistics.drawnTris--;
				_statistics.culledTris++;
				continue;
			}

			// Copy vertices to result
			for (u32 k = 0; k < 3; ++k) {
				vResult.push_back(*vsrc[k]);
				SPVertex& v = vResult.back();
				v.bc0 = (k % 3 == 0) ? 1.0f : 0.0f;
				v.bc1 = (k % 3 == 1) ? 1.0f : 0.0f;
			}

			if (needResterise)
				rasterise(vclip, clockwise);
			else {
				for (u32 k = 0; k < 3; ++k)
					maxY = std::max(maxY, vclip[k].y);
			}
		} else {
			assert(modify == 0);
			// No screen space coordinates, so use clipping in homogeneous space.
			SPVertex vCopy[3];
			for (u32 k = 0; k < 3; ++k) {
				SPVertex& v = vCopy[k];
				v = *vsrc[k];
				v.bc0 = (k % 3 == 0) ? 1.0f : 0.0f;
				v.bc1 = (k % 3 == 1) ? 1.0f : 0.0f;
			}
			auto prevNumVtx = vResult.size();
			clipInHomogeneousSpace(vCopy, vResult);
			const size_t numVertex = vResult.size() - prevNumVtx;
			if (numVertex == 0) {
				_statistics.drawnTris--;
				_statistics.clippedTris++;
				continue;
			}

			std::vector<vertexclip> vclip(numVertex);
			const bool cull = ((orbits & CLIP_W) == 0);
			bool clockwise = true;
			if (!calcScreenCoordinates(vResult.data() + prevNumVtx, vclip.data(), numVertex, cull, clockwise)) {
				vResult.resize(prevNumVtx);
				_statistics.drawnTris--;
				_statistics.culledTris++;
				continue;
			}

			if (needResterise) {
				for (size_t l = 0; l < numVertex; l += 3)
					rasterise(vclip.data() + l, clockwise);
			} else {
				for (const vertexclip& v : vclip)
					maxY = std::max(maxY, v.y);
			}
		}
	}

	if (!vResult.empty()) {
		vResult[0].HWLight = _pVertices[0].HWLight;
		graphics::Context::DrawTriangleParameters triParams;
		triParams.mode = graphics::drawmode::TRIANGLES;
		triParams.flatColors = _flatColors;
		triParams.combiner = currentCombiner();
		triParams.verticesCount = static_cast<u32>(vResult.size());
		triParams.vertices = vResult.data();
		gfxContext.drawTriangles(triParams);
	}

	return maxY;
}
