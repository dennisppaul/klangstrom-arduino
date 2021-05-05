// Allpass filter declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _allpass_
#define _allpass_
#include "denormals.h"

class allpass
{
public:
					allpass();
			void	setbuffer(float *buf, int size);
	inline  float	process(float inp);
			void	mute();
			void	setfeedback(float val);
			float	getfeedback();
// private:
	float	feedback;
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

inline float allpass::process(float input)
{
	float output;
	float bufout;
	
	bufout = buffer[bufidx];
	undenormalise(bufout);
	
	output = -input + bufout;
	buffer[bufidx] = input + (bufout*feedback);

	if(++bufidx>=bufsize) bufidx = 0;

	return output;
}

#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif//_allpass

//ends
