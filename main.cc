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

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>

#include "font_noto_sans_display.h"

const float pi = 3.14159f;

#ifdef max
#undef max
#endif

#include "synth.h"

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


    if(ImGui::Begin("Sfxr"))
    {
      bool sound_changed = false;
      if(ImGui::Button("Pickup/Coin")) { sound_changed = true; param.generatePickupCoin(); } ImGui::SameLine();
      if(ImGui::Button("Laser/Shoot")) { sound_changed = true; param.generateLaserShoot(); } ImGui::SameLine();
      if(ImGui::Button("Explosion")) { sound_changed = true; param.generateExplosion(); } ImGui::SameLine();
      if(ImGui::Button("Powerup")) { sound_changed = true; param.generatePowerup(); } ImGui::SameLine();
      if(ImGui::Button("Hit/Hurt")) { sound_changed = true; param.generateHitHurt(); } ImGui::SameLine();
      if(ImGui::Button("Jump")) { sound_changed = true; param.generateJump(); } ImGui::SameLine();
      if(ImGui::Button("Blip/Select")) { sound_changed = true; param.generateBlipSelect(); }

      if(ImGui::Button("Mutate sound")) { sound_changed = true; param.mutate(); } ImGui::SameLine();
      if(ImGui::Button("Randomzie")) { sound_changed = true; param.randomize(); }

      if(ImGui::Button("Synth sound")) { SynthSound(); } ImGui::SameLine();
      if(ImGui::Button("Play sound")) { SynthSound(); PlaySound(samples.size()); }

      ImGui::Checkbox("Play on change", &play_on_change);

      if(!samples.empty())
      {
        ImGui::PlotLines("Sample", &double_to_float, &samples, samples.size(), 0, nullptr, -1.0f, 1.0f, ImVec2{0, 120});
      }
      ImGui::Separator();
      auto params = param.GetParams();
      for(auto* p: params)
        {
          float current_value = p->get();
          auto changed = ImGui::SliderFloat(p->real_name.c_str(), &current_value, p->min_value, p->max_value);
          if(changed) {p->set(current_value); sound_changed = true;}
          ImGui::SameLine();
          ShowHelpMarker(p->description.c_str());
        }
      if (sound_changed && play_on_change)
      {
        SynthSound();
        PlaySound(samples.size());
      }
    }
    ImGui::End();
  }

  void SynthSound() { samples.resize(0); Synthesizer::GenerateSound(param, &samples); }

  bool play_on_change = true;
  Synthesizer::SfxrParams param;
  std::vector<double> samples;

  float
  SynthSample(int time) override
  {
    if(samples.empty()) { return 0.0f; }
    else return samples[time];
  }

  void
  Update(float dt, float current_time)
  {
  }

  void
  OnKey(SDL_Keycode key, Uint16 mod, bool down, float time)
  {
  }
};

int
main(int argc, char* argv[])
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

    app.Update(dt, time);

    while(SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      switch(event.type)
      {
        case SDL_QUIT:
          run = false;
          break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          if(!ImGui::GetIO().WantCaptureKeyboard)
          {
            if(event.key.repeat == 0)
            {
              app.OnKey(
                  event.key.keysym.sym,
                  event.key.keysym.mod,
                  event.type == SDL_KEYDOWN,
                  time);
            }
          }
          break;
        default:
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
