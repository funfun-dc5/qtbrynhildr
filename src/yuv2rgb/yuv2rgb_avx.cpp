// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018-2020 FunFun <fu.aba.dc5@gmail.com>

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

// AVX

#if defined(__AVX__)

//#error "Yet: INTEL AVX version (too SLOW)"

#if QTB_MULTI_THREAD_CONVERTER

// YUV convert to RGB (SIMD version)
void convertYUVtoRGB_SIMD(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  Aligned(32) int result[8];

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

  // 4) load Uc
  __m256 uc = _mm256_setr_ps(0, -0.34, 1.72, 0, 0, -0.34, 1.72, 0);
  // 5) load Vc
  __m256 vc = _mm256_setr_ps(1.402, -0.714, 0, 0, 1.402, -0.714, 0, 0);

  __m256 constMaxV = _mm256_setr_ps(255,255,255,255,255,255,255,255);
  __m256 constMinV = _mm256_setzero_ps();

#else // for TEST

  Aligned(32) const float constUc[8] = {0, -0.34, 1.72, 0, 0, -0.34, 1.72, 0};
  Aligned(32) const float constVc[8] = {1.402, -0.714, 0, 0, 1.402, -0.714, 0, 0};

  const float constMax = 255.0;
  const float constMin = 0.0;

  // 4) load Uc
  __m256 uc = _mm256_load_ps(constUc);
  // 5) load Vc
  __m256 vc = _mm256_load_ps(constVc);

  __m256 constMaxV = _mm256_broadcast_ss(&constMax);
  __m256 constMinV = _mm256_broadcast_ss(&constMin);

#endif // 1 // for TEST

  // set RC flag
  unsigned long mxcsr = _mm_getcsr();
  mxcsr &= 0xFFFF9FFF;
  //  mxcsr |= 0x00000000;
  //  mxcsr |= 0x00002000;
  //  mxcsr |= 0x00004000;
  mxcsr |= 0x00006000;
  _mm_setcsr(mxcsr);

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  Aligned(32) float y[8];
	  int y1, y2, u, v;
	  int y1p, y2p, up, vp;
	  float yf1, yf2, uf, vf;
	  __m256 yv, uv, vv;
	  __m256i yvi;

	  // set current YUV and previous YUV
	  y1 = *ytop++;
	  y2 = *ytop++;
	  y1p = *yptop++;
	  y2p = *yptop++;
	  u = *(utop + uvOffset) - 128;
	  v = *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  if (y1 == y1p && y2 == y2p && u == up && v == vp){
		// No need to calculate XPos and XPos+1
		rgbtop += IMAGE_FORMAT_SIZE * 2;
	  }
	  else {

		// set y[8]
		yf1 = (float)y1;
		yf2 = (float)y2;
		y[0] = yf1;
		y[1] = yf1;
		y[2] = yf1;
		y[3] = 0.0;
		y[4] = yf2;
		y[5] = yf2;
		y[6] = yf2;
		y[7] = 0.0;

		// set uf/vf
		uf = (float)u;
		vf = (float)v;

		// ============================================================
		// 1) load Y vector
		yv = _mm256_load_ps(y);
		// 2) load U vector
		uv = _mm256_broadcast_ss(&uf);
		// 3) load V vector
		vv = _mm256_broadcast_ss(&vf);

		// 4) U * Uc -> U
		uv = _mm256_mul_ps(uv, uc);
		// 5) V * Vc -> V
		vv = _mm256_mul_ps(vv, vc);

		// 6) Y + U + V -> Y
		yv = _mm256_add_ps(yv, uv); // Y + U -> Y
		yv = _mm256_add_ps(yv, vv); // Y + V -> Y

		// 7) Y > 255 ? 255 : Y
		yv = _mm256_min_ps(yv, constMaxV);
		// 8) Y < 0 ? 0 : Y
		yv = _mm256_max_ps(yv, constMinV);

		// 9) convert float to integer
		yvi = _mm256_cvtps_epi32(yv);

		// 10) store to result
		_mm256_store_si256((__m256i*)result, yvi);
		// ============================================================

		// set rgba32 * 2 from result int * 8

		// xPos
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
#endif // FORMAT_RGB32

		// xPos+1
#if FORMAT_RGB888
		// RR(8bit), GG(8bit), BB(8bit)
		// R
		*rgbtop++ = (uchar)result[4];

		// G
		*rgbtop++ = (uchar)result[5];

		// B
		*rgbtop++ = (uchar)result[6];
#elif FORMAT_RGB32
		// 0xaarrggbb (32bit value)
#if QTB_LITTLE_ENDIAN // Little Endian
		// B
		*rgbtop++ = (uchar)result[6];
		// G
		*rgbtop++ = (uchar)result[5];
		// R
		*rgbtop++ = (uchar)result[4];
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
#else // QTB_LITTLE_ENDIAN
		// A
		rgbtop++;	// *rgbtop++ = (uchar)0xFF;
		// R
		*rgbtop++ = (uchar)result[4];
		// G
		*rgbtop++ = (uchar)result[5];
		// B
		*rgbtop++ = (uchar)result[6];
#endif // QTB_LITTLE_ENDIAN
#endif // FORMAT_RGB32
	  }
	}
#if !QTB_LOAD_BITMAP
	rgbtop += rgbNext;
#endif // !QTB_LOAD_BITMAP
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
}

#endif // QTB_MULTI_THREAD_CONVERTER

#endif // defined(__AVX__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr
