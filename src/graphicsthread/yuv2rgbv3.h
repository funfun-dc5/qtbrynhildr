// -*- mode: c++; coding: utf-8-unix -*-
// Copyright (c) 2018 FunFun <fu.aba.dc5@gmail.com>

// yuv2rgbv3 function body

{
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

#if PRINT_CALC_RATE // for TEST
  // frame counter
  static int frameCounter = 0;
  frameCounter++;
  // skip counter
  int skipCounter = 0;
  // calc counter
  int calcCounter = 0;
#endif // for TEST

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

	  if (u == up && v == vp){
		// calculate xPos/xPos+1 if y == yp (y, u, v is NOT changed)

		// == xPos ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgb24top += IMAGE_FORMAT_SIZE;
#if PRINT_CALC_RATE // for TEST
		  skipCounter++;
#endif // for TEST
		}
		else {
		  y <<= 8; // y * 256

		  // R
		  r = clip(GET_R(y, v));
		  *rgb24top++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgb24top++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		  // A
		  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
#if PRINT_CALC_RATE // for TEST
		  calcCounter++;
#endif // for TEST
		}

		// == xPos+1 ==
		y =  *ytop++;
		yp = *yptop++;
		if (y == yp){
		  rgb24top += IMAGE_FORMAT_SIZE;
#if PRINT_CALC_RATE // for TEST
		  skipCounter++;
#endif // for TEST
		}
		else {
		  y <<= 8; // y * 256

		  // R
		  r = clip(GET_R(y, v));
		  *rgb24top++ = (uchar)r;
		  // G
		  g = clip(GET_G(y, u, v));
		  *rgb24top++ = (uchar)g;
		  // B
		  b = clip(GET_B(y, u));
		  *rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		  // A
		  *rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888
#if PRINT_CALC_RATE // for TEST
		  calcCounter++;
#endif // for TEST
		}
	  }
	  else {
		// always calculate xPos/xPos+1

		// == xPos ==
		y =  *ytop++;
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

		// == xPos+1 ==
		y =  *ytop++;
		y <<= 8; // y * 256

		// R
		r = clip(GET_R(y, v));
		*rgb24top++ = (uchar)r;
		// G
		g = clip(GET_G(y, u, v));
		*rgb24top++ = (uchar)g;
		// B
		b = clip(GET_B(y, u));
		*rgb24top++ = (uchar)b;
#if FORMAT_RGBA8888
		// A
		*rgb24top++ = (uchar)255;
#endif // FORMAT_RGBA8888

		yptop += 2;
#if PRINT_CALC_RATE // for TEST
		calcCounter += 2;
#endif // for TEST
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
#if PRINT_CALC_RATE // for TEST
  cout << "  calc rate : " <<
	(float)calcCounter/(calcCounter + skipCounter) * 100.0 << " (%) : frame " << frameCounter << endl << flush;
#endif // for TEST
}
