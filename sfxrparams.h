#ifndef SFXPARAMS_H
#define SFXPARAMS_H

#include <vector>
#include <string>
#include <cmath>

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
	class SfxrParams 
	{
    public:

		/** If the parameters have been changed since last time (shouldn't used cached sound) */
		bool paramsDirty = true;			
		
		//interface uses this to disable square sliders when non-square wavetype selected
		// public static const SquareParams:Array = ["squareDuty","dutySweep"];
		
		//params to exclude from list
		// public static const ExcludeParams:Array = ["waveType","masterVolume"];
		
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
			
    template<typename T>
    struct Param {
      // real name, decription, grouping, default, min, max, 
      Param(const std::string& name, const std::string& desc, int grouping, T def, T min, T max)
        : real_name(name)
          , description(desc)
          , default_value(def)
          , min_value(min)
          , max_value(max)
      {
      }

      std::string real_name;
      std::string description;
      T default_value;
      T min_value;
      T max_value;

      double random_power = 1.0;

      T get() const { return current_value; }
      void set(T t)
      {
        if(t > max_value) current_value = max_value;
        else if(t < min_value) current_value = min_value;
        else current_value = t;
      }
      bool locked = false;;
      T current_value = 0;
    };

		Param<double> waveType;
		
		Param<double> masterVolume;
		Param<double> attackTime;
		Param<double> sustainTime;
		Param<double> sustainPunch;
		Param<double> decayTime;
		
		Param<double> compressionAmount;
		
		Param<double> startFrequency;
		Param<double> minFrequency;
		
		Param<double> slide;
		Param<double> deltaSlide;
		
		Param<double> vibratoDepth;
		Param<double> vibratoSpeed;
		
		Param<double> overtones;
		Param<double> overtoneFalloff;
		
		Param<double> changeRepeat;
		
		Param<double> changeAmount;
		Param<double> changeSpeed;
		
		Param<double> changeAmount2;
		Param<double> changeSpeed2;
		
		Param<double> squareDuty;
		Param<double> dutySweep;
		
		Param<double> repeatSpeed;
		
		Param<double> flangerOffset;
		Param<double> flangerSweep;
		
		Param<double> lpFilterCutoff;
		Param<double> lpFilterCutoffSweep;
		Param<double> lpFilterResonance;
		
		Param<double> hpFilterCutoff;
		Param<double> hpFilterCutoffSweep;
						
		Param<double> bitCrush;
		Param<double> bitCrushSweep;
		
		//--------------------------------------------------------------------------
		//
		//  Getters / Setters
		//
		//--------------------------------------------------------------------------
		
		static constexpr int WAVETYPECOUNT = static_cast<int>(WaveType::COUNT);
		
		SfxrParams();

		void setAllLocked(bool locked);
    std::vector<Param<double>*> GetParams();
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

#endif  // SFXPARAMS_H

