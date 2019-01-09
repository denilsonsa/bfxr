#include <vector>
#include <cstdlib>

/*
class taken from http://www.firstpr.com.au/dsp/pink-noise/#Filtering
*/

namespace Synthesizer
{
	class PinkNumber
	{
    private:
		int max_key;
		int key;
    std::vector<int> white_values;
		unsigned int range;

    float random()
    {
      return rand() / static_cast<float>(RAND_MAX);
    }
		
    public:
		PinkNumber()
		{
			max_key = 0x1f; // Five bits set
			range = 128;
			key = 0;
			for (int i = 0; i < 5; i++)
				white_values.push_back(random() * (range/5.0f));
		}
		
		
		//returns number between -1 and 1		
		float GetNextValue()
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
	}; 
}
