// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header

// Qt Header

// Local Header
#include "yuv2rgb.h"


#if QTB_PUBLIC_MODE7_SUPPORT

namespace qtbrynhildr {

#if QTB_MULTI_THREAD_CONVERTER
// clip
inline int clip(int val)
{
  if (val < 0) return 0;
  if (val > 255) return 255;
  return val;
}

// YUV convert to RGB macro
#define GET_R(Y, V)		((256 * Y           + 358 * V) >> 8)
#define GET_G(Y, U, V)	((256 * Y -  88 * U - 182 * V) >> 8)
#define GET_B(Y, U)		((256 * Y + 453 * U          ) >> 8)

// YUV convert to RGB (qtbrynhildr::convertYUVtoRGB())
void convertYUVtoRGB(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  uchar *yptop;
  uchar *uptop;
  uchar *vptop;
  if (yuv == yuv1){
	yptop = y2topOrg + (ytop - y1topOrg);
	uptop = u2topOrg + (utop - u1topOrg);
	vptop = v2topOrg + (vtop - v1topOrg);
  }
  else {
	yptop = y1topOrg + (ytop - y2topOrg);
	uptop = u1topOrg + (utop - u2topOrg);
	vptop = v1topOrg + (vtop - v2topOrg);
  }

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int r, g, b;
	  int y, u, v;
	  int yp, up, vp;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  // == xPos ==
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgbtop += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgbtop++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // == xPos+1 ==
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgbtop += IMAGE_FORMAT_SIZE;
	  }
	  else {
		// R
		r = clip(GET_R(y, v));
		*rgbtop++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgbtop++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgbtop++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgbtop++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }
	}
	rgbtop += rgbNext;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
}

#endif // QTB_MULTI_THREAD_CONVERTER

} // end of namespace qtbrynhildr

#endif // QTB_PUBLIC_MODE7_SUPPORT
