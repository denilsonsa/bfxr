#ifndef SYNTH_H
#define SYNTH_H

#include <cmath>
#include <vector>

#include "pinknumber.h"
#include "sfxrparams.h"

namespace Synthesizer 
{
		unsigned int GenerateSound(const SfxrParams& params, std::vector<double>* data);
}

#endif  // SYNTH_H
