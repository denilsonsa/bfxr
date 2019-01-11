#include <vector>
#include <cstdlib>

/*
class taken from http://www.firstpr.com.au/dsp/pink-noise/#Filtering
*/

namespace Synthesizer
{
  double random();
  
	class PinkNumber
	{
    private:
		int max_key;
		int key;
    std::vector<int> white_values;
		unsigned int range;

    public:
		PinkNumber();

		//returns number between -1 and 1		
		float GetNextValue();
	}; 
}
