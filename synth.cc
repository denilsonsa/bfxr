#include <cmath>
#include <vector>

#include "pinknumber.h"
#include "sfxrparams.h"

namespace Synthesizer 
{
  float random();
  constexpr double PI = 3.14;

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
    void writeShort(int);
    void writeByte(int);
    void writeFloat(double);
    int length = 0;
    int position = 0;
  };

class SfxrSynth 
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
				if (_waveType == 0)
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
						if(_waveType == 3) 
						{ 
							for(unsigned int n= 0; n < 32; n++) _noiseBuffer[n] = random() * 2.0 - 1.0;
						}
						else if (_waveType == 5)
						{
							for(unsigned int n = 0; n < 32; n++) _pinkNoiseBuffer[n] = _pinkNumber.GetNextValue();							
						}
						else if (_waveType == 6)
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
							case 0: // Square wave
							{
								_sample += overtonestrength*(((tempphase / _periodTemp) < _squareDuty) ? 0.5 : -0.5);
								break;
							}
							case 1: // Saw wave
							{
								_sample += overtonestrength*(1.0 - (tempphase / _periodTemp) * 2.0);
								break;
							}
							case 2: // Sine wave (fast and accurate approx)
							{								
								 _pos = tempphase / _periodTemp;
								 _pos = _pos > 0.5 ? (_pos - 1.0) * 6.28318531 : _pos * 6.28318531;
								double _tempsample= _pos < 0 ? 1.27323954 * _pos + .405284735 * _pos * _pos : 1.27323954 * _pos - 0.405284735 * _pos * _pos;
								_sample += overtonestrength*(_tempsample < 0 ? .225 * (_tempsample *-_tempsample - _tempsample) + _tempsample : .225 * (_tempsample * _tempsample - _tempsample) + _tempsample);								
								break;
							}
							case 3: // Noise
							{
								_sample += overtonestrength*(_noiseBuffer[uint(tempphase * 32 / int(_periodTemp))%32]);
								break;
							}
							case 4: // Triangle Wave
							{						
								_sample += overtonestrength*(abs(1-(tempphase / _periodTemp)*2)-1);
								break;
							}
							case 5: // Pink Noise
							{						
								_sample += overtonestrength*(_pinkNoiseBuffer[uint(tempphase * 32 / int(_periodTemp))%32]);
								break;
							}
							case 6: // tan
							{
								//detuned
								_sample += tan(PI*tempphase/_periodTemp)*overtonestrength;
								break;
							}
							case 7: // Whistle 
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
							case 8: // Breaker
							{	
								double amp= tempphase/_periodTemp;								
								_sample += overtonestrength*(abs(1-amp*amp*2)-1);
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
				 
        // Writes same value to left and right channels
        buffer.writeFloat(_superSample);
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
			// Shorter reference
			auto& p = _params;
			
			_period = 100.0 / (p.startFrequency.get() * p.startFrequency.get() + 0.001);
			_maxPeriod = 100.0 / (p.minFrequency.get() * p.minFrequency.get() + 0.001);
			
			
			_slide = 1.0 - p.slide.get() * p.slide.get() * p.slide.get() * 0.01;
			_deltaSlide = -p.deltaSlide.get() * p.deltaSlide.get() * p.deltaSlide.get() * 0.000001;
			
			if (int(p.waveType.get()) == 0)
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
				p.paramsDirty = false;
				
				_masterVolume = p.masterVolume.get() * p.masterVolume.get();
				
				_waveType = uint(p.waveType.get());
				
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
		
		/**
		 * Cache the sound for speedy playback. 
		 * If a callback is passed in, the caching will be done asynchronously, taking maxTimePerFrame milliseconds 
		 * per frame to cache, them calling the callback when it's done. 
		 * If not, the whole sound is cached imidiately - can freeze the player for a few seconds, especially in debug mode. 
		 * @param	callback			Function to call when the caching is complete
		 * @param	maxTimePerFrame		Maximum time in milliseconds the caching will use per frame
		 */
		void cacheSound(unsigned int maxTimePerFrame = 5)
		{
			reset(true);
			
			ByteArray _cachedWave;
			
				synthWave(_cachedWave, _envelopeFullLength);
				
				auto length= _cachedWave.length;
				
				if(length < 24576)
				{
					// If the sound is smaller than the buffer length, add silence to allow it to play
					_cachedWave.position = length;
					while (_cachedWave.length<24576) _cachedWave.writeFloat(0.0);
				}
		}

		void Cache()
		{
			cacheSound();
		}
		
		//--------------------------------------------------------------------------
		//
		//  Sound Parameters
		//
		//--------------------------------------------------------------------------
		
		static constexpr int LoResNoisePeriod= 8;
    
		SfxrParams _params;	// Params instance
		// bool _cachingNormal;					// If the synth is caching a normal sound
		// bool _cachingAsync;					// If the synth is currently caching asynchronously
    
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
		int _waveType;							// The type of wave to generate
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
}

