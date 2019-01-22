#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <memory>

#include <glad/glad.h>
#include "imgui.h"

#include "nfd.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>

#include "IconsForkAwesome.h"

#include "font_noto_sans_display.h"
#include "font_forkawesome.h"

const float pi = 3.14159f;

#ifdef max
#undef max
#endif

#define BFXR_IMPLEMENTATION
#include "bfxr.h"
#include "bfxr_lang_en.h"

void ShowHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// https://stackoverflow.com/a/874160/180307
bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

class AppBase
{
 public:
  AppBase()
      : ok(true)
  {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      SDL_LogError(
          SDL_LOG_CATEGORY_APPLICATION, "Unable to init: %s", SDL_GetError());
      ok = false;
      return;
    }

    SetupWindow("bfxr");
    SetupAudioCallbacks();
  }

  void
  SetupWindow(const char* const title)
  {
// Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!window)
    {
      std::cerr << "Failed to create window.\n";
      ok = false;
      return;
    }

    gl_context = SDL_GL_CreateContext(window);
    if(!gl_context)
    {
      std::cerr << "Failed to init GL context.\n";
      ok = false;
      return;
    }

    SDL_GL_SetSwapInterval(1);  // Enable vsync

    if(!gladLoadGL())
    {
      std::cerr << "Failed to load GL.\n";
      ok = false;
      return;
    }

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << "\n";

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
    // Controls

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    io.Fonts->AddFontFromMemoryCompressedTTF(
        NotoSansDisplay_compressed_data, NotoSansDisplay_compressed_size, 16);

    {
        static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
        ImFontConfig         icons_config;
        icons_config.MergeMode        = true;
        icons_config.PixelSnapH       = true;
        icons_config.GlyphMinAdvanceX = 13.0f;

        io.Fonts->AddFontFromMemoryCompressedTTF(
            ForkAwesome_compressed_data,
            ForkAwesome_compressed_size,
            13.0f,
            &icons_config,
            icons_ranges);
    }

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // handle those errors in your application (e.g. use an assertion, or
    // display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and
    // stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame
    // below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf",
    // 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf",
    // 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);
  }

  void
  SetupAudioCallbacks()
  {
    SDL_AudioSpec spec;
    SDL_memset(&spec, 0, sizeof(spec));
    spec.freq     = sample_frequency;
    spec.format   = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples  = 1024;
    spec.callback = SDLAudioCallback;
    spec.userdata = this;

    if(0 != SDL_OpenAudio(&spec, nullptr))
    {
      SDL_LogError(
          SDL_LOG_CATEGORY_APPLICATION,
          "Failed to setup audio: %s",
          SDL_GetError());
      ok = false;
    }

    int i, count = SDL_GetNumAudioDevices(0);
    for(i = 0; i < count; ++i)
    {
      SDL_Log("Audio device %d: %s", i, SDL_GetAudioDeviceName(i, 0));
    }
  }

  void
  Start()
  {
    if(ok)
    {
      SDL_PauseAudio(0);
    }
  }

  ~AppBase()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
  };

  virtual void
  Draw() = 0;

  virtual float
  SynthSample(int time) = 0;

  void
  OnRender()
  {
    ImGuiIO&     io          = ImGui::GetIO();
    const ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    Draw();

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(window, gl_context);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  void
  AudioCallback(Uint8* stream, int bytes)
  {
    const int len    = bytes / 2;
    auto*     output = reinterpret_cast<Sint16*>(stream);

    const Sint16 max_amplitude = 32767;

    for(int i = 0; i < len; i += 1)
    {
      const auto sample_time = sample_position + i;
      float sample = 0.0f;
      if(sample_time >= sample_length )
      {
        sample = 0;
      }
      else
      {
        sample = SynthSample(sample_time);
      }
      if(sample > 1)
      {
        sample = 1;
      }
      if(sample < -1)
      {
        sample = -1;
      }
      output[i]       = static_cast<Sint16>(max_amplitude * sample);
    }

    if(sample_position <= sample_length )
    {
      sample_position += len;
    }
  }

  static void
  SDLAudioCallback(void* userdata, Uint8* stream, int len)
  {
    auto* app = (AppBase*)userdata;
    app->AudioCallback(stream, len);
  }

 public:
  bool ok;

  void PlaySound(int samples)
  {
    sample_position = 0;
    sample_length = samples;
  }

 protected:
  int sample_position     = 0;
  int sample_length = 0;
  int   sample_frequency    = 44100;
  float audio_callback_time = 0;
  int   samples_consumed    = 0;

 public:
  SDL_Window*   window;
  SDL_GLContext gl_context;
};

namespace {
  float double_to_float(void* data, int idx)
  {
    auto* arr = static_cast<std::vector<double>*>(data);
    return (*arr)[idx];
  }
}


      bool radio(const char* str, bfxr::WaveType* val, bfxr::WaveType wt)
      { if(ImGui::RadioButton(str, *val == wt)) { *val = wt; return true; } else { return false; } }

      void Locked(bool* b) {
        if(ImGui::Button(*b ? ICON_FK_LOCK : ICON_FK_UNLOCK )) { *b = !*b; }
      }

class App : public AppBase
{
 public:
  App()
  {
  }

  void
  Draw() override
  {
    // imgui: demo window
    ImGui::ShowDemoWindow();

    if(ImGui::Begin("UI"))
    {
      if(ImGui::Button("Light"))
      {
        ImGui::StyleColorsLight();
      }
      ImGui::SameLine();
      if(ImGui::Button("Dark"))
      {
        ImGui::StyleColorsDark();
      }
    }
    ImGui::End();


    if(ImGui::Begin("Bfxr"))
    {
      bool sound_changed = false;

#define BTN(TEXT, DESC, CODE) if(ImGui::Button(TEXT)) { sound_changed = true; CODE; } ImGui::SameLine(); ShowHelpMarker(DESC);
      BTN(TEXT_BTN_PICKUP_COIN, TEXT_BTN_PICKUP_COIN_DESCRIPTION, param.generatePickupCoin() ) ImGui::SameLine();
      BTN(TEXT_BTN_LASER_SHOOT, TEXT_BTN_LASER_SHOOT_DESCRIPTION, param.generateLaserShoot() ) ImGui::SameLine();
      BTN(TEXT_BTN_EXPLOSION, TEXT_BTN_EXPLOSION_DESCRIPTION, param.generateExplosion() ) ImGui::SameLine();
      BTN(TEXT_BTN_POWERUP, TEXT_BTN_POWERUP_DESCRIPTION, param.generatePowerup() ) ImGui::SameLine();
      BTN(TEXT_BTN_HIT_HURT, TEXT_BTN_HIT_HURT_DESCRIPTION, param.generateHitHurt() ) ImGui::SameLine();
      BTN(TEXT_BTN_JUMP, TEXT_BTN_JUMP_DESCRIPTION, param.generateJump() ) ImGui::SameLine();
      BTN(TEXT_BTN_BLIP_SELECT, TEXT_BTN_BLIP_SELECT_DESCRIPTION, param.generateBlipSelect() )

      BTN(TEXT_BTN_MUTATE, TEXT_BTN_MUTATE_DESCRIPTION, param.mutate() ) ImGui::SameLine();
      BTN(TEXT_BTN_RANDOMIZE, TEXT_BTN_RANDOMIZE_DESCRIPTION, param.randomize() )
#undef BTN

      if(ImGui::Button("Synth sound")) { SynthSound(); } ImGui::SameLine();
      if(ImGui::Button("Play sound")) { SynthSound(); PlaySound(samples.size()); }

      ImGui::Checkbox("Play on change", &play_on_change);

      if(!samples.empty())
      {
        ImGui::PlotLines("Sample", &double_to_float, &samples, samples.size(), 0, nullptr, -1.0f, 1.0f, ImVec2{0, 120});
      }
      if(!samples.empty() && ImGui::Button("Save wav"))
      {
        nfdchar_t* target = NULL;
        const auto r = NFD_SaveDialog("*.wav", nullptr, &target);
        if(r == NFD_OKAY)
        {
          std::string file = target;
          free(target);
          if(!hasEnding(file, ".wav"))
          {
            file += ".wav";
          }
          bfxr::SaveWav(file.c_str(), samples);
        }
      }
      ImGui::Separator();

#define RAD(TEXT, DESC, WT) if(radio(TEXT, &param.waveType, WT)) { sound_changed = true; } ImGui::SameLine(); ShowHelpMarker(DESC)
      RAD(TEXT_WT_SQUARE, TEXT_WT_SQUARE_DESCRIPTION, bfxr::WaveType::Square); ImGui::SameLine();
      RAD(TEXT_WT_SAW, TEXT_WT_SAW_DESCRIPTION, bfxr::WaveType::Saw); ImGui::SameLine();
      RAD(TEXT_WT_SIN, TEXT_WT_SIN_DESCRIPTION, bfxr::WaveType::Sin); ImGui::SameLine();
      RAD(TEXT_WT_WHITE, TEXT_WT_WHITE_DESCRIPTION, bfxr::WaveType::Noise); ImGui::SameLine();
      RAD(TEXT_WT_TRIANGLE, TEXT_WT_TRIANGLE_DESCRIPTION, bfxr::WaveType::Triangle); ImGui::SameLine();
      RAD(TEXT_WT_PINK, TEXT_WT_PINK_DESCRIPTION, bfxr::WaveType::Pink); ImGui::SameLine();
      RAD(TEXT_WT_TAN, TEXT_WT_TAN_DESCRIPTION, bfxr::WaveType::Tan); ImGui::SameLine();
      RAD(TEXT_WT_WHISTLE, TEXT_WT_WHISTLE_DESCRIPTION, bfxr::WaveType::Whistle); ImGui::SameLine();
      RAD(TEXT_WT_BREAKER, TEXT_WT_BREAKER_DESCRIPTION, bfxr::WaveType::Breaker); ImGui::SameLine();
      RAD(TEXT_WT_ONEBIT, TEXT_WT_ONEBIT_DESCRIPTION, bfxr::WaveType::OneBitNoise); ImGui::SameLine();
      RAD(TEXT_WT_BUZZ, TEXT_WT_BUZZ_DESCRIPTION, bfxr::WaveType::Buzz);
#undef RAD

      int id = 0;
#define ONVAR(p)\
        do {\
          ImGui::PushID(id++);\
          float current_value = param.p;\
          auto changed = ImGui::SliderFloat(TEXT_PARAM_##p, &current_value, BFXR_PARAM_##p##_MIN, BFXR_PARAM_##p##_MAX);\
          if(changed) {param.p =current_value; sound_changed = true;}\
          ImGui::SameLine();\
          Locked(&param.p##_locked);\
          ImGui::SameLine();\
          ShowHelpMarker(TEXT_PARAM_D_##p);\
          ImGui::PopID();\
        }while(false)
      ALLVALUES
#undef ONVAR
      if (sound_changed && play_on_change)
      {
        SynthSound();
        PlaySound(samples.size());
      }
    }
    ImGui::End();
  }

  void SynthSound() { samples.resize(0); bfxr::GenerateSound(param, &samples); }

  bool play_on_change = true;
  bfxr::BfxrParams param;
  std::vector<double> samples;

  float
  SynthSample(int time) override
  {
    if(samples.empty()) { return 0.0f; }
    else return samples[time];
  }
};

int
main(int, char**)
{

  App app;
  if(!app.ok)
  {
    return -1;
  }

  SDL_Event event;

  Uint64 current_time = SDL_GetPerformanceCounter();
  Uint64 last_time    = 0;

  app.Start();

  float time = 0;

  bool run = true;
  while(run)
  {
    last_time    = current_time;
    current_time = SDL_GetPerformanceCounter();

    const auto dt =
        (static_cast<float>(current_time - last_time) /
         SDL_GetPerformanceFrequency());

    time += dt;

    while(SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      switch(event.type)
      {
        case SDL_QUIT:
          run = false;
          break;
      }

      if(event.type == SDL_WINDOWEVENT &&
         event.window.event == SDL_WINDOWEVENT_CLOSE &&
         event.window.windowID == SDL_GetWindowID(app.window))
      {
        run = false;
      }
    }

    app.OnRender();
  }

  SDL_Quit();

  return 0;
}
