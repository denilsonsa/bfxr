#ifndef BFXR_H
#define BFXR_H

// ----------------------------------------------------------------------
// Documentation section
// ----------------------------------------------------------------------

// todo: add documentation on how to use the library

// ----------------------------------------------------------------------
// Include section
// ----------------------------------------------------------------------

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <vector>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>


// ----------------------------------------------------------------------
// Header section
// ----------------------------------------------------------------------


namespace bfxr
{
  // Exposes AllOnes<N>::Value where N is a positive integer and Value is the number
  // of 1s in the binary value 0b11 (in this case N=2)
  template<int f> struct AllOnes { enum {Value = (1 << (f-1) ) | AllOnes<f-1>::Value}; };
  template<> struct AllOnes<0> { enum {Value=0}; };

  /*
     Implementes the 1/f noise algorithm discovered by Richard F. Voss
     of the Thomas J. Watson Research Instritute of IBM as described in
      White and brown music, fractal curves and one-over-f fluctuations
      by: M. Gardner
      Scientific American, Vol. 238, No. 4. (April 1978), pp. 16-27

    Implmentation inspired by the class by Thomas Hudson found at
    http://www.firstpr.com.au/dsp/pink-noise/#Filtering

    License: zlib
   */
  class PinkNoise
  {
    private:
      enum { NUMBER_OF_VALUES = 5 };

      int index;
      double white_values[NUMBER_OF_VALUES];

    public:
      PinkNoise();

      //returns number between 0 and 1
      double GetNextValue();
  }; 
}

namespace bfxr
{
  /**
   * BfxrParams
   * 
   * Copyright 2010 Thomas Vian
   *
   * Licensed under the Apache License, Version 2.0 (the "License");
   * you may not use this file except in compliance with the License.
   * You may obtain a copy of the License at
   *
   * 	http://www.apache.org/licenses/LICENSE-2.0
   *
   * Unless required by applicable law or agreed to in writing, software
   * distributed under the License is distributed on an "AS IS" BASIS,
   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   * See the License for the specific language governing permissions and
   * limitations under the License.
   * 
   * @author Thomas Vian
   */

  enum class WaveType
  {
    Square,
    Saw,
    Sin,
    Noise,
    Triangle,
    Pink,
    Tan,
    Whistle,
    Breaker,
    OneBitNoise,
    Buzz,
    COUNT
  };

  class BfxrParams 
  {
    public:
      WaveType waveType;

      double masterVolume = 0.0;
      double attackTime = 0.0;
      double sustainTime = 0.0;
      double sustainPunch = 0.0;
      double decayTime = 0.0;
      double compressionAmount = 0.0;
      double startFrequency = 0.0;
      double minFrequency = 0.0;
      double slide = 0.0;
      double deltaSlide = 0.0;
      double vibratoDepth = 0.0;
      double vibratoSpeed = 0.0;
      double overtones = 0.0;
      double overtoneFalloff = 0.0;
      double changeRepeat = 0.0;
      double changeAmount = 0.0;
      double changeSpeed = 0.0;
      double changeAmount2 = 0.0;
      double changeSpeed2 = 0.0;
      double squareDuty = 0.0;
      double dutySweep = 0.0;
      double repeatSpeed = 0.0;
      double flangerOffset = 0.0;
      double flangerSweep = 0.0;
      double lpFilterCutoff = 0.0;
      double lpFilterCutoffSweep = 0.0;
      double lpFilterResonance = 0.0;
      double hpFilterCutoff = 0.0;
      double hpFilterCutoffSweep = 0.0;
      double bitCrush = 0.0;
      double bitCrushSweep = 0.0;

      bool waveType_locked = false;

      bool masterVolume_locked = false;
      bool attackTime_locked = false;
      bool sustainTime_locked = false;
      bool sustainPunch_locked = false;
      bool decayTime_locked = false;
      bool compressionAmount_locked = false;
      bool startFrequency_locked = false;
      bool minFrequency_locked = false;
      bool slide_locked = false;
      bool deltaSlide_locked = false;
      bool vibratoDepth_locked = false;
      bool vibratoSpeed_locked = false;
      bool overtones_locked = false;
      bool overtoneFalloff_locked = false;
      bool changeRepeat_locked = false;
      bool changeAmount_locked = false;
      bool changeSpeed_locked = false;
      bool changeAmount2_locked = false;
      bool changeSpeed2_locked = false;
      bool squareDuty_locked = false;
      bool dutySweep_locked = false;
      bool repeatSpeed_locked = false;
      bool flangerOffset_locked = false;
      bool flangerSweep_locked = false;
      bool lpFilterCutoff_locked = false;
      bool lpFilterCutoffSweep_locked = false;
      bool lpFilterResonance_locked = false;
      bool hpFilterCutoff_locked = false;
      bool hpFilterCutoffSweep_locked = false;
      bool bitCrush_locked = false;
      bool bitCrushSweep_locked = false;

      BfxrParams();

      void setAllLocked(bool locked);
      void generatePickupCoin();
      void generateLaserShoot();
      void generateExplosion();
      void generatePowerup();
      void generateHitHurt();
      void generateJump();
      void generateBlipSelect();
      void resetParams();
      void mutate(double mutation = 0.05);
      void randomize();

      // make sure all the doubles are within range
      void makeValid();
  };
}

#define BFXR_PARAM_masterVolume_DEF 0.5
#define BFXR_PARAM_masterVolume_MIN 0
#define BFXR_PARAM_masterVolume_MAX 1

#define BFXR_PARAM_attackTime_DEF 0
#define BFXR_PARAM_attackTime_MIN 0
#define BFXR_PARAM_attackTime_MAX 1

#define BFXR_PARAM_sustainTime_DEF 0.3
#define BFXR_PARAM_sustainTime_MIN 0
#define BFXR_PARAM_sustainTime_MAX 1

#define BFXR_PARAM_sustainPunch_DEF 0
#define BFXR_PARAM_sustainPunch_MIN 0
#define BFXR_PARAM_sustainPunch_MAX 1

#define BFXR_PARAM_decayTime_DEF 0.4
#define BFXR_PARAM_decayTime_MIN 0
#define BFXR_PARAM_decayTime_MAX 1

#define BFXR_PARAM_compressionAmount_DEF 0.3
#define BFXR_PARAM_compressionAmount_MIN 0
#define BFXR_PARAM_compressionAmount_MAX 1

#define BFXR_PARAM_startFrequency_DEF 0.3
#define BFXR_PARAM_startFrequency_MIN 0
#define BFXR_PARAM_startFrequency_MAX 1

#define BFXR_PARAM_minFrequency_DEF 0.0
#define BFXR_PARAM_minFrequency_MIN 0
#define BFXR_PARAM_minFrequency_MAX 1

#define BFXR_PARAM_slide_DEF 0.0
#define BFXR_PARAM_slide_MIN -1
#define BFXR_PARAM_slide_MAX 1

#define BFXR_PARAM_deltaSlide_DEF 0.0
#define BFXR_PARAM_deltaSlide_MIN -1
#define BFXR_PARAM_deltaSlide_MAX 1

#define BFXR_PARAM_vibratoDepth_DEF 0
#define BFXR_PARAM_vibratoDepth_MIN 0
#define BFXR_PARAM_vibratoDepth_MAX 1

#define BFXR_PARAM_vibratoSpeed_DEF 0
#define BFXR_PARAM_vibratoSpeed_MIN 0
#define BFXR_PARAM_vibratoSpeed_MAX 1

#define BFXR_PARAM_overtones_DEF 0
#define BFXR_PARAM_overtones_MIN 0
#define BFXR_PARAM_overtones_MAX 1

#define BFXR_PARAM_overtoneFalloff_DEF 0
#define BFXR_PARAM_overtoneFalloff_MIN 0
#define BFXR_PARAM_overtoneFalloff_MAX 1

#define BFXR_PARAM_changeRepeat_DEF 0
#define BFXR_PARAM_changeRepeat_MIN 0
#define BFXR_PARAM_changeRepeat_MAX 1

#define BFXR_PARAM_changeAmount_DEF 0
#define BFXR_PARAM_changeAmount_MIN -1
#define BFXR_PARAM_changeAmount_MAX 1

#define BFXR_PARAM_changeSpeed_DEF 0
#define BFXR_PARAM_changeSpeed_MIN 0
#define BFXR_PARAM_changeSpeed_MAX 1

#define BFXR_PARAM_changeAmount2_DEF 0
#define BFXR_PARAM_changeAmount2_MIN -1
#define BFXR_PARAM_changeAmount2_MAX 1

#define BFXR_PARAM_changeSpeed2_DEF 0
#define BFXR_PARAM_changeSpeed2_MIN 0
#define BFXR_PARAM_changeSpeed2_MAX 1

#define BFXR_PARAM_squareDuty_DEF 0
#define BFXR_PARAM_squareDuty_MIN 0
#define BFXR_PARAM_squareDuty_MAX 1

#define BFXR_PARAM_dutySweep_DEF 0
#define BFXR_PARAM_dutySweep_MIN -1
#define BFXR_PARAM_dutySweep_MAX 1

#define BFXR_PARAM_repeatSpeed_DEF 0
#define BFXR_PARAM_repeatSpeed_MIN 0
#define BFXR_PARAM_repeatSpeed_MAX 1

#define BFXR_PARAM_flangerOffset_DEF 0
#define BFXR_PARAM_flangerOffset_MIN -1
#define BFXR_PARAM_flangerOffset_MAX 1

#define BFXR_PARAM_flangerSweep_DEF 0
#define BFXR_PARAM_flangerSweep_MIN -1
#define BFXR_PARAM_flangerSweep_MAX 1

#define BFXR_PARAM_lpFilterCutoff_DEF 1
#define BFXR_PARAM_lpFilterCutoff_MIN 0
#define BFXR_PARAM_lpFilterCutoff_MAX 1

#define BFXR_PARAM_lpFilterCutoffSweep_DEF 0
#define BFXR_PARAM_lpFilterCutoffSweep_MIN -1
#define BFXR_PARAM_lpFilterCutoffSweep_MAX 1

#define BFXR_PARAM_lpFilterResonance_DEF 0
#define BFXR_PARAM_lpFilterResonance_MIN 0
#define BFXR_PARAM_lpFilterResonance_MAX 1

#define BFXR_PARAM_hpFilterCutoff_DEF 0
#define BFXR_PARAM_hpFilterCutoff_MIN 0
#define BFXR_PARAM_hpFilterCutoff_MAX 1

#define BFXR_PARAM_hpFilterCutoffSweep_DEF 0
#define BFXR_PARAM_hpFilterCutoffSweep_MIN -1
#define BFXR_PARAM_hpFilterCutoffSweep_MAX 1

#define BFXR_PARAM_bitCrush_DEF 0
#define BFXR_PARAM_bitCrush_MIN 0
#define BFXR_PARAM_bitCrush_MAX 1

#define BFXR_PARAM_bitCrushSweep_DEF 0
#define BFXR_PARAM_bitCrushSweep_MIN -1
#define BFXR_PARAM_bitCrushSweep_MAX 1



#define BFXR_PARAM_masterVolume_RANDOM_POWER 1.0
#define BFXR_PARAM_attackTime_RANDOM_POWER 4
#define BFXR_PARAM_sustainTime_RANDOM_POWER 2
#define BFXR_PARAM_sustainPunch_RANDOM_POWER 2
#define BFXR_PARAM_decayTime_RANDOM_POWER 1.0
#define BFXR_PARAM_compressionAmount_RANDOM_POWER 1.0
#define BFXR_PARAM_startFrequency_RANDOM_POWER 1.0
#define BFXR_PARAM_minFrequency_RANDOM_POWER 1.0
#define BFXR_PARAM_slide_RANDOM_POWER 1.0
#define BFXR_PARAM_deltaSlide_RANDOM_POWER 1.0
#define BFXR_PARAM_vibratoDepth_RANDOM_POWER 3
#define BFXR_PARAM_vibratoSpeed_RANDOM_POWER 1.0
#define BFXR_PARAM_overtones_RANDOM_POWER 3
#define BFXR_PARAM_overtoneFalloff_RANDOM_POWER 0.25
#define BFXR_PARAM_changeRepeat_RANDOM_POWER 1.0
#define BFXR_PARAM_changeAmount_RANDOM_POWER 1.0
#define BFXR_PARAM_changeSpeed_RANDOM_POWER 1.0
#define BFXR_PARAM_changeAmount2_RANDOM_POWER 1.0
#define BFXR_PARAM_changeSpeed2_RANDOM_POWER 1.0
#define BFXR_PARAM_squareDuty_RANDOM_POWER 1.0
#define BFXR_PARAM_dutySweep_RANDOM_POWER 3
#define BFXR_PARAM_repeatSpeed_RANDOM_POWER 1.0
#define BFXR_PARAM_flangerOffset_RANDOM_POWER 3
#define BFXR_PARAM_flangerSweep_RANDOM_POWER 3
#define BFXR_PARAM_lpFilterCutoff_RANDOM_POWER 0.3
#define BFXR_PARAM_lpFilterCutoffSweep_RANDOM_POWER 1.0
#define BFXR_PARAM_lpFilterResonance_RANDOM_POWER 1.0
#define BFXR_PARAM_hpFilterCutoff_RANDOM_POWER 5
#define BFXR_PARAM_hpFilterCutoffSweep_RANDOM_POWER 1.0
#define BFXR_PARAM_bitCrush_RANDOM_POWER 4
#define BFXR_PARAM_bitCrushSweep_RANDOM_POWER 5



namespace bfxr
{
  void GenerateSound(const BfxrParams& params, std::vector<double>* data);


  // Copied straight out of sfxr source
  // license: MIT
  bool SaveWav(const char* filename, const std::vector<double>& data);
}

// ----------------------------------------------------------------------
// Implementation section
// ----------------------------------------------------------------------

#ifdef BFXR_IMPLEMENTATION

#include <vector>
#include <cstdlib>

#include <vector>
#include <string>
#include <cmath>

#include <cmath>
#include <vector>

namespace bfxr
{
  double random()
  {
    return rand() / static_cast<double>(RAND_MAX);
  }

  PinkNoise::PinkNoise()
    : index(0)
  {
    for (int i = 0; i < NUMBER_OF_VALUES; i++)
    {
      white_values[i] = random();
    }
  }

  double PinkNoise::GetNextValue()
  {
    const int last_index = index;

    index++;
    if (index > AllOnes<NUMBER_OF_VALUES>::Value)
    {
      index = 0;
    }

    // Exclusive-Or previous value with current value. This gives
    // a list of bits that have changed.
    const int diff = last_index ^ index;

    double sum = 0;
    for (int i = 0; i < NUMBER_OF_VALUES; i++)
    {
      // If bit changed get new random number for corresponding white_value
      if (diff & (1 << i))
        white_values[i] = random();
      sum += white_values[i];
    }

    // the gardner article uses the dice sum but since we care about keeping
    // the 0-1 range we divide by the ammount of stored randoms
    return sum/NUMBER_OF_VALUES;
  }
}


namespace bfxr
{
  /**
   * BfxrParams
   * 
   * Copyright 2010 Thomas Vian
   *
   * Licensed under the Apache License, Version 2.0 (the "License");
   * you may not use this file except in compliance with the License.
   * You may obtain a copy of the License at
   *
   * 	http://www.apache.org/licenses/LICENSE-2.0
   *
   * Unless required by applicable law or agreed to in writing, software
   * distributed under the License is distributed on an "AS IS" BASIS,
   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   * See the License for the specific language governing permissions and
   * limitations under the License.
   * 
   * @author Thomas Vian
   */

  BfxrParams::BfxrParams()
  {
    makeValid();
    resetParams();
  }

#define ALLVALUES\
  ONVAR(masterVolume);\
  ONVAR(attackTime);\
  ONVAR(sustainTime);\
  ONVAR(sustainPunch);\
  ONVAR(decayTime);\
  ONVAR(compressionAmount);\
  ONVAR(startFrequency);\
  ONVAR(minFrequency);\
  ONVAR(slide);\
  ONVAR(deltaSlide);\
  ONVAR(vibratoDepth);\
  ONVAR(vibratoSpeed);\
  ONVAR(overtones);\
  ONVAR(overtoneFalloff);\
  ONVAR(changeRepeat);\
  ONVAR(changeAmount);\
  ONVAR(changeSpeed);\
  ONVAR(changeAmount2);\
  ONVAR(changeSpeed2);\
  ONVAR(squareDuty);\
  ONVAR(dutySweep);\
  ONVAR(repeatSpeed);\
  ONVAR(flangerOffset);\
  ONVAR(flangerSweep);\
  ONVAR(lpFilterCutoff);\
  ONVAR(lpFilterCutoffSweep);\
  ONVAR(lpFilterResonance);\
  ONVAR(hpFilterCutoff);\
  ONVAR(hpFilterCutoffSweep);\
  ONVAR(bitCrush);\
  ONVAR(bitCrushSweep);

  void BfxrParams::makeValid()
  {
#define ONVAR(n) do { n = BFXR_PARAM_##n##_DEF; } while(false)
    ALLVALUES
#undef ONVAR
  }

  void BfxrParams::setAllLocked(bool locked)
  {
    waveType_locked = locked;
#define ONVAR(p) p##_locked = locked
    ALLVALUES
#undef ONVAR
  }

  void BfxrParams::generatePickupCoin()
  {
    resetParams();

    startFrequency =0.4+random()*0.5;

    sustainTime = random() * 0.1;
    decayTime = 0.1 + random() * 0.4;
    sustainPunch = 0.3 + random() * 0.3;

    if(random() < 0.5) 
    {
      changeSpeed = 0.5 + random() * 0.2;
      auto cnum = int(random()*7)+1;
      auto cden = cnum+int(random()*7)+2;

      changeAmount =static_cast<double>(cnum)/cden;
    }

  }

  void BfxrParams::generateLaserShoot()
  {
    resetParams();

    auto wt = static_cast<unsigned int>(random() * 3);
    if( wt == 2 && random() < 0.5) 
    {
      wt = static_cast<unsigned int>(random() * 2);
    }
    waveType = static_cast<WaveType>(wt);

    startFrequency =0.5 + random() * 0.5;
    minFrequency =startFrequency - 0.2 - random() * 0.6;

    if(minFrequency < 0.2) 
      minFrequency =0.2;

    slide = -0.15 - random() * 0.2;			

    if(random() < 0.33)
    {
      startFrequency = random() * 0.6;
      minFrequency = random() * 0.1;
      slide = -0.35 - random() * 0.3;
    }

    if(random() < 0.5) 
    {
      squareDuty = random() * 0.5;
      dutySweep = random() * 0.2;
    }
    else
    {
      squareDuty = 0.4 + random() * 0.5;
      dutySweep =- random() * 0.7;	
    }

    sustainTime = 0.1 + random() * 0.2;
    decayTime = random() * 0.4;
    if(random() < 0.5) sustainPunch = random() * 0.3;

    if(random() < 0.33)
    {
      flangerOffset = random() * 0.2;
      flangerSweep = -random() * 0.2;
    }

    if(random() < 0.5) hpFilterCutoff = random() * 0.3;
  }

  void BfxrParams::generateExplosion()
  {
    resetParams();
    waveType = WaveType::Noise;

    if(random() < 0.5)
    {
      startFrequency = 0.1 + random() * 0.4;
      slide = -0.1 + random() * 0.4;
    }
    else
    {
      startFrequency = 0.2 + random() * 0.7;
      slide = -0.2 - random() * 0.2;
    }

    startFrequency = startFrequency * startFrequency;

    if(random() < 0.2) slide = 0.0;
    if(random() < 0.33) repeatSpeed = 0.3 + random() * 0.5;

    sustainTime = 0.1 + random() * 0.3;
    decayTime = random() * 0.5;
    sustainPunch = 0.2 + random() * 0.6;

    if(random() < 0.5)
    {
      flangerOffset = -0.3 + random() * 0.9;
      flangerSweep = -random() * 0.3;
    }

    if(random() < 0.33)
    {
      changeSpeed = 0.6 + random() * 0.3;
      changeAmount = 0.8 - random() * 1.6;
    }
  }

  void BfxrParams::generatePowerup()
  {
    resetParams();

    if(random() < 0.5) waveType = WaveType::Saw;
    else 					squareDuty = random() * 0.6;

    if(random() < 0.5)
    {
      startFrequency = 0.2 + random() * 0.3;
      slide = 0.1 + random() * 0.4;
      repeatSpeed = 0.4 + random() * 0.4;
    }
    else
    {
      startFrequency = 0.2 + random() * 0.3;
      slide = 0.05 + random() * 0.2;

      if(random() < 0.5)
      {
        vibratoDepth = random() * 0.7;
        vibratoSpeed = random() * 0.6;
      }
    }

    sustainTime = random() * 0.4;
    decayTime = 0.1 + random() * 0.4;
  }

  void BfxrParams::generateHitHurt()
  {
    resetParams();

    const auto wt = static_cast<unsigned int>(random() * 3);
    switch(wt) {
      case 0: waveType = WaveType::Square; break;
      case 1: waveType = WaveType::Saw; break;
      case 2: waveType = WaveType::Noise; break;
    }
    if(waveType == WaveType::Square) 
      squareDuty = random() * 0.6;

    startFrequency = 0.2 + random() * 0.6;
    slide = -0.3 - random() * 0.4;

    sustainTime = random() * 0.1;
    decayTime = 0.1 + random() * 0.2;

    if(random() < 0.5) hpFilterCutoff = random() * 0.3;
  }

  void BfxrParams::generateJump()
  {
    resetParams();

    waveType = WaveType::Square;
    squareDuty = random() * 0.6;
    startFrequency = 0.3 + random() * 0.3;
    slide = 0.1 + random() * 0.2;

    sustainTime = 0.1 + random() * 0.3;
    decayTime = 0.1 + random() * 0.2;

    if(random() < 0.5) hpFilterCutoff = random() * 0.3;
    if(random() < 0.5) lpFilterCutoff = 1.0 - random() * 0.6;
  }

  void BfxrParams::generateBlipSelect()
  {
    resetParams();

    waveType = (random() < 0.5)? WaveType::Square : WaveType::Saw;
    if(waveType == WaveType::Square) 
      squareDuty = random() * 0.6;

    startFrequency =0.2 + random() * 0.4;

    sustainTime = 0.1 + random() * 0.1;
    decayTime = random() * 0.2;
    hpFilterCutoff = 0.1;
  }

  void BfxrParams::resetParams()
  {
    waveType = WaveType::Square;
#define ONVAR(v) do { v =BFXR_PARAM_##v##_DEF; v##_locked = false; } while(false)
    ALLVALUES
#undef ONVAR
      masterVolume_locked = true;
  }

  void BfxrParams::mutate(double mutation)
  {			
    // should waveType be mutated... I dont think so
#define ONVAR(param) do \
    {\
      if (!param##_locked)\
      {\
        if (random()<0.5)\
        {\
          param =param + random()*mutation*2 - mutation;\
        }\
      }\
    } while(false)
    ALLVALUES
#undef ONVAR
  }

  void BfxrParams::randomize()
  {
#define ONVAR(param) do \
    {\
      if (!param##_locked)\
      {\
        const auto min = BFXR_PARAM_##param##_MIN;\
        const auto max = BFXR_PARAM_##param##_MAX;\
        const auto r = pow(random(), BFXR_PARAM_##param##_RANDOM_POWER);\
        param =min  + (max-min)*r;\
      }\
    } while(false)
    ALLVALUES
#undef ONVAR

      if (!waveType_locked)
      {
        waveType = static_cast<WaveType>(static_cast<unsigned int>(random() * static_cast<int>(WaveType::COUNT)));
      }

    if (!repeatSpeed_locked)
    {
      if (random()<0.5)
        repeatSpeed =0;
    }

    if (!slide_locked)
    {
      auto r=random()*2-1;
      r=pow(r,5);
      slide =r;
    }
    if (!deltaSlide_locked)
    {
      auto r=random()*2-1;
      r=pow(r,3);
      deltaSlide =r;
    }

    if (!minFrequency_locked)
      minFrequency =0;

    if (!startFrequency_locked)
      startFrequency =  	(random() < 0.5) ? pow(random()*2-1, 2) : (pow(random() * 0.5, 3) + 0.5);

    if ((!sustainTime_locked) && (!decayTime_locked))
    {
      if(attackTime + sustainTime + decayTime < 0.2)
      {
        sustainTime = 0.2 + random() * 0.3;
        decayTime = 0.2 + random() * 0.3;
      }
    }

    if (!slide_locked)
    {
      if((startFrequency > 0.7 && slide > 0.2) || (startFrequency < 0.2 && slide < -0.05)) 
      {
        slide = -slide;
      }
    }

    if (!lpFilterCutoffSweep_locked)
    {
      if(lpFilterCutoff < 0.1 && lpFilterCutoffSweep < -0.05) 
      {
        lpFilterCutoffSweep = -lpFilterCutoffSweep;
      }
    }
  }
} // end of sfxr param



namespace bfxr
{
  constexpr double PI = 3.14;

  double Abs(double d) {
    if(d > 0) return d;
    else return -d;
  }

  /**
   * BfxrSynth
   * 
   * Copyright 2010 Thomas Vian
   *
   * Licensed under the Apache License, Version 2.0 (the "License");
   * you may not use this file except in compliance with the License.
   * You may obtain a copy of the License at
   *
   * 	http://www.apache.org/licenses/LICENSE-2.0
   *
   * Unless required by applicable law or agreed to in writing, software
   * distributed under the License is distributed on an "AS IS" BASIS,
   * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   * See the License for the specific language governing permissions and
   * limitations under the License.
   * 
   * @author Thomas Vian
   */
  struct BfxrSynth 
  {
    BfxrSynth(const BfxrParams& p)
      : _params(p)
    {
      _finished = false;

      reset(true);
    }

    double synthOneSample() {
        if (_finished) 
        {
          return 0.0;
        }

        // Repeats every _repeatLimit times, partially resetting the sound parameters
        if(_repeatLimit != 0)
        {
          if(++_repeatTime >= _repeatLimit)
          {
            _repeatTime = 0;
            reset(false);
          }
        }

        _changePeriodTime++;
        if (_changePeriodTime>=_changePeriod)
        {				
          _changeTime=0;
          _changeTime2=0;
          _changePeriodTime=0;
          if (_changeReached)
          {
            _period /= _changeAmount;
            _changeReached=false;
          }
          if (_changeReached2)
          {
            _period /= _changeAmount2;
            _changeReached2=false;
          }
        }

        // If _changeLimit is reached, shifts the pitch
        if(!_changeReached)
        {
          if(++_changeTime >= _changeLimit)
          {
            _changeReached = true;
            _period *= _changeAmount;
          }
        }

        // If _changeLimit is reached, shifts the pitch
        if(!_changeReached2)
        {
          if(++_changeTime2 >= _changeLimit2)
          {
            _period *= _changeAmount2;
            _changeReached2=true;
          }
        }

        // Acccelerate and apply slide
        _slide += _deltaSlide;
        _period *= _slide;

        // Checks for frequency getting too low, and stops the sound if a minFrequency was set
        if(_period > _maxPeriod)
        {
          _period = _maxPeriod;
          if(_minFreqency > 0.0) {
            _muted = true;
          }										
        }

        _periodTemp = _period;

        // Applies the vibrato effect
        if(_vibratoAmplitude > 0.0)
        {
          _vibratoPhase += _vibratoSpeed;
          _periodTemp = _period * (1.0 + std::sin(_vibratoPhase) * _vibratoAmplitude);
        }

        _periodTemp = int(_periodTemp);
        if(_periodTemp < 8) _periodTemp = 8;

        // Sweeps the square duty
        if (_waveType == WaveType::Square)
        {
          _squareDuty += _dutySweep;
          if(_squareDuty < 0.0) _squareDuty = 0.0;
          else if (_squareDuty > 0.5) _squareDuty = 0.5;
        }

        // Moves through the different stages of the volume envelope
        if(++_envelopeTime > _envelopeLength)
        {
          _envelopeTime = 0;

          switch(++_envelopeStage)
          {
            case 1: _envelopeLength = _envelopeLength1; break;
            case 2: _envelopeLength = _envelopeLength2; break;
          }
        }

        // Sets the volume based on the position in the envelope
        switch(_envelopeStage)
        {
          case 0: _envelopeVolume = _envelopeTime * _envelopeOverLength0; 									break;
          case 1: _envelopeVolume = 1.0 + (1.0 - _envelopeTime * _envelopeOverLength1) * 2.0 * _sustainPunch; break;
          case 2: _envelopeVolume = 1.0 - _envelopeTime * _envelopeOverLength2; 								break;
          case 3: _envelopeVolume = 0.0; _finished = true; 													break;
        }

        // Moves the flanger offset
        if (_flanger)
        {
          _flangerOffset += _flangerDeltaOffset;
          _flangerInt = int(_flangerOffset);
          if(_flangerInt < 0) 	_flangerInt = -_flangerInt;
          else if (_flangerInt > 1023) _flangerInt = 1023;
        }

        // Moves the high-pass filter cutoff
        if(_filters && _hpFilterDeltaCutoff != 0.0)
        {
          _hpFilterCutoff *= _hpFilterDeltaCutoff;
          if(_hpFilterCutoff < 0.00001) 	_hpFilterCutoff = 0.00001;
          else if(_hpFilterCutoff > 0.1) 		_hpFilterCutoff = 0.1;
        }

        double _superSample = 0.0;
        for(int j= 0; j < 8; j++)
        {
          // Cycles through the period
          _phase++;
          if(_phase >= _periodTemp)
          {
            _phase = _phase - _periodTemp; // todo: int double operation stored in int hrm...

            // Generates new random noise for this period
            if(_waveType == WaveType::Noise) 
            { 
              for(unsigned int n= 0; n < 32; n++) _noiseBuffer[n] = random() * 2.0 - 1.0;
            }
            else if (_waveType == WaveType::Pink)
            {
              for(unsigned int n = 0; n < 32; n++) _pinkNoiseBuffer[n] = _pinkNumber.GetNextValue() * 2.0 - 1.0;
            }
            else if (_waveType == WaveType::Tan)
            {
              for(unsigned int n = 0; n < 32; n++) _loResNoiseBuffer[n] = ((n%LoResNoisePeriod)==0) ? random()*2.0-1.0 : _loResNoiseBuffer[n-1];							
            }
            else if (_waveType == WaveType::OneBitNoise)
            {
              // Based on SN76489 periodic "white" noise
              // http://www.smspower.org/Development/SN76489?sid=ae16503f2fb18070f3f40f2af56807f1#NoiseChannel
              // This one matches the behaviour of the SN76489 in the BBC Micro.
              const int feedBit = (_oneBitNoiseState >> 1 & 1) ^ (_oneBitNoiseState & 1);
              _oneBitNoiseState = _oneBitNoiseState >> 1 | (feedBit << 14);
              _oneBitNoise = (~_oneBitNoiseState & 1) - 0.5;
            }
            else if (_waveType == WaveType::Buzz)
            {
              // Based on SN76489 periodic "white" noise
              // http://www.smspower.org/Development/SN76489?sid=ae16503f2fb18070f3f40f2af56807f1#NoiseChannel
              // This one doesn't match the behaviour of anything real, but it made a nice sound, so I kept it.
              const int feedBit = (_buzzState >> 3 & 1) ^ (_buzzState & 1);
              _buzzState = _buzzState >> 1 | (feedBit << 14);
              _buzz = (~_buzzState & 1) - 0.5;
            }
          }

          double _sample=0;
          double overtonestrength=1;
          for (int k=0;k<=_overtones;k++)
          {
            double tempphase= fmod((_phase*(k+1)),_periodTemp);
            // Gets the sample from the oscillator
            switch(_waveType)
            {
              case WaveType::Square:
                {
                  _sample += overtonestrength*(((tempphase / _periodTemp) < _squareDuty) ? 0.5 : -0.5);
                  break;
                }
              case WaveType::Saw:
                {
                  _sample += overtonestrength*(1.0 - (tempphase / _periodTemp) * 2.0);
                  break;
                }
              case WaveType::Sin:
                {								
                  _pos = tempphase / _periodTemp;
                  _pos = _pos > 0.5 ? (_pos - 1.0) * 6.28318531 : _pos * 6.28318531;
                  double _tempsample= _pos < 0 ? 1.27323954 * _pos + .405284735 * _pos * _pos : 1.27323954 * _pos - 0.405284735 * _pos * _pos;
                  _sample += overtonestrength*(_tempsample < 0 ? .225 * (_tempsample *-_tempsample - _tempsample) + _tempsample : .225 * (_tempsample * _tempsample - _tempsample) + _tempsample);								
                  break;
                }
              case WaveType::Noise:
                {
                  _sample += overtonestrength*(_noiseBuffer[static_cast<unsigned int>(tempphase * 32 / int(_periodTemp))%32]);
                  break;
                }
              case WaveType::Triangle:
                {						
                  _sample += overtonestrength*(Abs(1-(tempphase / _periodTemp)*2)-1);
                  break;
                }
              case WaveType::Pink:
                {						
                  _sample += overtonestrength*(_pinkNoiseBuffer[static_cast<unsigned int>(tempphase * 32 / int(_periodTemp))%32]);
                  break;
                }
              case WaveType::Tan:
                {
                  //detuned
                  _sample += tan(PI*tempphase/_periodTemp)*overtonestrength;
                  break;
                }
              case WaveType::Whistle:
                {				
                  // Sin wave code
                  _pos = tempphase / _periodTemp;
                  _pos = _pos > 0.5 ? (_pos - 1.0) * 6.28318531 : _pos * 6.28318531;
                  double _tempsample = _pos < 0 ? 1.27323954 * _pos + .405284735 * _pos * _pos : 1.27323954 * _pos - 0.405284735 * _pos * _pos;
                  double value= 0.75*(_tempsample < 0 ? .225 * (_tempsample *-_tempsample - _tempsample) + _tempsample : .225 * (_tempsample * _tempsample - _tempsample) + _tempsample);
                  //then whistle (essentially an overtone with frequencyx20 and amplitude0.25

                  _pos = fmod((tempphase*20) , _periodTemp) / _periodTemp;
                  _pos = _pos > 0.5 ? (_pos - 1.0) * 6.28318531 : _pos * 6.28318531;
                  _tempsample = _pos < 0 ? 1.27323954 * _pos + .405284735 * _pos * _pos : 1.27323954 * _pos - 0.405284735 * _pos * _pos;
                  value += 0.25*(_tempsample < 0 ? .225 * (_tempsample *-_tempsample - _tempsample) + _tempsample : .225 * (_tempsample * _tempsample - _tempsample) + _tempsample);

                  _sample += overtonestrength*value;//main wave

                  break;
                }
              case WaveType::Breaker:
                {	
                  double amp= tempphase/_periodTemp;								
                  _sample += overtonestrength*(Abs(1-amp*amp*2)-1);
                  break;
                }
              case WaveType::OneBitNoise: // 1-bit periodic "white" noise
                {
                  _sample += overtonestrength*_oneBitNoise;
                  break;
                }
              case WaveType::Buzz: // 1-bit periodic "buzz" noise
                {
                  _sample += overtonestrength*_buzz;
                  break;
                }
              case WaveType::COUNT:
                assert(0 && "invalid case");
                break;
            }
            overtonestrength*=(1-_overtoneFalloff);

          }					

          // Applies the low and high pass filters
          if (_filters)
          {
            _lpFilterOldPos = _lpFilterPos;
            _lpFilterCutoff *= _lpFilterDeltaCutoff;
            if(_lpFilterCutoff < 0.0) _lpFilterCutoff = 0.0;
            else if(_lpFilterCutoff > 0.1) _lpFilterCutoff = 0.1;

            if(_lpFilterOn)
            {
              _lpFilterDeltaPos += (_sample - _lpFilterPos) * _lpFilterCutoff;
              _lpFilterDeltaPos *= _lpFilterDamping;
            }
            else
            {
              _lpFilterPos = _sample;
              _lpFilterDeltaPos = 0.0;
            }

            _lpFilterPos += _lpFilterDeltaPos;

            _hpFilterPos += _lpFilterPos - _lpFilterOldPos;
            _hpFilterPos *= 1.0 - _hpFilterCutoff;
            _sample = _hpFilterPos;
          }

          // Applies the flanger effect
          if (_flanger)
          {
            _flangerBuffer[_flangerPos&1023] = _sample;
            _sample += _flangerBuffer[(_flangerPos - _flangerInt + 1024) & 1023];
            _flangerPos = (_flangerPos + 1) & 1023;
          }

          _superSample += _sample;
        }

        // Clipping if too loud
        if(_superSample > 8.0) 	_superSample = 8.0;
        else if(_superSample < -8.0) 	_superSample = -8.0;					 				 				

        // Averages out the super samples and applies volumes
        _superSample = _masterVolume * _envelopeVolume * _superSample * 0.125;				


        //BIT CRUSH				
        _bitcrush_phase+=_bitcrush_freq;
        if (_bitcrush_phase>1)
        {
          _bitcrush_phase=0;
          _bitcrush_last=_superSample;	 
        }
        _bitcrush_freq = std::max(std::min(_bitcrush_freq+_bitcrush_freq_sweep,1.0),0.0);

        _superSample=_bitcrush_last; 				



        //compressor

        if (_superSample>0)
        {
          _superSample = pow(_superSample,_compression_factor);
        }
        else
        {
          _superSample = -pow(-_superSample,_compression_factor);
        }

        if (_muted)
        {
          _superSample = 0;
        }

        return _superSample;
    }

    void clampTotalLength()
    {
      auto& p = _params;
      const auto totalTime = p.attackTime + p.sustainTime + p.decayTime;
      if (totalTime < MIN_LENGTH ) 
      {
        const auto multiplier = MIN_LENGTH / totalTime;
        p.attackTime =p.attackTime * multiplier;
        p.sustainTime =p.sustainTime * multiplier;
        p.decayTime =p.decayTime * multiplier;
      }
    }

    /**
     * Resets the runing variables from the params
     * Used once at the start (total reset) and for the repeat effect (partial reset)
     * @param	totalReset	If the reset is total
     */
    void reset(bool totalReset)
    {
#if 1
      auto& p = _params;

      _period = 100.0 / (p.startFrequency * p.startFrequency + 0.001);
      _maxPeriod = 100.0 / (p.minFrequency * p.minFrequency + 0.001);


      _slide = 1.0 - p.slide * p.slide * p.slide * 0.01;
      _deltaSlide = -p.deltaSlide * p.deltaSlide * p.deltaSlide * 0.000001;

      if (p.waveType == WaveType::Square)
      {
        _squareDuty = 0.5 - p.squareDuty * 0.5;
        _dutySweep = -p.dutySweep * 0.00005;
      }

      // removed a call to max(x) with a single arg
      _changePeriod = (((1-p.changeRepeat)+0.1)/1.1) * 20000 + 32;
      _changePeriodTime = 0;

      if (p.changeAmount > 0.0) 	_changeAmount = 1.0 - p.changeAmount * p.changeAmount * 0.9;
      else 						_changeAmount = 1.0 + p.changeAmount * p.changeAmount * 10.0;

      _changeTime = 0;
      _changeReached=false;

      if(p.changeSpeed == 1.0) 	_changeLimit = 0;
      else 						_changeLimit = (1.0 - p.changeSpeed) * (1.0 - p.changeSpeed) * 20000 + 32;


      if (p.changeAmount2 > 0.0) 	_changeAmount2 = 1.0 - p.changeAmount2 * p.changeAmount2 * 0.9;
      else 						_changeAmount2 = 1.0 + p.changeAmount2 * p.changeAmount2 * 10.0;


      _changeTime2 = 0;			
      _changeReached2=false;

      if(p.changeSpeed2 == 1.0) 	_changeLimit2 = 0;
      else 						_changeLimit2 = (1.0 - p.changeSpeed2) * (1.0 - p.changeSpeed2) * 20000 + 32;

      _changeLimit*=(1-p.changeRepeat+0.1)/1.1;
      _changeLimit2*=(1-p.changeRepeat+0.1)/1.1;

      if(totalReset)
      {
        _masterVolume = p.masterVolume * p.masterVolume;

        _waveType = p.waveType;

        if (p.sustainTime < 0.01) p.sustainTime = 0.01;

        clampTotalLength();

        _sustainPunch = p.sustainPunch;

        _phase = 0;

        _minFreqency = p.minFrequency;
        _muted=false;
        _overtones = p.overtones*10;
        _overtoneFalloff = p.overtoneFalloff;

        _bitcrush_freq = 1 - pow(p.bitCrush,1.0/3.0);				
        _bitcrush_freq_sweep = -p.bitCrushSweep* 0.000015;
        _bitcrush_phase=0;
        _bitcrush_last=0;				

        _compression_factor = 1/(1+4*p.compressionAmount);

        _filters = p.lpFilterCutoff != 1.0 || p.hpFilterCutoff != 0.0;				

        _lpFilterPos = 0.0;
        _lpFilterDeltaPos = 0.0;
        _lpFilterCutoff = p.lpFilterCutoff * p.lpFilterCutoff * p.lpFilterCutoff * 0.1;
        _lpFilterDeltaCutoff = 1.0 + p.lpFilterCutoffSweep * 0.0001;
        _lpFilterDamping = 5.0 / (1.0 + p.lpFilterResonance * p.lpFilterResonance * 20.0) * (0.01 + _lpFilterCutoff);
        if (_lpFilterDamping > 0.8) _lpFilterDamping = 0.8;
        _lpFilterDamping = 1.0 - _lpFilterDamping;
        _lpFilterOn = p.lpFilterCutoff != 1.0;

        _hpFilterPos = 0.0;
        _hpFilterCutoff = p.hpFilterCutoff * p.hpFilterCutoff * 0.1;
        _hpFilterDeltaCutoff = 1.0 + p.hpFilterCutoffSweep * 0.0003;

        _vibratoPhase = 0.0;
        _vibratoSpeed = p.vibratoSpeed * p.vibratoSpeed * 0.01;
        _vibratoAmplitude = p.vibratoDepth * 0.5;

        _envelopeVolume = 0.0;
        _envelopeStage = 0;
        _envelopeTime = 0;
        _envelopeLength0 = p.attackTime * p.attackTime * 100000.0;
        _envelopeLength1 = p.sustainTime * p.sustainTime * 100000.0;
        _envelopeLength2 = p.decayTime * p.decayTime * 100000.0 + 10;
        _envelopeLength = _envelopeLength0;
        _envelopeFullLength = _envelopeLength0 + _envelopeLength1 + _envelopeLength2;

        _envelopeOverLength0 = 1.0 / _envelopeLength0;
        _envelopeOverLength1 = 1.0 / _envelopeLength1;
        _envelopeOverLength2 = 1.0 / _envelopeLength2;

        _flanger = p.flangerOffset != 0.0 || p.flangerSweep != 0.0;

        _flangerOffset = p.flangerOffset * p.flangerOffset * 1020.0;
        if(p.flangerOffset < 0.0) _flangerOffset = -_flangerOffset;
        _flangerDeltaOffset = p.flangerSweep * p.flangerSweep * p.flangerSweep * 0.2;
        _flangerPos = 0;

        _flangerBuffer.reserve(1024);
        _noiseBuffer.reserve(32);
        _pinkNoiseBuffer.reserve(32);
        _loResNoiseBuffer.reserve(32);

        _oneBitNoiseState = 1 << 14;
        _oneBitNoise = 0;
        _buzzState = 1 << 14;
        _buzz = 0;

        for(unsigned int i= 0; i < 1024; i++) _flangerBuffer[i] = 0.0;
        for(unsigned int i = 0; i < 32; i++) _noiseBuffer[i] = random() * 2.0 - 1.0;
        for(unsigned int i = 0; i < 32; i++) _pinkNoiseBuffer[i] = _pinkNumber.GetNextValue() * 2.0 - 1.0;
        for(unsigned int i = 0; i < 32; i++) _loResNoiseBuffer[i] = ((i%LoResNoisePeriod)==0) ? random()*2.0-1.0 : _loResNoiseBuffer[i-1];							

        _repeatTime = 0;

        if (p.repeatSpeed == 0.0) 	_repeatLimit = 0;
        else 						_repeatLimit = int((1.0-p.repeatSpeed) * (1.0-p.repeatSpeed) * 20000) + 32;
      }
#endif
    }

    unsigned int GetNumberOfSamples()
    {
      // If the sound is smaller than the buffer length, add silence to allow it to play
      return std::max<unsigned int>(1536, _envelopeFullLength);
    }

    void GenerateSound(std::vector<double>* data)
    {
      const auto samples = GetNumberOfSamples();
      for(unsigned int i=0; i< samples; i+=1) {
        data->push_back(synthOneSample());
      }
    }

    static constexpr int LoResNoisePeriod= 8;
    static constexpr double MIN_LENGTH = 0.18;

    BfxrParams _params;	// Params instance

    //--------------------------------------------------------------------------
    //
    //  Synth Variables
    //
    //--------------------------------------------------------------------------

    bool _finished;						// If the sound has finished

    double _masterVolume;					// masterVolume * masterVolume (for quick calculations)

    WaveType _waveType;							// The type of wave to generate

    double _envelopeVolume;					// Current volume of the envelope
    int _envelopeStage;						// Current stage of the envelope (attack, sustain, decay, end)
    double _envelopeTime;					// Current time through current enelope stage
    double _envelopeLength;					// Length of the current envelope stage
    double _envelopeLength0;				// Length of the attack stage
    double _envelopeLength1;				// Length of the sustain stage
    double _envelopeLength2;				// Length of the decay stage
    double _envelopeOverLength0;			// 1 / _envelopeLength0 (for quick calculations)
    double _envelopeOverLength1;			// 1 / _envelopeLength1 (for quick calculations)
    double _envelopeOverLength2;			// 1 / _envelopeLength2 (for quick calculations)
    double _envelopeFullLength;				// Full length of the volume envelop (and therefore sound)

    double _sustainPunch;					// The punch factor (louder at begining of sustain)

    int _phase;								// Phase through the wave
    double _pos;							// Phase expresed as a Number from 0-1, used for fast sin approx
    double _period;							// Period of the wave
    double _periodTemp;						// Period modified by vibrato
    double _maxPeriod;						// Maximum period before sound stops (from minFrequency)

    double _slide;							// Note slide
    double _deltaSlide;						// Change in slide
    double _minFreqency;					// Minimum frequency before stopping
    bool _muted;							// Whether or not min frequency has been attained


    int _overtones;					// Minimum frequency before stopping
    double _overtoneFalloff;					// Minimum frequency before stopping

    double _vibratoPhase;					// Phase through the vibrato sine wave
    double _vibratoSpeed;					// Speed at which the vibrato phase moves
    double _vibratoAmplitude;				// Amount to change the period of the wave by at the peak of the vibrato wave

    double _changePeriod;
    int _changePeriodTime;

    double _changeAmount;					// Amount to change the note by
    int _changeTime;						// Counter for the note change
    int _changeLimit;						// Once the time reaches this limit, the note changes
    bool _changeReached;

    double _changeAmount2;					// Amount to change the note by
    int _changeTime2;						// Counter for the note change
    int _changeLimit2;						// Once the time reaches this limit, the note changes
    bool _changeReached2;


    double _squareDuty;						// Offset of center switching point in the square wave
    double _dutySweep;						// Amount to change the duty by

    int _repeatTime;						// Counter for the repeats
    int _repeatLimit;						// Once the time reaches this limit, some of the variables are reset

    bool _flanger;						// If the flanger is active
    double _flangerOffset;					// Phase offset for flanger effect
    double _flangerDeltaOffset;				// Change in phase offset
    int _flangerInt;							// Integer flanger offset, for bit maths
    int _flangerPos;							// Position through the flanger buffer
    std::vector<double> _flangerBuffer;			// Buffer of wave values used to create the out of phase second wave

    bool _filters;						// If the filters are active
    double _lpFilterPos;					// Adjusted wave position after low-pass filter
    double _lpFilterOldPos;					// Previous low-pass wave position
    double _lpFilterDeltaPos;				// Change in low-pass wave position, as allowed by the cutoff and damping
    double _lpFilterCutoff;					// Cutoff multiplier which adjusts the amount the wave position can move
    double _lpFilterDeltaCutoff;			// Speed of the low-pass cutoff multiplier
    double _lpFilterDamping;				// Damping muliplier which restricts how fast the wave position can move
    bool _lpFilterOn;					// If the low pass filter is active

    double _hpFilterPos;					// Adjusted wave position after high-pass filter
    double _hpFilterCutoff;					// Cutoff multiplier which adjusts the amount the wave position can move
    double _hpFilterDeltaCutoff;			// Speed of the high-pass cutoff multiplier

    std::vector<double> _noiseBuffer;			// Buffer of random values used to generate noise
    std::vector<double> _pinkNoiseBuffer;			// Buffer of random values used to generate noise
    std::vector<double> _loResNoiseBuffer;			// Buffer of random values used to generate noise

    int _oneBitNoiseState;					// Buffer containing one-bit periodic noise state.
    double _oneBitNoise;					// Current sample of one-bit noise.

    int _buzzState;							// Buffer containing 'buzz' periodic noise state.
    double _buzz;							// Current sample of 'buzz' noise.

    PinkNoise _pinkNumber;

    // double _superSample;					// Actual sample writen to the wave
    // double _sample;							// Sub-sample calculated 8 times per actual sample, averaged out to get the super sample
    // unsigned int _sampleCount;						// Number of samples added to the buffer sample
    // double _bufferSample;					// Another supersample used to create a 22050Hz wave

    double _bitcrush_freq;					// inversely proportional to the number of samples to skip 
    double _bitcrush_freq_sweep;			// change of the above
    double _bitcrush_phase;					// samples when this > 1
    double _bitcrush_last;					// last sample value

    double _compression_factor;
  };

  void GenerateSound(const BfxrParams& params, std::vector<double>* data)
  {
    BfxrSynth synth{params};
    synth.GenerateSound(data);
  }




  bool SaveWav(const char* filename, const std::vector<double>& data)
  {
    const int wav_bits=16;
    const int wav_freq=44100;

    FILE* foutput=fopen(filename, "wb");
    if(!foutput)
      return false;
    // write wav header
    unsigned int dword=0;
    unsigned short word=0;
    fwrite("RIFF", 4, 1, foutput); // "RIFF"
    dword=0;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fwrite("WAVE", 4, 1, foutput); // "WAVE"

    fwrite("fmt ", 4, 1, foutput); // "fmt "
    dword=16;
    fwrite(&dword, 1, 4, foutput); // chunk size
    word=1;
    fwrite(&word, 1, 2, foutput); // compression code
    word=1;
    fwrite(&word, 1, 2, foutput); // channels
    dword=wav_freq;
    fwrite(&dword, 1, 4, foutput); // sample rate
    dword=wav_freq*wav_bits/8;
    fwrite(&dword, 1, 4, foutput); // bytes/sec
    word=wav_bits/8;
    fwrite(&word, 1, 2, foutput); // block align
    word=wav_bits;
    fwrite(&word, 1, 2, foutput); // bits per sample

    fwrite("data", 4, 1, foutput); // "data"
    dword=0;
    int foutstream_datasize=ftell(foutput);
    fwrite(&dword, 1, 4, foutput); // chunk size

    // write sample data
    for(auto sample: data)
    {
      auto ssample = sample;
			if(ssample>1.0) ssample=1.0;
			if(ssample<-1.0) ssample=-1.0;
			assert(wav_freq==44100);
      const auto filesample = ssample;
      if(wav_bits==16)
      {
        short isample=(short)(filesample*32000);
        fwrite(&isample, 1, 2, foutput);
      }
      else
      {
        unsigned char isample=(unsigned char)(filesample*127+128);
        fwrite(&isample, 1, 1, foutput);
      }
    }
    const auto file_sampleswritten = data.size();

    // seek back to header and write size info
    fseek(foutput, 4, SEEK_SET);
    dword=0;
    dword=foutstream_datasize-4+file_sampleswritten*wav_bits/8;
    fwrite(&dword, 1, 4, foutput); // remaining file size
    fseek(foutput, foutstream_datasize, SEEK_SET);
    dword=file_sampleswritten*wav_bits/8;
    fwrite(&dword, 1, 4, foutput); // chunk size (data)
    fclose(foutput);
    
    return true;
  }

}

#endif // BFXR_IMPLEMENTATION

#endif  // BFXR_H

