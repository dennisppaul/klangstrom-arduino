// Comb filter class declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _comb_
#define _comb_

#include "denormals.h"

class comb
{
public:
					comb();
			void	setbuffer(float *buf, int size);
	inline  float	process(float inp);
			void	mute();
			void	setdamp(float val);
			float	getdamp();
			void	setfeedback(float val);
			float	getfeedback();
private:
	float	feedback;
	float	filterstore;
	float	damp1;
	float	damp2;
	float	*buffer;
	int		bufsize;
	int		bufidx;
};

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-variable"
# pragma clang diagnostic ignored "-Wstrict-aliasing"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

// Big to inline - but crucial for speed

inline float comb::process(float input)
{
	float output;

	output = buffer[bufidx];
	undenormalise(output);

	filterstore = (output*damp2) + (filterstore*damp1);
	undenormalise(filterstore);

	buffer[bufidx] = input + (filterstore*feedback);

	if(++bufidx>=bufsize) bufidx = 0;

	return output;
}

#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif //_comb_

//ends
