// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// yuv2rgb_sse function body

// NOW SSE instruction only
#ifdef __AVX2__
#undef __AVX2__
#endif // __AVX2__

{
  Aligned(16) int result[4];

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

#if defined(__AVX2__)
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

	  // xPos

	  // set y
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
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

#else // defined(__AVX2__)

  Aligned(16) int ya[4] = {  0,   0,   0,   0};
  Aligned(16) int ua[4] = {  0,   0,   0,   0};
  Aligned(16) int va[4] = {  0,   0,   0,   0};

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
	  ua[0] = u;
	  ua[1] = u;
	  ua[2] = u;
	  va[0] = v;
	  va[1] = v;
	  va[2] = v;

	  // load U, V
	  uv0 = _mm_load_si128((const __m128i*)ua);
	  vv0 = _mm_load_si128((const __m128i*)va);

	  // xPos

	  // set y
	  y =  *ytop++;
	  yp = *yptop++;
	  if (y == yp && u == up && v == vp){
		rgb24top += IMAGE_FORMAT_SIZE;
	  }
	  else {
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

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
		ya[0] = y;
		ya[1] = y;
		ya[2] = y;

		// 1) load Y
		yv = _mm_load_si128((const __m128i*)ya);

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
#endif // defined(__AVX2__)
}
