// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// yuv2rgb_avx function body

{
  Aligned(32) int result[8];

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
		rgb24top += IMAGE_FORMAT_SIZE * 2;
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

		// xPos+1
		// R
		*rgb24top++ = (uchar)result[4];

		// G
		*rgb24top++ = (uchar)result[5];

		// B
		*rgb24top++ = (uchar)result[6];

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
