#include <vector>
#include <cstdlib>
#include "pinknumber.h"

/*
class taken from http://www.firstpr.com.au/dsp/pink-noise/#Filtering
*/

namespace Synthesizer
{
    float random()
    {
      return rand() / static_cast<float>(RAND_MAX);
    }
		
    PinkNumber::PinkNumber()
		{
			max_key = 0x1f; // Five bits set
			range = 128;
			key = 0;
			for (int i = 0; i < 5; i++)
				white_values.push_back(random() * (range/5.0f));
		}
		
		
		//returns number between -1 and 1		
		float PinkNumber::GetNextValue()
		{
			int last_key = key;
			
			key++;
			if (key > max_key)
				key = 0;
			// Exclusive-Or previous value with current value. This gives
			// a list of bits that have changed.
			int diff = last_key ^ key;
			unsigned int sum = 0;
			for (int i = 0; i < 5; i++)
			{
				// If bit changed get new random number for corresponding
				// white_value
				if (diff & (1 << i))
					white_values[i] = random() * (range/5.0f);
				sum += white_values[i];
			}
			return sum/64.0f-1.0f;
		}
}
