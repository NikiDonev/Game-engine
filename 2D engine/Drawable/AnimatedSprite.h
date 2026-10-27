#pragma once
#include <GLFW/glfw3.h>
#include <algorithm>

#include "Sprite.h"
#include "TextureAtlas.h"

struct Animation {
  uint32_t startFrame{};
  uint32_t frameCount{}, frame{};
  bool loop{true};
  void Play() {
    frame = 0;
    m_DelaySec = 0.0f;
    m_Time = glfwGetTime();
  }
  void setFPS(float fps) { m_DelaySec = 1.0f / fps; }
  glm::vec4 UpdateFrame(const TextureAtlas &atlas) {
    float newTime = glfwGetTime();
    float duration = newTime - m_Time;
    if (duration > fabs(m_DelaySec)) {
      m_Time = newTime;
      frame += static_cast<int>(duration / m_DelaySec);
      frame %= frameCount;
      LOG_INFO("frame %i, delay %f s, duration %f s", frame, m_DelaySec,
               duration);
      return (atlas.getTexCoords(startFrame + frame));
    }
    return {};
  }

private:
  float m_DelaySec{};
  float m_Time{};
};

class AnimatedSprite : public Sprite {
public:
  std::vector<Animation> animations;
  TextureAtlas atlas;

  AnimatedSprite() {}
  void SetAutoplay(int autoplay) {
    m_Autoplay = std::clamp(autoplay, -1, (int)animations.size());
  }
  void Update() {
    if (!animations.empty())
      animations[m_ActiveAnimation].UpdateFrame(atlas);
  }
  void setActiveAnimation() {}

private:
  uint32_t m_ActiveAnimation{};
  int m_Autoplay = -1;
};
