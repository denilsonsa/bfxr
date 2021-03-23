# Bfxr

[![Travis Build status](https://travis-ci.org/madeso/bfxr.svg?branch=master)](https://travis-ci.org/madeso/bfxr)
[![Appveyor Build status](https://ci.appveyor.com/api/projects/status/g5rknca4y5tw3ifm/branch/master?svg=true)](https://ci.appveyor.com/project/madeso/bfxr/branch/master)

C++ port of bfxr by increpare

Single-header stb like Library without gui and dear imgui + sdl2 gui provided

Soonish a single-header stb like library for dear imgui gui, and also possible a wxWidgets ui since file saving/loading from dear imgui might be complicated

Perhaps someday a snazzy html interface with the generator compiled with emscripten for some browser fun...

Note: This is a work in progress.

## Additions since sfxr
* 5 new waveforms : triangle, breaker, tan, whistle, and pink noise.
* 3 new filters : compression, harmonics, and bitcrusher.
* Ability to lock parameters during mutation/randomization.
* Expanded pitch-jumping abilities - good for arpeggiation effects.

## Changes since bfxr flash version
* C++ source instead of flash
* Rewrote pink noise generator to better fit the original algorithm
* Added 2 additional waveforms by @djcsdy, originally written for the flash version but never merged
  - ‘1b Noise’ is a periodic one-bit noise generator using the same algorithm as the SN76489 chip present in a BBC Micro.
  - ‘Buzz’ is a periodic noise generator using a similar algorithm that has a shorter period. It originated as a mistake, but it makes a nice digital buzz/clang sound, so I kept it.


## History
* Dr. Petter writes sfxr and licenses it under 'mit'
* Tom Vian ports is to action script and calls it as3sfxr
* increpare makes additions and renames it to bfxr and licenses it under 'apache license 2'
* I (madeso) ports it "back" to C++
