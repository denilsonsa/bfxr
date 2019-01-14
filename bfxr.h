#ifndef SFXPARAMS_H
#define SFXPARAMS_H

// ----------------------------------------------------------------------
// Documentation section
// ----------------------------------------------------------------------

// todo: add documentation on how to use the library

// ----------------------------------------------------------------------
// Include section
// ----------------------------------------------------------------------

#include <vector>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>



// ----------------------------------------------------------------------
// Header section
// ----------------------------------------------------------------------


namespace Synthesizer
{
  double random();
  
/*
class taken from http://www.firstpr.com.au/dsp/pink-noise/#Filtering
*/
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

namespace Synthesizer   
{
	/**
	 * SfxrParams
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
    COUNT
  };
			
	class SfxrParams 
	{
    public:

    struct Param {
      // real name, decription, grouping, default, min, max, 
      Param(const std::string& name, const std::string& desc, int grouping, double def, double min, double max)
        : real_name(name)
          , description(desc)
          , default_value(def)
          , min_value(min)
          , max_value(max)
      {
      }

      std::string real_name;
      std::string description;
      double default_value;
      double min_value;
      double max_value;

      double random_power = 1.0;

      double get() const { return current_value; }
      void set(double t)
      {
        if(t > max_value) { current_value = max_value; }
        else if(t < min_value) { current_value = min_value; }
        else  { current_value = t; }
      }
      bool locked = false;
      double current_value = 0;
    };

		WaveType waveType;
    bool waveType_locked;
		
		Param masterVolume;
		Param attackTime;
		Param sustainTime;
		Param sustainPunch;
		Param decayTime;
		
		Param compressionAmount;
		
		Param startFrequency;
		Param minFrequency;
		
		Param slide;
		Param deltaSlide;
		
		Param vibratoDepth;
		Param vibratoSpeed;
		
		Param overtones;
		Param overtoneFalloff;
		
		Param changeRepeat;
		
		Param changeAmount;
		Param changeSpeed;
		
		Param changeAmount2;
		Param changeSpeed2;
		
		Param squareDuty;
		Param dutySweep;
		
		Param repeatSpeed;
		
		Param flangerOffset;
		Param flangerSweep;
		
		Param lpFilterCutoff;
		Param lpFilterCutoffSweep;
		Param lpFilterResonance;
		
		Param hpFilterCutoff;
		Param hpFilterCutoffSweep;
						
		Param bitCrush;
		Param bitCrushSweep;
		
		static constexpr int WAVETYPECOUNT = static_cast<int>(WaveType::COUNT);
		
		SfxrParams();

		void setAllLocked(bool locked);
    std::vector<Param*> GetParams();
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
  };
}


namespace Synthesizer 
{
		unsigned int GenerateSound(const SfxrParams& params, std::vector<double>* data);
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

namespace Synthesizer
{
    double random()
    {
      return rand() / static_cast<double>(RAND_MAX);
    }
		
    PinkNumber::PinkNumber()
		{
			max_key = 0x1f; // Five bits set
			range = 128;
			key = 0;
			for (int i = 0; i < 5; i++)
				white_values.push_back(random() * (range/5.0f));
		}
		
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
				// If bit changed get new random number for corresponding white_value
				if (diff & (1 << i))
					white_values[i] = random() * (range/5.0f);
				sum += white_values[i];
			}
			return sum/64.0f-1.0f;
		}
}


namespace Synthesizer   
{
  double random();

	/**
	 * SfxrParams
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
		
  SfxrParams::SfxrParams()
      // real name, decription, grouping, default, min, max, 
      : masterVolume ( "Master Volume","Overall volume of the sound.", 1,0.5,0,1) 	
      , attackTime ( "Attack Time","Length of the volume envelope attack.", 1,0,0,1)		
      , sustainTime ( "Sustain Time","Length of the volume envelope sustain.", 1,0.3,0,1) 	
      , sustainPunch ( "Punch","Tilts the sustain envelope for more 'pop'.", 1,0,0,1) 		
      , decayTime ( "Decay Time","Length of the volume envelope decay (yes, I know it's called release).", 1,0.4,0,1) 	
      , compressionAmount ( "Compression","Pushes amplitudes together into a narrower range to make them stand out more.  Very good for sound effects, where you want them to stick out against background music.", 15,0.3,0,1)
      , startFrequency ( "Frequency","Base note of the sound.", 2,0.3,0,1) 		
      , minFrequency ( "Frequency Cutoff","If sliding, the sound will stop at this frequency, to prevent really low notes.  If unlocked, this is set to zero during randomization.", 2,0.0,0,1) 		
      , slide ( "Frequency Slide","Slides the frequency up or down.", 3,0.0,-1,1) 	
      , deltaSlide ( "Delta Slide","Accelerates the frequency slide.  Can be used to get the frequency to change direction.", 3,0.0,-1,1) 		
      , vibratoDepth ( "Vibrato Depth","Strength of the vibrato effect.", 4,0,0,1) 		
      , vibratoSpeed ( "Vibrato Speed","Speed of the vibrato effect (i.e. frequency).", 4,0,0,1) 		
      , overtones ( "Harmonics","Overlays copies of the waveform with copies and multiples of its frequency.  Good for bulking out or otherwise enriching the texture of the sounds (warning: this is the number 1 cause of bfxr slowdown!).", 13,0,0,1) 		
      , overtoneFalloff ( "Harmonics Falloff","The rate at which higher overtones should decay.", 13,0,0,1) 
      , changeRepeat ( "Pitch Jump Repeat Speed","Larger Values means more pitch jumps, which can be useful for arpeggiation.", 5,0,0,1) 		
      , changeAmount ( "Pitch Jump Amount 1","Jump in pitch, either up or down.", 5,0,-1,1) 		
      , changeSpeed ( "Pitch Jump Onset 1","How quickly the note shift happens.", 5,0,0,1) 		
      , changeAmount2 ( "Pitch Jump Amount 2","Jump in pitch, either up or down.", 5,0,-1,1) 	
      , changeSpeed2 ( "Pitch Jump Onset 2","How quickly the note shift happens.", 5,0,0,1) 		
      , squareDuty ( "Square Duty","Square waveform only : Controls the ratio between the up and down states of the square wave, changing the tibre.", 8,0,0,1) 		
      , dutySweep ( "Duty Sweep","Square waveform only : Sweeps the duty up or down.", 8,0,-1,1) 		
      , repeatSpeed ( "Repeat Speed","Speed of the note repeating - certain variables are reset each time.", 9,0,0,1) 	
      , flangerOffset ( "Flanger Offset","Offsets a second copy of the wave by a small phase, changing the tibre.", 10,0,-1,1) 		
      , flangerSweep ( "Flanger Sweep","Sweeps the phase up or down.", 10,0,-1,1) 
      , lpFilterCutoff ( "Low-pass Filter Cutoff","Frequency at which the low-pass filter starts attenuating higher frequencies.  Named most likely to result in 'Huh why can't I hear anything?' at her high-school grad. ", 11,1,0,1) 		
      , lpFilterCutoffSweep ( "Low-pass Filter Cutoff Sweep","Sweeps the low-pass cutoff up or down.", 11,0,-1,1) 	
      , lpFilterResonance ( "Low-pass Filter Resonance","Changes the attenuation rate for the low-pass filter, changing the timbre.", 11,0,0,1) 		
      , hpFilterCutoff ( "High-pass Filter Cutoff","Frequency at which the high-pass filter starts attenuating lower frequencies.", 12,0,0,1) 	
      , hpFilterCutoffSweep ( "High-pass Filter Cutoff Sweep","Sweeps the high-pass cutoff up or down.", 12,0,-1,1) 	
      , bitCrush ( "Bit Crush","Resamples the audio at a lower frequency.", 14,0,0,1)
      , bitCrushSweep ( "Bit Crush Sweep","Sweeps the Bit Crush filter up or down.", 14,0,-1,1) 
		{
			resetParams();

      attackTime.random_power = 4;
      sustainTime.random_power = 2;
      sustainPunch.random_power = 2;
      overtones.random_power = 3;
      overtoneFalloff.random_power = 0.25;
      vibratoDepth.random_power = 3;
      dutySweep.random_power = 3;
      flangerOffset.random_power = 3;
      flangerSweep.random_power = 3;
      lpFilterCutoff.random_power = 0.3;
      // lpFilterCutoffSweep.random_power = 3;
      hpFilterCutoff.random_power = 5;
      // hpFilterCutoffSweep.random_power = 5;
      bitCrush.random_power = 4;
      bitCrushSweep.random_power = 5;
		}

		void SfxrParams::setAllLocked(bool locked)
    {
      waveType_locked = locked;
      auto params = GetParams();
      for(auto* p: params)
      {
        p->locked = locked;
      }
    }
		
    std::vector<SfxrParams::Param*> SfxrParams::GetParams()
		{
      std::vector<Param*> ret;

      ret.push_back(&masterVolume);
      ret.push_back(&attackTime);
      ret.push_back(&sustainTime);
      ret.push_back(&sustainPunch);
      ret.push_back(&decayTime);
      ret.push_back(&compressionAmount);
      ret.push_back(&startFrequency);
      ret.push_back(&minFrequency);
      ret.push_back(&slide);
      ret.push_back(&deltaSlide);
      ret.push_back(&vibratoDepth);
      ret.push_back(&vibratoSpeed);
      ret.push_back(&overtones);
      ret.push_back(&overtoneFalloff);
      ret.push_back(&changeRepeat);
      ret.push_back(&changeAmount);
      ret.push_back(&changeSpeed);
      ret.push_back(&changeAmount2);
      ret.push_back(&changeSpeed2);
      ret.push_back(&squareDuty);
      ret.push_back(&dutySweep);
      ret.push_back(&repeatSpeed);
      ret.push_back(&flangerOffset);
      ret.push_back(&flangerSweep);
      ret.push_back(&lpFilterCutoff);
      ret.push_back(&lpFilterCutoffSweep);
      ret.push_back(&lpFilterResonance);
      ret.push_back(&hpFilterCutoff);
      ret.push_back(&hpFilterCutoffSweep);
      ret.push_back(&bitCrush);
      ret.push_back(&bitCrushSweep);

      return ret;
		}
		
		void SfxrParams::generatePickupCoin()
		{
			resetParams();
			
			startFrequency.set(0.4+random()*0.5);
			
			sustainTime.set( random() * 0.1);
			decayTime.set( 0.1 + random() * 0.4);
			sustainPunch.set( 0.3 + random() * 0.3);
			
			if(random() < 0.5) 
			{
				changeSpeed.set( 0.5 + random() * 0.2);
				auto cnum = int(random()*7)+1;
				auto cden = cnum+int(random()*7)+2;
				
				changeAmount.set(static_cast<double>(cnum)/cden);
			}
			
		}
		
		void SfxrParams::generateLaserShoot()
		{
			resetParams();
			
			auto wt = static_cast<unsigned int>(random() * 3);
			if( wt == 2 && random() < 0.5) 
			{
				wt = static_cast<unsigned int>(random() * 2);
			}
      waveType = static_cast<WaveType>(wt);
			
			startFrequency.set(0.5 + random() * 0.5);
			minFrequency.set(startFrequency.get() - 0.2 - random() * 0.6);
			
			if(minFrequency.get() < 0.2) 
				minFrequency.set(0.2);
			
			slide.set( -0.15 - random() * 0.2);			
			 
			if(random() < 0.33)
			{
				startFrequency.set( random() * 0.6);
				minFrequency.set( random() * 0.1);
				slide.set( -0.35 - random() * 0.3);
			}
			
			if(random() < 0.5) 
			{
				squareDuty.set( random() * 0.5);
				dutySweep.set( random() * 0.2);
			}
			else
			{
				squareDuty.set( 0.4 + random() * 0.5);
				dutySweep.set(- random() * 0.7);	
			}
			
			sustainTime.set( 0.1 + random() * 0.2);
			decayTime.set( random() * 0.4);
			if(random() < 0.5) sustainPunch.set( random() * 0.3);
			
			if(random() < 0.33)
			{
				flangerOffset.set( random() * 0.2);
				flangerSweep.set( -random() * 0.2);
			}
			
			if(random() < 0.5) hpFilterCutoff.set( random() * 0.3);
		}
		
		void SfxrParams::generateExplosion()
		{
			resetParams();
			waveType = WaveType::Noise;
			
			if(random() < 0.5)
			{
				startFrequency.set( 0.1 + random() * 0.4);
				slide.set( -0.1 + random() * 0.4);
			}
			else
			{
				startFrequency.set( 0.2 + random() * 0.7);
				slide.set( -0.2 - random() * 0.2);
			}
			
			startFrequency.set( startFrequency.get() * startFrequency.get());
			
			if(random() < 0.2) slide.set( 0.0);
			if(random() < 0.33) repeatSpeed.set( 0.3 + random() * 0.5);
			
			sustainTime.set( 0.1 + random() * 0.3);
			decayTime.set( random() * 0.5);
			sustainPunch.set( 0.2 + random() * 0.6);
			
			if(random() < 0.5)
			{
				flangerOffset.set( -0.3 + random() * 0.9);
				flangerSweep.set( -random() * 0.3);
			}
			
			if(random() < 0.33)
			{
				changeSpeed.set( 0.6 + random() * 0.3);
				changeAmount.set( 0.8 - random() * 1.6);
			}
		}
		
		void SfxrParams::generatePowerup()
		{
			resetParams();
			
			if(random() < 0.5) waveType = WaveType::Saw;
			else 					squareDuty.set( random() * 0.6);
			
			if(random() < 0.5)
			{
				startFrequency.set( 0.2 + random() * 0.3);
				slide.set( 0.1 + random() * 0.4);
				repeatSpeed.set( 0.4 + random() * 0.4);
			}
			else
			{
				startFrequency.set( 0.2 + random() * 0.3);
				slide.set( 0.05 + random() * 0.2);
				
				if(random() < 0.5)
				{
					vibratoDepth.set( random() * 0.7);
					vibratoSpeed.set( random() * 0.6);
				}
			}
			
			sustainTime.set( random() * 0.4);
			decayTime.set( 0.1 + random() * 0.4);
		}
		
		void SfxrParams::generateHitHurt()
		{
			resetParams();
			
			const auto wt = static_cast<unsigned int>(random() * 3);
			switch(wt) {
        case 0: waveType = WaveType::Square; break;
        case 1: waveType = WaveType::Saw; break;
        case 2: waveType = WaveType::Noise; break;
      }
			if(waveType == WaveType::Square) 
				squareDuty.set( random() * 0.6);
			
			startFrequency.set( 0.2 + random() * 0.6);
			slide.set( -0.3 - random() * 0.4);
			
			sustainTime.set( random() * 0.1);
			decayTime.set( 0.1 + random() * 0.2);
			
			if(random() < 0.5) hpFilterCutoff.set( random() * 0.3);
		}
		
		void SfxrParams::generateJump()
		{
			resetParams();
			
			waveType = WaveType::Square;
			squareDuty.set( random() * 0.6);
			startFrequency.set( 0.3 + random() * 0.3);
			slide.set( 0.1 + random() * 0.2);
			
			sustainTime.set( 0.1 + random() * 0.3);
			decayTime.set( 0.1 + random() * 0.2);
			
			if(random() < 0.5) hpFilterCutoff.set( random() * 0.3);
			if(random() < 0.5) lpFilterCutoff.set( 1.0 - random() * 0.6);
		}
		
		void SfxrParams::generateBlipSelect()
		{
			resetParams();
			
      waveType = (random() < 0.5)? WaveType::Square : WaveType::Saw;
			if(waveType == WaveType::Square) 
				squareDuty.set( random() * 0.6);
			
			startFrequency.set(0.2 + random() * 0.4);
			
			sustainTime.set( 0.1 + random() * 0.1);
			decayTime.set( random() * 0.2);
			hpFilterCutoff.set( 0.1);
		}
		
		void SfxrParams::resetParams()
		{
      waveType = WaveType::Square;
      auto params = GetParams();
      for(auto* p: params)
      {
        p->set( p->default_value);
        p->locked = false;
      }
			masterVolume.locked = true;
		}
		
		void SfxrParams::mutate(double mutation)
		{			
      // should waveType be mutated... I dont think so
      auto params = GetParams();
			for (auto* param : params)
			{
				if (!param->locked)
				{
					if (random()<0.5)
					{
						param->set(param->get() + random()*mutation*2 - mutation);
					}
				}
			}
		}
		
		void SfxrParams::randomize()
		{
			auto params = GetParams();
			for (auto* param : params)
			{
				if (!param->locked)
				{
          const auto min = param->min_value;
          const auto max = param->max_value;
          const auto r = pow(random(), param->random_power);
					param->set(min  + (max-min)*r);
				}
			}
			
			if (!waveType_locked)
			{
        waveType = static_cast<WaveType>(random() * static_cast<int>(WaveType::COUNT));
			}
			
			if (!repeatSpeed.locked)
			{
				if (random()<0.5)
					repeatSpeed.set(0);
			}
						
			if (!slide.locked)
			{
				auto r=random()*2-1;
				r=pow(r,5);
				slide.set(r);
			}
			if (!deltaSlide.locked)
			{
				auto r=random()*2-1;
				r=pow(r,3);
				deltaSlide.set(r);
			}
			
			if (!minFrequency.locked)
				minFrequency.set(0);
			
			if (!startFrequency.locked)
				startFrequency.set(  	(random() < 0.5) ? pow(random()*2-1, 2) : (pow(random() * 0.5, 3) + 0.5));
			
			if ((!sustainTime.locked) && (!decayTime.locked))
			{
				if(attackTime.get() + sustainTime.get() + decayTime.get() < 0.2)
				{
					sustainTime.set( 0.2 + random() * 0.3);
					decayTime.set( 0.2 + random() * 0.3);
				}
			}
			
			if (!slide.locked)
			{
				if((startFrequency.get() > 0.7 && slide.get() > 0.2) || (startFrequency.get() < 0.2 && slide.get() < -0.05)) 
				{
					slide.set( -slide.get());
				}
			}
			
			if (!lpFilterCutoffSweep.locked)
			{
				if(lpFilterCutoff.get() < 0.1 && lpFilterCutoffSweep.get() < -0.05) 
				{
					lpFilterCutoffSweep.set( -lpFilterCutoffSweep.get());
				}
			}
		}
} // end of sfxr param



namespace Synthesizer 
{
  constexpr double PI = 3.14;

  double Abs(double d) {
    if(d > 0) return d;
    else return -d;
  }

	/**
	 * SfxrSynth
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
  struct ByteArray{
    std::vector<double>* data = nullptr;
    void writeFloat(double d)
    {
      if(data) {
        data->emplace_back(d);
      }
      length += 1;
    }
    unsigned int length = 0;
  };

struct SfxrSynth 
	{
		/**
		 * Writes the wave to the supplied buffer ByteArray
		 * @param	buffer		A ByteArray to write the wave to
		 * @param	waveData	If the wave should be written for the waveData
		 * @return				If the wave is finished
		 */
		bool synthWave(ByteArray& buffer, unsigned int length)
		{
			_finished = false;
			
			_sampleCount = 0;
			_bufferSample = 0.0;
			
			for(unsigned int i= 0; i < length; i++)
			{
				if (_finished) 
				{
					return true;					
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
				
				_superSample = 0.0;
				for(int j= 0; j < 8; j++)
				{
					// Cycles through the period
					_phase++;
					if(_phase >= _periodTemp)
					{
						_phase = _phase - _periodTemp;
						
						// Generates new random noise for this period
						if(_waveType == WaveType::Noise) 
						{ 
							for(unsigned int n= 0; n < 32; n++) _noiseBuffer[n] = random() * 2.0 - 1.0;
						}
						else if (_waveType == WaveType::Pink)
						{
							for(unsigned int n = 0; n < 32; n++) _pinkNoiseBuffer[n] = _pinkNumber.GetNextValue();							
						}
						else if (_waveType == WaveType::Tan)
						{
							for(unsigned int n = 0; n < 32; n++) _loResNoiseBuffer[n] = ((n%LoResNoisePeriod)==0) ? random()*2.0-1.0 : _loResNoiseBuffer[n-1];							
						}
					}
					
					_sample=0;
					double overtonestrength=1;
					for (int k=0;k<=_overtones;k++)
					{
						double tempphase= fmod((_phase*(k+1)),_periodTemp);
						// Gets the sample from the oscillator
						switch(_waveType)
						{
              case WaveType::Square: // Square wave
							{
								_sample += overtonestrength*(((tempphase / _periodTemp) < _squareDuty) ? 0.5 : -0.5);
								break;
							}
              case WaveType::Saw: // Saw wave
							{
								_sample += overtonestrength*(1.0 - (tempphase / _periodTemp) * 2.0);
								break;
							}
              case WaveType::Sin: // Sine wave (fast and accurate approx)
							{								
								 _pos = tempphase / _periodTemp;
								 _pos = _pos > 0.5 ? (_pos - 1.0) * 6.28318531 : _pos * 6.28318531;
								double _tempsample= _pos < 0 ? 1.27323954 * _pos + .405284735 * _pos * _pos : 1.27323954 * _pos - 0.405284735 * _pos * _pos;
								_sample += overtonestrength*(_tempsample < 0 ? .225 * (_tempsample *-_tempsample - _tempsample) + _tempsample : .225 * (_tempsample * _tempsample - _tempsample) + _tempsample);								
								break;
							}
              case WaveType::Noise: // Noise
							{
								_sample += overtonestrength*(_noiseBuffer[uint(tempphase * 32 / int(_periodTemp))%32]);
								break;
							}
              case WaveType::Triangle: // Triangle Wave
							{						
								_sample += overtonestrength*(Abs(1-(tempphase / _periodTemp)*2)-1);
								break;
							}
              case WaveType::Pink: // Pink Noise
							{						
								_sample += overtonestrength*(_pinkNoiseBuffer[uint(tempphase * 32 / int(_periodTemp))%32]);
								break;
							}
              case WaveType::Tan: // tan
							{
								//detuned
								_sample += tan(PI*tempphase/_periodTemp)*overtonestrength;
								break;
							}
              case WaveType::Whistle: // Whistle 
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
              case WaveType::Breaker: // Breaker
							{	
								double amp= tempphase/_periodTemp;								
								_sample += overtonestrength*(Abs(1-amp*amp*2)-1);
								break;
							}
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
				 
        buffer.writeFloat(_superSample);
			}
			
			return false;
		}

    void clampTotalLength()
		{
			auto& p = _params;
			const auto totalTime = p.attackTime.get() + p.sustainTime.get() + p.decayTime.get();
			if (totalTime < MIN_LENGTH ) 
			{
				const auto multiplier = MIN_LENGTH / totalTime;
				p.attackTime.set(p.attackTime.get() * multiplier);
				p.sustainTime.set(p.sustainTime.get() * multiplier);
				p.decayTime.set(p.decayTime.get() * multiplier);
			}
		}
		
		/**
		 * Resets the runing variables from the params
		 * Used once at the start (total reset) and for the repeat effect (partial reset)
		 * @param	totalReset	If the reset is total
		 */
		void reset(bool totalReset)
		{
			auto& p = _params;
			
			_period = 100.0 / (p.startFrequency.get() * p.startFrequency.get() + 0.001);
			_maxPeriod = 100.0 / (p.minFrequency.get() * p.minFrequency.get() + 0.001);
			
			
			_slide = 1.0 - p.slide.get() * p.slide.get() * p.slide.get() * 0.01;
			_deltaSlide = -p.deltaSlide.get() * p.deltaSlide.get() * p.deltaSlide.get() * 0.000001;
			
			if (p.waveType == WaveType::Square)
			{
				_squareDuty = 0.5 - p.squareDuty.get() * 0.5;
				_dutySweep = -p.dutySweep.get() * 0.00005;
			}
			
      // removed a call to max(x) with a single arg
			_changePeriod = (((1-p.changeRepeat.get())+0.1)/1.1) * 20000 + 32;
			_changePeriodTime = 0;
			
			if (p.changeAmount.get() > 0.0) 	_changeAmount = 1.0 - p.changeAmount.get() * p.changeAmount.get() * 0.9;
			else 						_changeAmount = 1.0 + p.changeAmount.get() * p.changeAmount.get() * 10.0;
			
			_changeTime = 0;
			_changeReached=false;
			
			if(p.changeSpeed.get() == 1.0) 	_changeLimit = 0;
			else 						_changeLimit = (1.0 - p.changeSpeed.get()) * (1.0 - p.changeSpeed.get()) * 20000 + 32;
			
			
			if (p.changeAmount2.get() > 0.0) 	_changeAmount2 = 1.0 - p.changeAmount2.get() * p.changeAmount2.get() * 0.9;
			else 						_changeAmount2 = 1.0 + p.changeAmount2.get() * p.changeAmount2.get() * 10.0;
			
			
			_changeTime2 = 0;			
			_changeReached2=false;
			
			if(p.changeSpeed2.get() == 1.0) 	_changeLimit2 = 0;
			else 						_changeLimit2 = (1.0 - p.changeSpeed2.get()) * (1.0 - p.changeSpeed2.get()) * 20000 + 32;
			
			_changeLimit*=(1-p.changeRepeat.get()+0.1)/1.1;
			_changeLimit2*=(1-p.changeRepeat.get()+0.1)/1.1;
			
			if(totalReset)
			{
				_masterVolume = p.masterVolume.get() * p.masterVolume.get();
				
				_waveType = p.waveType;
				
				if (p.sustainTime.get() < 0.01) p.sustainTime.set( 0.01);
				
				clampTotalLength();
				
				_sustainPunch = p.sustainPunch.get();
				
				_phase = 0;
				
				_minFreqency = p.minFrequency.get();
				_muted=false;
				_overtones = p.overtones.get()*10;
				_overtoneFalloff = p.overtoneFalloff.get();
								
				_bitcrush_freq = 1 - pow(p.bitCrush.get(),1.0/3.0);				
				_bitcrush_freq_sweep = -p.bitCrushSweep.get()* 0.000015;
				_bitcrush_phase=0;
				_bitcrush_last=0;				
				
				_compression_factor = 1/(1+4*p.compressionAmount.get());
				
				_filters = p.lpFilterCutoff.get() != 1.0 || p.hpFilterCutoff.get() != 0.0;				
				
				_lpFilterPos = 0.0;
				_lpFilterDeltaPos = 0.0;
				_lpFilterCutoff = p.lpFilterCutoff.get() * p.lpFilterCutoff.get() * p.lpFilterCutoff.get() * 0.1;
				_lpFilterDeltaCutoff = 1.0 + p.lpFilterCutoffSweep.get() * 0.0001;
				_lpFilterDamping = 5.0 / (1.0 + p.lpFilterResonance.get() * p.lpFilterResonance.get() * 20.0) * (0.01 + _lpFilterCutoff);
				if (_lpFilterDamping > 0.8) _lpFilterDamping = 0.8;
				_lpFilterDamping = 1.0 - _lpFilterDamping;
				_lpFilterOn = p.lpFilterCutoff.get() != 1.0;
				
				_hpFilterPos = 0.0;
				_hpFilterCutoff = p.hpFilterCutoff.get() * p.hpFilterCutoff.get() * 0.1;
				_hpFilterDeltaCutoff = 1.0 + p.hpFilterCutoffSweep.get() * 0.0003;
				
				_vibratoPhase = 0.0;
				_vibratoSpeed = p.vibratoSpeed.get() * p.vibratoSpeed.get() * 0.01;
				_vibratoAmplitude = p.vibratoDepth.get() * 0.5;
				
				_envelopeVolume = 0.0;
				_envelopeStage = 0;
				_envelopeTime = 0;
				_envelopeLength0 = p.attackTime.get() * p.attackTime.get() * 100000.0;
				_envelopeLength1 = p.sustainTime.get() * p.sustainTime.get() * 100000.0;
				_envelopeLength2 = p.decayTime.get() * p.decayTime.get() * 100000.0 + 10;
				_envelopeLength = _envelopeLength0;
				_envelopeFullLength = _envelopeLength0 + _envelopeLength1 + _envelopeLength2;
				
				_envelopeOverLength0 = 1.0 / _envelopeLength0;
				_envelopeOverLength1 = 1.0 / _envelopeLength1;
				_envelopeOverLength2 = 1.0 / _envelopeLength2;
				
				_flanger = p.flangerOffset.get() != 0.0 || p.flangerSweep.get() != 0.0;
				
				_flangerOffset = p.flangerOffset.get() * p.flangerOffset.get() * 1020.0;
				if(p.flangerOffset.get() < 0.0) _flangerOffset = -_flangerOffset;
				_flangerDeltaOffset = p.flangerSweep.get() * p.flangerSweep.get() * p.flangerSweep.get() * 0.2;
				_flangerPos = 0;
				
				_flangerBuffer.reserve(1024);
        _noiseBuffer.reserve(32);
        _pinkNoiseBuffer.reserve(32);
        _loResNoiseBuffer.reserve(32);
				
				for(unsigned int i= 0; i < 1024; i++) _flangerBuffer[i] = 0.0;
				for(unsigned int i = 0; i < 32; i++) _noiseBuffer[i] = random() * 2.0 - 1.0;
				for(unsigned int i = 0; i < 32; i++) _pinkNoiseBuffer[i] = _pinkNumber.GetNextValue();
				for(unsigned int i = 0; i < 32; i++) _loResNoiseBuffer[i] = ((i%LoResNoisePeriod)==0) ? random()*2.0-1.0 : _loResNoiseBuffer[i-1];							
			
				_repeatTime = 0;
				
				if (p.repeatSpeed.get() == 0.0) 	_repeatLimit = 0;
				else 						_repeatLimit = int((1.0-p.repeatSpeed.get()) * (1.0-p.repeatSpeed.get()) * 20000) + 32;
			}
		}
		
		unsigned int GenerateSound(std::vector<double>* data)
		{
			reset(true);
			
			ByteArray _cachedWave;
      _cachedWave.data = data;
			
				synthWave(_cachedWave, _envelopeFullLength);
				
				auto length= _cachedWave.length;
				if(length < 1536)
				{
					// If the sound is smaller than the buffer length, add silence to allow it to play
					while (_cachedWave.length<1536) _cachedWave.writeFloat(0.0);
				}

        return _cachedWave.length;
		}

		static constexpr int LoResNoisePeriod= 8;
    
		SfxrParams _params;	// Params instance
    
		static constexpr double MIN_LENGTH = 0.18;
    /*
		public static const version:int = 104;
		public static const CACHED:String = "cached";		// triggered when the synth stored in this is fully cached (either via a cache command, or play()).
		public static const PLAY_COMPLETE:String = "playcomplete";		// triggered when the synth stored in this is fully cached (either via a cache command, or play()).		
		//should be <32
		
		
		private var _sound:Sound;							// Sound instance used to play the sound
		private var _channel:SoundChannel;					// SoundChannel instance of playing Sound
		
		private var _mutation:Boolean;						// If the cmeurrent sound playing or caching is a mutation
		
		private var _cachedWave:ByteArray;					// Cached wave data from a cacheSound() call
		
		private var _cachingMutation:int;					// Current caching ID
		private var _cachedMutation:ByteArray;				// Current caching wave data for mutation
		private var _cachedMutations:Vector.<ByteArray>;	// Cached mutated wave data
		private var _cachedMutationsNum:uint;				// Number of cached mutations
		private var _cachedMutationAmount:Number;			// Amount to mutate during cache
		
		private var _cacheTimePerFrame:uint;				// Maximum time allowed per frame to cache sound asynchronously
		private var _cachedCallback:Function;				// Function to call when finished caching asynchronously
		private var _cacheTicker:Shape;						// Shape used for enterFrame event
		
		private var _waveData:ByteArray;					// Full wave, read out in chuncks by the onSampleData method
		private var _waveDataPos:uint;						// Current position in the waveData
		private var _waveDataLength:uint;					// Number of bytes in the waveData
		private var _waveDataBytes:uint;					// Number of bytes to write to the soundcard
		
		private var _original:SfxrParams;					// Copied properties for mutation base
    */
		
		//--------------------------------------------------------------------------
		//
		//  Synth Variables
		//
		//--------------------------------------------------------------------------
		
		bool _finished;						// If the sound has finished

		double _masterVolume;					// masterVolume * masterVolume (for quick calculations)
//		
		WaveType _waveType;							// The type of wave to generate
//		
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
//		
		double _sustainPunch;					// The punch factor (louder at begining of sustain)
//		
		int _phase;								// Phase through the wave
		double _pos;							// Phase expresed as a Number from 0-1, used for fast sin approx
		double _period;							// Period of the wave
		double _periodTemp;						// Period modified by vibrato
		double _maxPeriod;						// Maximum period before sound stops (from minFrequency)
//		
		double _slide;							// Note slide
		double _deltaSlide;						// Change in slide
		double _minFreqency;					// Minimum frequency before stopping
		bool _muted;							// Whether or not min frequency has been attained
//		
//		
		int _overtones;					// Minimum frequency before stopping
		double _overtoneFalloff;					// Minimum frequency before stopping
//		
		double _vibratoPhase;					// Phase through the vibrato sine wave
		double _vibratoSpeed;					// Speed at which the vibrato phase moves
		double _vibratoAmplitude;				// Amount to change the period of the wave by at the peak of the vibrato wave
//		
		double _changePeriod;
		int _changePeriodTime;
//		
		double _changeAmount;					// Amount to change the note by
		int _changeTime;						// Counter for the note change
		int _changeLimit;						// Once the time reaches this limit, the note changes
		bool _changeReached;
		
		double _changeAmount2;					// Amount to change the note by
		int _changeTime2;						// Counter for the note change
		int _changeLimit2;						// Once the time reaches this limit, the note changes
		bool _changeReached2;
//		
//		
		double _squareDuty;						// Offset of center switching point in the square wave
		double _dutySweep;						// Amount to change the duty by
//		
		int _repeatTime;						// Counter for the repeats
		int _repeatLimit;						// Once the time reaches this limit, some of the variables are reset
//		
		bool _flanger;						// If the flanger is active
		double _flangerOffset;					// Phase offset for flanger effect
		double _flangerDeltaOffset;				// Change in phase offset
		int _flangerInt;							// Integer flanger offset, for bit maths
		int _flangerPos;							// Position through the flanger buffer
    std::vector<double> _flangerBuffer;			// Buffer of wave values used to create the out of phase second wave
//		
		bool _filters;						// If the filters are active
		double _lpFilterPos;					// Adjusted wave position after low-pass filter
		double _lpFilterOldPos;					// Previous low-pass wave position
		double _lpFilterDeltaPos;				// Change in low-pass wave position, as allowed by the cutoff and damping
		double _lpFilterCutoff;					// Cutoff multiplier which adjusts the amount the wave position can move
		double _lpFilterDeltaCutoff;			// Speed of the low-pass cutoff multiplier
		double _lpFilterDamping;				// Damping muliplier which restricts how fast the wave position can move
		bool _lpFilterOn;					// If the low pass filter is active
//		
		double _hpFilterPos;					// Adjusted wave position after high-pass filter
		double _hpFilterCutoff;					// Cutoff multiplier which adjusts the amount the wave position can move
		double _hpFilterDeltaCutoff;			// Speed of the high-pass cutoff multiplier
//		
    std::vector<double> _noiseBuffer;			// Buffer of random values used to generate noise
    std::vector<double> _pinkNoiseBuffer;			// Buffer of random values used to generate noise
    std::vector<double> _loResNoiseBuffer;			// Buffer of random values used to generate noise
//		
		PinkNumber _pinkNumber;
//		
		double _superSample;					// Actual sample writen to the wave
		double _sample;							// Sub-sample calculated 8 times per actual sample, averaged out to get the super sample
		unsigned int _sampleCount;						// Number of samples added to the buffer sample
		double _bufferSample;					// Another supersample used to create a 22050Hz wave
//		
		double _bitcrush_freq;					// inversely proportional to the number of samples to skip 
		double _bitcrush_freq_sweep;			// change of the above
		double _bitcrush_phase;					// samples when this > 1
		double _bitcrush_last;					// last sample value
//		
		double _compression_factor;
	};

		unsigned int GenerateSound(const SfxrParams& params, std::vector<double>* data)
    {
      SfxrSynth synth;
      synth._params = params;
      return synth.GenerateSound(data);
    }
}








#endif // BFXR_IMPLEMENTATION

#endif  // SFXPARAMS_H

