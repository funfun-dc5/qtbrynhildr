// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// yuv2rgb_neon function body

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
  if (yuv420 == yuv1){
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

	  // xPos

	  // set y
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
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

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }

	  // xPos+1

	  // set y
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
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

		// R
		*rgb24top++ = (uchar)result[0];

		// G
		*rgb24top++ = (uchar)result[1];

		// B
		*rgb24top++ = (uchar)result[2];

#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
	  }
	}
	rgb24top += rgb24Next;
	if (yPos & 0x1){
	  utop += uvNext;
	  vtop += uvNext;
	  uptop += uvNext;
	  vptop += uvNext;
	}
  }
}
