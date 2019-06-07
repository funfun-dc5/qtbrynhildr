// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if _MSC_VER
#include <intrin.h>
#else // _MSC_VER
#include <x86intrin.h>
#endif // _MSC_VER

// Qt Header

// Local Header
#include "yuv2rgb.h"

namespace qtbrynhildr {

#if QTB_SIMD_SUPPORT

// SSE & AVX2

#if defined(__AVX2__)

#if QTB_MULTI_THREAD_CONVERTER

// YUV convert to RGB (SIMD version)
void convertYUVtoRGB_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  Aligned(16) int result[4];

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

#if 1 // for TEST

  __m128i yc = _mm_setr_epi32(256, 256, 256, 0);
  __m128i uc = _mm_setr_epi32(0,   -88, 453, 0);
  __m128i vc = _mm_setr_epi32(358,-182,   0, 0);

  __m128i constMaxV = _mm_setr_epi32(255, 255, 255, 255);
  //  __m128i constMinV = _mm_setr_epi32(0, 0, 0, 0);
  __m128i constMinV = _mm_setzero_si128();

#else // for TEST

  Aligned(16) const int constYc[4] = {256, 256, 256, 0};
  Aligned(16) const int constUc[4] = {0,   -88, 453, 0};
  Aligned(16) const int constVc[4] = {358,-182,   0, 0};

  // 2) load Yc, Uc, Vc
  __m128i yc = _mm_load_si128((const __m128i*)constYc);
  __m128i uc = _mm_load_si128((const __m128i*)constUc);
  __m128i vc = _mm_load_si128((const __m128i*)constVc);

  Aligned(16) const int constMax[4] = {255, 255, 255, 255};
  Aligned(16) const int constMin[4] = {  0,   0,   0,   0};

  __m128i constMaxV = _mm_load_si128((const __m128i*)constMax);
  __m128i constMinV = _mm_load_si128((const __m128i*)constMin);

#endif // for TEST

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  __m128i yv, uv, vv;
	  __m128i uv0, vv0;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  // load U, V
	  uv0 = _mm_set1_epi32(u);
	  vv0 = _mm_set1_epi32(v);
	  uv0 = _mm_broadcastd_epi32(uv0); // AVX2
	  vv0 = _mm_broadcastd_epi32(vv0); // AVX2

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// xPos

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
		}
		else {
		  // 1) load Y
		  yv = _mm_set1_epi32(y);
		  yv = _mm_broadcastd_epi32(yv);	// AVX2

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)result[0];

		  // G
		  *rgbtop++ = (uchar)result[1];

		  // B
		  *rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)result[2];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#endif // QTB_LITTLE_ENDIAN
#endif
		}

		// xPos+1

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
		}
		else {
		  // 1) load Y
		  yv = _mm_set1_epi32(y);
		  yv = _mm_broadcastd_epi32(yv);	// AVX2

		  // 2) Y * Yc -> Y
		  yv = _mm_mullo_epi32(yv, yc);

		  // 3) U * Uc -> U
		  uv = _mm_mullo_epi32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = _mm_mullo_epi32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = _mm_add_epi32(yv, uv);
		  yv = _mm_add_epi32(yv, vv);

		  // 6) >> 8
		  yv = _mm_srai_epi32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = _mm_min_epi32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = _mm_max_epi32(yv, constMinV);

		  // 9) store to result
		  _mm_store_si128((__m128i*)result, yv);

		  // set rgba32 from result int * 4

#if FORMAT_RGB888
		  // RR(8bit), GG(8bit), BB(8bit)
		  // R
		  *rgbtop++ = (uchar)result[0];

		  // G
		  *rgbtop++ = (uchar)result[1];

		  // B
		  *rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		  // 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		  // B
		  *rgbtop++ = (uchar)result[2];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		  // A
		  rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		  // R
		  *rgbtop++ = (uchar)result[0];
		  // G
		  *rgbtop++ = (uchar)result[1];
		  // B
		  *rgbtop++ = (uchar)result[2];
#endif // QTB_LITTLE_ENDIAN
#endif
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// xPos

		// set y
		y =  *ytop++;

		// 1) load Y
		yv = _mm_set1_epi32(y);
		yv = _mm_broadcastd_epi32(yv);	// AVX2

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)result[0];

		// G
		*rgbtop++ = (uchar)result[1];

		// B
		*rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)result[2];
		// G
		*rgbtop++ = (uchar)result[1];
		// R
		*rgbtop++ = (uchar)result[0];
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#endif // QTB_LITTLE_ENDIAN
#endif

		// xPos+1

		// set y
		y =  *ytop++;

		// 1) load Y
		yv = _mm_set1_epi32(y);
		yv = _mm_broadcastd_epi32(yv);	// AVX2

		// 2) Y * Yc -> Y
		yv = _mm_mullo_epi32(yv, yc);

		// 3) U * Uc -> U
		uv = _mm_mullo_epi32(uv0, uc);

		// 4) V * Vc -> V
		vv = _mm_mullo_epi32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = _mm_add_epi32(yv, uv);
		yv = _mm_add_epi32(yv, vv);

		// 6) >> 8
		yv = _mm_srai_epi32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = _mm_min_epi32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = _mm_max_epi32(yv, constMinV);

		// 9) store to result
		_mm_store_si128((__m128i*)result, yv);

		// set rgba32 from result int * 4

#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)result[0];

		// G
		*rgbtop++ = (uchar)result[1];

		// B
		*rgbtop++ = (uchar)result[2];
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)result[2];
		// G
		*rgbtop++ = (uchar)result[1];
		// R
		*rgbtop++ = (uchar)result[0];
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)result[0];
		// G
		*rgbtop++ = (uchar)result[1];
		// B
		*rgbtop++ = (uchar)result[2];
#endif // QTB_LITTLE_ENDIAN
#endif

		yptop += 2;
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

#endif // defined(__AVX2__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr
