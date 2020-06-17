// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018-2020 FunFun <fu.aba.dc5@gmail.com>

// Common Header
#include "common/common.h"

// System Header
#if defined(__arm__) || defined(__aarch64__)
#include <arm_neon.h>
#endif // defined(__arm__) || defined(__aarch64__)

// Qt Header

// Local Header
#include "yuv2rgb.h"

namespace qtbrynhildr {

#if QTB_SIMD_SUPPORT

// NEON

#if defined(__arm__) || defined(__aarch64__)

// YUV convert to RGB (SIMD version)
void convertYUVtoRGB_SIMD_NEON(uchar *ytop, uchar* utop, uchar *vtop, uchar *rgbtop, int height)
{
  Aligned(16) int result[4];

  //  Aligned(16) const int yca[4] = {256,  256, 256, 0};
  Aligned(16) const int uca[4] = {0,    -88, 453, 0};
  Aligned(16) const int vca[4] = {358, -182,   0, 0};

  //  int32x4_t yc = vld1q_s32(yca);
  int32x4_t yc = vdupq_n_s32(256);
  int32x4_t uc = vld1q_s32(uca);
  int32x4_t vc = vld1q_s32(vca);

  int32x4_t constMaxV = vdupq_n_s32(255);
  int32x4_t constMinV = vdupq_n_s32(0);

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

#if QTB_BENCHMARK
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
  // calc rate
  calcRate = 0.0;
#endif // QTB_BENCHMARK

  for (int yPos = 0; yPos < height; yPos++){
	for (int xPos = 0, uvOffset = 0; xPos < width; xPos += 2, uvOffset++){
	  int y, u, v;
	  int yp, up, vp;
	  int32x4_t yv, uv, vv;
	  int32x4_t uv0, vv0;

	  // set u/v
	  u =  *(utop + uvOffset) - 128;
	  v =  *(vtop + uvOffset) - 128;
	  up = *(uptop + uvOffset) - 128;
	  vp = *(vptop + uvOffset) - 128;

	  // load U, V
	  uv0 = vdupq_n_s32(u);
	  vv0 = vdupq_n_s32(v);

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y != yp

		// xPos

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  // 1) load Y
		  yv = vdupq_n_s32(y);

		  // 2) Y * Yc -> Y
		  yv = vmulq_s32(yv, yc);

		  // 3) U * Uc -> U
		  uv = vmulq_s32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = vmulq_s32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = vaddq_s32(yv, uv);
		  yv = vaddq_s32(yv, vv);

		  // 6) >> 8
		  yv = vshrq_n_s32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = vminq_s32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = vmaxq_s32(yv, constMinV);

		  // 9) store to result
		  vst1q_s32(result, yv);

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
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}

		// xPos+1

		// set y
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgbtop += IMAGE_FORMAT_SIZE;
#if QTB_BENCHMARK
		  skipCounter++;
#endif // QTB_BENCHMARK
		}
		else {
		  // 1) load Y
		  yv = vdupq_n_s32(y);

		  // 2) Y * Yc -> Y
		  yv = vmulq_s32(yv, yc);

		  // 3) U * Uc -> U
		  uv = vmulq_s32(uv0, uc);

		  // 4) V * Vc -> V
		  vv = vmulq_s32(vv0, vc);

		  // 5) Y + U + V -> Y
		  yv = vaddq_s32(yv, uv);
		  yv = vaddq_s32(yv, vv);

		  // 6) >> 8
		  yv = vshrq_n_s32(yv, 8);

		  // 7) Y > 255 ? 255 : Y
		  yv = vminq_s32(yv, constMaxV);

		  // 8)  Y < 0 ? 0 : Y
		  yv = vmaxq_s32(yv, constMinV);

		  // 9) store to result
		  vst1q_s32(result, yv);

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
#endif // FORMAT_RGB32

#if QTB_BENCHMARK
		  calcCounter++;
#endif // QTB_BENCHMARK
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// xPos

		// set y
		y =  *ytop++;

		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

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
#endif // FORMAT_RGB32

		// xPos+1

		// set y
		y =  *ytop++;

		// 1) load Y
		yv = vdupq_n_s32(y);

		// 2) Y * Yc -> Y
		yv = vmulq_s32(yv, yc);

		// 3) U * Uc -> U
		uv = vmulq_s32(uv0, uc);

		// 4) V * Vc -> V
		vv = vmulq_s32(vv0, vc);

		// 5) Y + U + V -> Y
		yv = vaddq_s32(yv, uv);
		yv = vaddq_s32(yv, vv);

		// 6) >> 8
		yv = vshrq_n_s32(yv, 8);

		// 7) Y > 255 ? 255 : Y
		yv = vminq_s32(yv, constMaxV);

		// 8)  Y < 0 ? 0 : Y
		yv = vmaxq_s32(yv, constMinV);

		// 9) store to result
		vst1q_s32(result, yv);

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
#endif // FORMAT_RGB32

		yptop += 2;

#if QTB_BENCHMARK
		calcCounter += 2;
#endif // QTB_BENCHMARK
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
#if QTB_BENCHMARK
  if (calcCounter + skipCounter > 0)
	calcRate = (double)calcCounter/(calcCounter + skipCounter) * 100.0;
#endif // QTB_BENCHMARK
}

#endif // defined(__arm__) || defined(__aarch64__)

#endif // QTB_SIMD_SUPPORT

} // end of namespace qtbrynhildr
