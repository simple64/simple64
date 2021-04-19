
/*
 *      2D clipping of a triangle into a polygon using the clipping
 *      boundarys LeftClip, RightClip, TopClip and BotClip.
 *
 *      The code is based on Sutherland-Hodgman algorithm where we
 *      clip the polygon against each clipping boundary in turn.
 *
 *      Note! This code is far from optimal but it does what it is
 *            supposed to do.
 *
 *      This source is part of the fatmap2.txt document by
 *      Mats Byggmastar, mri@penti.sit.fi
 *      17.4.1997 Jakobstad, Finland
 *
 *      Companies with self respect are encouraged to contact me if
 *      any of this code is to be used as part of a commercial product.
 */

//****************************************************************
//
// Adopted for GLideN64 by Gonetz, Dec 2016
//
//****************************************************************


#include <assert.h>
#include "gSP.h"
#include "Combiner.h"
#include "ClipPolygon.h"
#include "DisplayWindow.h"

/*
 *      vbp is a pointer to a vertex array. The first 3 vertices in that
 *      array is our source vertices. The rest of the array will be used
 *      to hold new vertices created during clipping.
 *
 *      you can then access the new vertices using the *final variable
 *
 *      function returns the number of vertices in the resulting polygon
 */
unsigned int clipInScreenSpace(vertexclip *** final, vertexclip * vbp, unsigned int numVertices)
{
	const float LeftClip = gSP.viewport.x;
	const float RightClip = LeftClip + gSP.viewport.width;
	const float BotClip = gSP.viewport.y;
	const float TopClip = BotClip + gSP.viewport.height;
	graphics::CombinerProgram * pCurrCmb = currentCombiner();
	const bool clipColor = pCurrCmb->usesShade();
	const bool clipTexCoords = pCurrCmb->usesTexture();

	auto cliptestx = [LeftClip, RightClip](vertexclip * v)
	{
		int bits = 0;
		if (v->y < LeftClip)
			bits |= LEFT;
		if (v->y > RightClip)
			bits |= RIGHT;
		return bits;
	};

	auto cliptesty = [BotClip, TopClip](vertexclip * v)
	{
		int bits = 0;
		if (v->y < BotClip)
			bits |= BOT;
		if (v->y > TopClip)
			bits |= TOP;
		return bits;
	};

	auto clipVertex = [clipColor, clipTexCoords](vertexclip const* src1, vertexclip const* src2, vertexclip * dst, float a)
	{
		float ima = 1.0f - a;
#define doClip(V) dst->V = src1->V*ima + src2->V*a
		doClip(x);
		doClip(y);
		doClip(z);
#undef doClip
	};

	static vertexclip * vp1[12], * vp2[12];     // vertex ptr buffers
	vertexclip ** src = vp1;
	vertexclip ** dst = vp2;
	vertexclip ** tmp;

	for (unsigned int n = 0; n < numVertices; n++)
		vp1[n] = vbp + n;
	vp1[numVertices] = vbp + 0;

	vbp += numVertices;       // Next free vertex

	unsigned int dsti = 0;
	unsigned int max = numVertices;

	// right clip
	for (unsigned int n = 0; n < max; n++) {
		vertexclip * src1 = src[n];             // current vertex
		vertexclip * src2 = src[n + 1];           // next vertex
		if ((src1->clip & RIGHT) == VISIBLE) {
			dst[dsti++] = src1;                 // add clip vertex to list
			if ((src2->clip & RIGHT) == VISIBLE)
				continue;
		} else if ((src2->clip & RIGHT) != VISIBLE)
			continue;
		float a = (RightClip - src1->x) / (src2->x - src1->x);
		dst[dsti] = vbp++;                      // create new vertex
		clipVertex(src1, src2, dst[dsti], a);
		dst[dsti]->x = RightClip;
		dst[dsti]->clip = cliptesty(dst[dsti]);
		dsti++;
	}
	dst[dsti] = dst[0];
	tmp = src; src = dst; dst = tmp;            // swap src - dst buffers
	max = dsti;
	dsti = 0;

	// left clip
	for(unsigned int n = 0; n < max; n++) {
		vertexclip * src1 = src[n];             // current vertex
		vertexclip * src2 = src[n+1];           // next vertex
		if((src1->clip & LEFT) == VISIBLE) {
			dst[dsti++] = src1;                 // add clip vertex to list
			if((src2->clip & LEFT) == VISIBLE)
				continue;
		} else if((src2->clip & LEFT) != VISIBLE)
			continue;
		float a = (LeftClip - src1->x) / (src2->x - src1->x);
		dst[dsti] = vbp++;                      // create new vertex
		clipVertex(src1, src2, dst[dsti], a);
		dst[dsti]->x = LeftClip;
		dst[dsti]->clip = cliptesty(dst[dsti]);
		dsti++;
	}
	dst[dsti] = dst[0];
	tmp = src; src = dst; dst = tmp;            // Swap src - dst buffers
	max = dsti;
	dsti = 0;

	// top clip
	for(unsigned int n = 0; n < max; n++) {
		vertexclip * src1 = src[n];             // current vertex
		vertexclip * src2 = src[n+1];           // next vertex
		if((src1->clip & TOP) == VISIBLE) {
			dst[dsti++] = src1;                 // add clip vertex to list
			if((src2->clip & TOP) == VISIBLE)
				continue;
		} else if((src2->clip & TOP) != VISIBLE)
			continue;
		float a = (TopClip - src1->y) / (src2->y - src1->y);
		dst[dsti] = vbp++;                      // create new vertex
		clipVertex(src1, src2, dst[dsti], a);
		dst[dsti]->y = TopClip;
		dst[dsti]->clip = cliptestx(dst[dsti]);
		dsti++;
	}
	dst[dsti] = dst[0];
	tmp = src; src = dst; dst = tmp;            // swap src - dst buffers
	max = dsti;
	dsti = 0;

	// bot clip
	for(unsigned int n = 0; n < max; n++) {
		vertexclip * src1 = src[n];             // current vertex
		vertexclip * src2 = src[n+1];           // next vertex
		if((src1->clip & BOT) == VISIBLE) {
			dst[dsti++] = src1;                 // add clip vertex to list
			if((src2->clip & BOT) == VISIBLE)
				continue;
		} else if((src2->clip & BOT) != VISIBLE)
			continue;
		float a = (BotClip - src1->y) / (src2->y - src1->y);
		dst[dsti] = vbp++;                      // create new vertex
		clipVertex(src1, src2, dst[dsti], a);
		dst[dsti]->y = BotClip;
		dsti++;
	}

	*final = dst;

	return dsti;
}

unsigned int clipW(const SPVertex ** _vsrc, SPVertex * _vdst)
{
	graphics::CombinerProgram * pCurrCmb = currentCombiner();
	const bool clipColor = pCurrCmb->usesShade();
	const bool clipTexCoords = pCurrCmb->usesTexture();

	auto copyVertex = [](SPVertex & _dst, const SPVertex * _src)
	{
		_dst.x = _src->x;
		_dst.y = _src->y;
		_dst.z = _src->z;
		_dst.w = _src->w;
		_dst.modify = _src->modify;
	};

	unsigned int dsti = 0;
	for (int n = 0; n < 3; ++n) {
		const SPVertex * src1 = _vsrc[n];     // current vertex
		const SPVertex * src2 = _vsrc[n + 1]; // next vertex
		if (src1->w >= 0.01f) {
			copyVertex(_vdst[dsti++], src1);  // add visible vertex to list
			if (src2->w >= 0.01f)
				continue;
		} else if (src2->w < 0.01f)
			continue;
		float a = (-src1->w) / (src2->w - src1->w);
		float ima = 1.0f - a;
		// create new vertex
		_vdst[dsti].x = src1->x*ima + src2->x*a;
		_vdst[dsti].y = src1->y*ima + src2->y*a;
		_vdst[dsti].z = src1->z*ima + src2->z*a;
		_vdst[dsti].w = 0.01f;
		_vdst[dsti].modify = 0;
		dsti++;
	}
	return dsti;
}

// --------------------------------------------------------------------------------------------------------------------
//
// clipping using homogeneous coordinates
// http://www.3dcpptutorials.sk/
//
// --------------------------------------------------------------------------------------------------------------------
//
// c equals the x or y or z coordinate of a fragment
//
// --------------------------------------------------------------------------------------------------------------------
//
// a fragment of an edge is in front of or lies on a negative clip plane: - w <= c
//
// a fragment of an edge is behind a negative clip plane: - w > c
//
// to find the intersection of an edge with a negative clip plane we need to find t for which: - w = c
//
// --------------------------------------------------------------------------------------------------------------------
//
// w = w1 + (w2 - w1) * t
// c = c1 + (c2 - c1) * t
//
// - w = c
//
// - (w1 + (w2 - w1) * t) = c1 + (c2 - c1) * t
// - w1 - (w2 - w1) * t = c1 + (c2 - c1) * t
// - (w2 - w1) * t = c1 + w1 + (c2 - c1) * t
// - (c2 - c1) * t - (w2 - w1) * t = c1 + w1
// (c2 - c1) * t + (w2 - w1) * t = - c1 - w1
// (c2 - c1 + w2 - w1) * t = - c1 - w1
// t = (- c1 - w1) / (c2 - c1 + w2 - w1)
// t = (c1 + w1) / (- c2 + c1 - w2 + w1)
// t = (c1 + w1) / (c1 - c2 + w1 - w2)
//
// --------------------------------------------------------------------------------------------------------------------
//
// a fragment of an edge is in front of or lies on a positive clip plane: c <= w
//
// a fragment of an edge is behind a positive clip plane: c > w
//
// to find the intersection of an edge with a positive clip plane we need to find t for which: c = w
//
// --------------------------------------------------------------------------------------------------------------------
//
// c = c1 + (c2 - c1) * t
// w = w1 + (w2 - w1) * t
//
// c = w
//
// c1 + (c2 - c1) * t = w1 + (w2 - w1) * t
// (c2 - c1) * t = w1 - c1 + (w2 - w1) * t
// (c2 - c1) * t - (w2 - w1) * t = w1 - c1
// ((c2 - c1) - (w2 - w1)) * t = w1 - c1
// (c2 - c1 - w2 + w1) * t = w1 - c1
// t = (w1 - c1) / (c2 - c1 - w2 + w1)
// t = (- w1 + c1) / (- c2 + c1 + w2 - w1)
// t = (c1 - w1) / (c1 - c2 - w1 + w2)
//
// --------------------------------------------------------------------------------------------------------------------
void clipInHomogeneousSpace(SPVertex * _pVertices, std::vector<SPVertex> & _vResult, int _clipPlane)
{
	if (_clipPlane == 5 && GBI.isNoN()) {
		// Skip near plane clipping
		clipInHomogeneousSpace(_pVertices, _vResult, _clipPlane + 1);
		return;
	}

	graphics::CombinerProgram * pCurrCmb = currentCombiner();
	const bool usesColor = pCurrCmb->usesShade();
	const bool usesTexture = pCurrCmb->usesTexture();
	const float wScale = static_cast<float>(gSP.clipRatio);
	const float xScale = dwnd().getAdjustScale();

	auto clipVertex = [usesColor, usesTexture](SPVertex const* src, SPVertex * dst, float t)
	{
#define doClip(V) dst->V += (src->V - dst->V) * t
		doClip(x);
		doClip(y);
		doClip(z);
		doClip(w);
		doClip(bc0);
		doClip(bc1);
		if (usesColor)
		{
			doClip(r);
			doClip(g);
			doClip(b);
			doClip(a);
			doClip(flat_r);
			doClip(flat_g);
			doClip(flat_b);
			doClip(flat_a);
		}
		if (usesTexture)
		{
			doClip(s);
			doClip(t);
		}
		dst->modify = src->modify & (MODIFY_ST | MODIFY_RGBA);
		dst->HWLight = src->HWLight;
#undef doClip
	};

	auto copyVertex = [usesColor, usesTexture](SPVertex const* src, SPVertex * dst)
	{
#define doCopy(V) dst->V = src->V
		doCopy(x);
		doCopy(y);
		doCopy(z);
		doCopy(w);
		doCopy(bc0);
		doCopy(bc1);
		doCopy(a);
		doCopy(flat_a);
		if (usesColor)
		{
			doCopy(r);
			doCopy(g);
			doCopy(b);
			doCopy(flat_r);
			doCopy(flat_g);
			doCopy(flat_b);
		}
		if (usesTexture)
		{
			doCopy(s);
			doCopy(t);
		}
		doCopy(HWLight);
		dst->modify = src->modify & (MODIFY_ST | MODIFY_RGBA);
#undef doCopy
	};


	if (_clipPlane >= 1 && _clipPlane <= 6) {
		unsigned int fifocp, fbcp, fbcpc = 0; // fragment in front of clip plane, fragment behind clip plane, fragments behind clip plane count

		// visibility testing

		SPVertex *pVertex = _pVertices;

		for (unsigned int i = 0; i < 3; i++) {
			bool visible;

			switch (_clipPlane) {
			case 1:
				visible = -pVertex->w * wScale <= pVertex->x * xScale;
				break;
			case 2:
				visible = pVertex->x * xScale <= pVertex->w * wScale;
				break;
			case 3:
				visible = -pVertex->w * wScale <= pVertex->y;
				break;
			case 4:
				visible = pVertex->y <= pVertex->w * wScale;
				break;
			case 5:
				visible = -pVertex->w * wScale <= pVertex->z;
				break;
			case 6:
				visible = pVertex->z <= pVertex->w * wScale;
				break;
			}

			if (visible) {
				fifocp = i;
			} else {
				fbcp = i;
				fbcpc++;
			}

			pVertex++;
		}

		// clipping

		if (fbcpc == 3) {
			return;
		} else if (fbcpc == 2) {
			SPVertex *Vertex1 = _pVertices + (fifocp + 1) % 3;
			SPVertex *Vertex2 = _pVertices + (fifocp + 2) % 3;
			SPVertex *Vertex3 = _pVertices + fifocp;

			float t1, t2;
			const float scaledW1 = Vertex1->w * wScale;
			const float scaledW2 = Vertex2->w * wScale;
			const float scaledW3 = Vertex3->w * wScale;

			switch (_clipPlane) {
			case 1:
			{
				const float scaledX1 = Vertex1->x * xScale;
				const float scaledX2 = Vertex2->x * xScale;
				const float scaledX3 = Vertex3->x * xScale;
				t1 = (scaledX1 + scaledW1) / (scaledX1 - scaledX3 + scaledW1 - scaledW3);
				t2 = (scaledX2 + scaledW2) / (scaledX2 - scaledX3 + scaledW2 - scaledW3);
			}
				break;

			case 2:
			{
				const float scaledX1 = Vertex1->x * xScale;
				const float scaledX2 = Vertex2->x * xScale;
				const float scaledX3 = Vertex3->x * xScale;
				t1 = (scaledX1 - scaledW1) / (scaledX1 - scaledX3 - scaledW1 + scaledW3);
				t2 = (scaledX2 - scaledW2) / (scaledX2 - scaledX3 - scaledW2 + scaledW3);
			}
				break;

			case 3:
				t1 = (Vertex1->y + scaledW1) / (Vertex1->y - Vertex3->y + scaledW1 - scaledW3);
				t2 = (Vertex2->y + scaledW2) / (Vertex2->y - Vertex3->y + scaledW2 - scaledW3);
				break;

			case 4:
				t1 = (Vertex1->y - scaledW1) / (Vertex1->y - Vertex3->y - scaledW1 + scaledW3);
				t2 = (Vertex2->y - scaledW2) / (Vertex2->y - Vertex3->y - scaledW2 + scaledW3);
				break;

			case 5:
				t1 = (Vertex1->z + scaledW1) / (Vertex1->z - Vertex3->z + scaledW1 - scaledW3);
				t2 = (Vertex2->z + scaledW2) / (Vertex2->z - Vertex3->z + scaledW2 - scaledW3);
				break;

			case 6:
				t1 = (Vertex1->z - scaledW1) / (Vertex1->z - Vertex3->z - scaledW1 + scaledW3);
				t2 = (Vertex2->z - scaledW2) / (Vertex2->z - Vertex3->z - scaledW2 + scaledW3);
				break;
			}

			clipVertex(Vertex3, Vertex1, t1);
			clipVertex(Vertex3, Vertex2, t2);

			clipInHomogeneousSpace(_pVertices, _vResult, _clipPlane + 1);
		} else if (fbcpc == 1) {
			unsigned int if1 = (fbcp + 1) % 3, if2 = (fbcp + 2) % 3;

			SPVertex *Vertex1 = _pVertices + if1;
			SPVertex *Vertex2 = _pVertices + if2;
			SPVertex *Vertex3 = _pVertices + fbcp;

			SPVertex NewVertices[3];

			SPVertex *NewVertex1 = NewVertices + if1;
			SPVertex *NewVertex2 = NewVertices + if2;
			SPVertex *NewVertex3 = NewVertices + fbcp;

			float t1, t2;
			const float scaledW1 = Vertex1->w * wScale;
			const float scaledW2 = Vertex2->w * wScale;
			const float scaledW3 = Vertex3->w * wScale;

			switch (_clipPlane) {
			case 1:
			{
				const float scaledX1 = Vertex1->x * xScale;
				const float scaledX2 = Vertex2->x * xScale;
				const float scaledX3 = Vertex3->x * xScale;
				t1 = (scaledX3 + scaledW3) / (scaledX3 - scaledX1 + scaledW3 - scaledW1);
				t2 = (scaledX3 + scaledW3) / (scaledX3 - scaledX2 + scaledW3 - scaledW2);
			}
				break;

			case 2:
			{
				const float scaledX1 = Vertex1->x * xScale;
				const float scaledX2 = Vertex2->x * xScale;
				const float scaledX3 = Vertex3->x * xScale;
				t1 = (scaledX3 - scaledW3) / (scaledX3 - scaledX1 - scaledW3 + scaledW1);
				t2 = (scaledX3 - scaledW3) / (scaledX3 - scaledX2 - scaledW3 + scaledW2);
			}
				break;

			case 3:
				t1 = (Vertex3->y + scaledW3) / (Vertex3->y - Vertex1->y + scaledW3 - scaledW1);
				t2 = (Vertex3->y + scaledW3) / (Vertex3->y - Vertex2->y + scaledW3 - scaledW2);
				break;

			case 4:
				t1 = (Vertex3->y - scaledW3) / (Vertex3->y - Vertex1->y - scaledW3 + scaledW1);
				t2 = (Vertex3->y - scaledW3) / (Vertex3->y - Vertex2->y - scaledW3 + scaledW2);
				break;

			case 5:
				t1 = (Vertex3->z + scaledW3) / (Vertex3->z - Vertex1->z + scaledW3 - scaledW1);
				t2 = (Vertex3->z + scaledW3) / (Vertex3->z - Vertex2->z + scaledW3 - scaledW2);
				break;

			case 6:
				t1 = (Vertex3->z - scaledW3) / (Vertex3->z - Vertex1->z - scaledW3 + scaledW1);
				t2 = (Vertex3->z - scaledW3) / (Vertex3->z - Vertex2->z - scaledW3 + scaledW2);
				break;
			}

			copyVertex(Vertex3, NewVertex3);
			copyVertex(Vertex2, NewVertex2);
			clipVertex(Vertex1, Vertex3, t1);
			copyVertex(Vertex3, NewVertex1);
			clipVertex(NewVertex2, NewVertex3, t2);

			clipInHomogeneousSpace(_pVertices, _vResult, _clipPlane + 1);
			clipInHomogeneousSpace(NewVertices, _vResult, _clipPlane + 1);
		} else if (fbcpc == 0) {
			clipInHomogeneousSpace(_pVertices, _vResult, _clipPlane + 1);
		}
	} else if (_clipPlane == 7) {
		for (int i = 0; i < 3; ++i)
			_vResult.push_back(_pVertices[i]);
	}
}
