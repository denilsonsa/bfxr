#ifndef BFXR_LANG_EN_H
#define BFXR_LANG_EN_H

// this file contains all the language strings for bfxr

// ------------------------------------------------------------
// Buttons
// ------------------------------------------------------------

#define TEXT_BTN_PICKUP_COIN "Pickup/Coin"
#define TEXT_BTN_PICKUP_COIN_DESCRIPTION "Blips and baleeps.  Try messing with the wave-forms to get your own sound."

#define TEXT_BTN_LASER_SHOOT "Laser/Shoot"
#define TEXT_BTN_LASER_SHOOT_DESCRIPTION "Pew pew.  Try playing about with the Frequency properties (slide + delta slide especially).  If you want to add some texture, try adding some light, high-frequency vibrato."

#define TEXT_BTN_EXPLOSION "Explosion"
#define TEXT_BTN_EXPLOSION_DESCRIPTION "Boom.  To make this louder, try increasing compression, or fiddling with the frequency parameters.  To make this softer, try switching to pink noise or decreasing the frequency.  If you're hearing nothing after messing with parameters, try fiddling with 'frequency cutoff'."

#define TEXT_BTN_POWERUP "Powerup"
#define TEXT_BTN_POWERUP_DESCRIPTION "Whoo.  Try messing with the slide + delta slide parameters to make these less unreservedly exhuberant.  Or how about increasing the decay and playing with the Pitch Jump/Onset parameters?"

#define TEXT_BTN_HIT_HURT "Hit/Hurt"
#define TEXT_BTN_HIT_HURT_DESCRIPTION "If you want something more crackly, try out a tan wave here."

#define TEXT_BTN_JUMP "Jump"
#define TEXT_BTN_JUMP_DESCRIPTION "Try turn your jump into a soggy kiss with some bitcrush.  "

#define TEXT_BTN_BLIP_SELECT "Blip/Select"
#define TEXT_BTN_BLIP_SELECT_DESCRIPTION "You might want to make a variation of this with longer decay for blips that accompany fadeouts or animations."

#define TEXT_BTN_RANDOMIZE "Randomize"
#define TEXT_BTN_RANDOMIZE_DESCRIPTION "Taking your life into your hands... . (Only modifies unlocked parameters)"

#define TEXT_BTN_MUTATE "Mutation"
#define TEXT_BTN_MUTATE_DESCRIPTION "Modify each unlocked parameter by a small wee amount."

// ------------------------------------------------------------
// Waveforms
// ------------------------------------------------------------

#define TEXT_WT_TRIANGLE "Triangle"
#define TEXT_WT_TRIANGLE_DESCRIPTION "Triangle waves are robust at all frequencies, stand out quite well in most situations, and have a clear, resonant quality."

#define TEXT_WT_SIN "Sin"
#define TEXT_WT_SIN_DESCRIPTION "Sin waves are the most elementary of all wave-types.  However, they can be sensitive to context (background noise or accoustics can drown them out sometimes), so be careful."

#define TEXT_WT_SQUARE "Square"
#define TEXT_WT_SQUARE_DESCRIPTION "Square waves can be quite powerful.  They have two extra properties, Square Duty and Duty Sweep, that can further control the timbre of the wave"

#define TEXT_WT_SAW "Saw"
#define TEXT_WT_SAW_DESCRIPTION "Saw waves are raspy waves."

#define TEXT_WT_BREAKER "Breaker"
#define TEXT_WT_BREAKER_DESCRIPTION "These are defined by a quadratic equation (a=t*t%1, giving a toothed-shaped), making them a little more hi-fi than other wave-types on this list.  For the most part, like a smoother, slicker triangle wave."											

#define TEXT_WT_TAN "Tan"
#define TEXT_WT_TAN_DESCRIPTION "A potentially crazy wave.  Does strange things.  Tends to produce plenty of distortion	 (because the basic shape goes outside of the standard waveform range)."											

#define TEXT_WT_WHISTLE "Whistle"
#define TEXT_WT_WHISTLE_DESCRIPTION "A sin wave with an additional sine wave overlayed at a lower amplitude and 20x the frequency.  It can end up sounding buzzy, hollow, resonant, or breathy."											

#define TEXT_WT_WHITE "White"
#define TEXT_WT_WHITE_DESCRIPTION "White noise is your bog standard random number stream.  Quite hard-sounding, compared to pink noise."										

#define TEXT_WT_PINK "Pink"
#define TEXT_WT_PINK_DESCRIPTION "Pink noise is a stream of random numbers with a filtered frequency spectrum to make it softer than white noise."											

#define TEXT_WT_ONEBIT "1b Noise"
#define TEXT_WT_ONEBIT_DESCRIPTION "Periodic 1-bit \"white\" noise. Useful for glitchy and punky sound effects."

#define TEXT_WT_BUZZ "Buzz"
#define TEXT_WT_BUZZ_DESCRIPTION "Periodic 1-bit noise with a shortened period. It makes a nice digital buzz or clang sound."

// ------------------------------------------------------------
// Sound parameters
// ------------------------------------------------------------


#define TEXT_PARAM_masterVolume  "Master Volume"
#define TEXT_PARAM_D_masterVolume  "Overall volume of the sound."

#define TEXT_PARAM_attackTime  "Attack Time"
#define TEXT_PARAM_D_attackTime  "Length of the volume envelope attack."

#define TEXT_PARAM_sustainTime  "Sustain Time"
#define TEXT_PARAM_D_sustainTime  "Length of the volume envelope sustain."

#define TEXT_PARAM_sustainPunch  "Punch"
#define TEXT_PARAM_D_sustainPunch  "Tilts the sustain envelope for more 'pop'."

#define TEXT_PARAM_decayTime  "Decay Time"
#define TEXT_PARAM_D_decayTime  "Length of the volume envelope decay (yes, I know it's called release)."

#define TEXT_PARAM_compressionAmount  "Compression"
#define TEXT_PARAM_D_compressionAmount  "Pushes amplitudes together into a narrower range to make them stand out more.  Very good for sound effects, where you want them to stick out against background music."

#define TEXT_PARAM_startFrequency  "Frequency"
#define TEXT_PARAM_D_startFrequency  "Base note of the sound."

#define TEXT_PARAM_minFrequency  "Frequency Cutoff"
#define TEXT_PARAM_D_minFrequency  "If sliding, the sound will stop at this frequency, to prevent really low notes.  If unlocked, this is set to zero during randomization."

#define TEXT_PARAM_slide  "Frequency Slide"
#define TEXT_PARAM_D_slide  "Slides the frequency up or down."

#define TEXT_PARAM_deltaSlide  "Delta Slide"
#define TEXT_PARAM_D_deltaSlide  "Accelerates the frequency slide.  Can be used to get the frequency to change direction."

#define TEXT_PARAM_vibratoDepth  "Vibrato Depth"
#define TEXT_PARAM_D_vibratoDepth  "Strength of the vibrato effect."

#define TEXT_PARAM_vibratoSpeed  "Vibrato Speed"
#define TEXT_PARAM_D_vibratoSpeed  "Speed of the vibrato effect (i.e. frequency)."

#define TEXT_PARAM_overtones  "Harmonics"
#define TEXT_PARAM_D_overtones  "Overlays copies of the waveform with copies and multiples of its frequency.  Good for bulking out or otherwise enriching the texture of the sounds (warning: this is the number 1 cause of bfxr slowdown!)."

#define TEXT_PARAM_overtoneFalloff  "Harmonics Falloff"
#define TEXT_PARAM_D_overtoneFalloff  "The rate at which higher overtones should decay."

#define TEXT_PARAM_changeRepeat  "Pitch Jump Repeat Speed"
#define TEXT_PARAM_D_changeRepeat  "Larger Values means more pitch jumps, which can be useful for arpeggiation."

#define TEXT_PARAM_changeAmount  "Pitch Jump Amount 1"
#define TEXT_PARAM_D_changeAmount  "Jump in pitch, either up or down."

#define TEXT_PARAM_changeSpeed  "Pitch Jump Onset 1"
#define TEXT_PARAM_D_changeSpeed  "How quickly the note shift happens."

#define TEXT_PARAM_changeAmount2  "Pitch Jump Amount 2"
#define TEXT_PARAM_D_changeAmount2  "Jump in pitch, either up or down."

#define TEXT_PARAM_changeSpeed2  "Pitch Jump Onset 2"
#define TEXT_PARAM_D_changeSpeed2  "How quickly the note shift happens."

#define TEXT_PARAM_squareDuty  "Square Duty"
#define TEXT_PARAM_D_squareDuty  "Square waveform only : Controls the ratio between the up and down states of the square wave, changing the tibre."

#define TEXT_PARAM_dutySweep  "Duty Sweep"
#define TEXT_PARAM_D_dutySweep  "Square waveform only : Sweeps the duty up or down."

#define TEXT_PARAM_repeatSpeed  "Repeat Speed"
#define TEXT_PARAM_D_repeatSpeed  "Speed of the note repeating - certain variables are reset each time."

#define TEXT_PARAM_flangerOffset  "Flanger Offset"
#define TEXT_PARAM_D_flangerOffset  "Offsets a second copy of the wave by a small phase, changing the tibre."

#define TEXT_PARAM_flangerSweep  "Flanger Sweep"
#define TEXT_PARAM_D_flangerSweep  "Sweeps the phase up or down."

#define TEXT_PARAM_lpFilterCutoff  "Low-pass Filter Cutoff"
#define TEXT_PARAM_D_lpFilterCutoff  "Frequency at which the low-pass filter starts attenuating higher frequencies.  Named most likely to result in 'Huh why can't I hear anything?' at her high-school grad. "

#define TEXT_PARAM_lpFilterCutoffSweep  "Low-pass Filter Cutoff Sweep"
#define TEXT_PARAM_D_lpFilterCutoffSweep  "Sweeps the low-pass cutoff up or down."

#define TEXT_PARAM_lpFilterResonance  "Low-pass Filter Resonance"
#define TEXT_PARAM_D_lpFilterResonance  "Changes the attenuation rate for the low-pass filter, changing the timbre."

#define TEXT_PARAM_hpFilterCutoff  "High-pass Filter Cutoff"
#define TEXT_PARAM_D_hpFilterCutoff  "Frequency at which the high-pass filter starts attenuating lower frequencies."

#define TEXT_PARAM_hpFilterCutoffSweep  "High-pass Filter Cutoff Sweep"
#define TEXT_PARAM_D_hpFilterCutoffSweep  "Sweeps the high-pass cutoff up or down."

#define TEXT_PARAM_bitCrush  "Bit Crush"
#define TEXT_PARAM_D_bitCrush  "Resamples the audio at a lower frequency."

#define TEXT_PARAM_bitCrushSweep  "Bit Crush Sweep"
#define TEXT_PARAM_D_bitCrushSweep  "Sweeps the Bit Crush filter up or down."


#endif  // BFXR_LANG_EN_H
