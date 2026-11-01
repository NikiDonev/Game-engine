#pragma once
#include <GLFW/glfw3.h>
#include <algorithm>

#include "Sprite.h"
#include "TextureAtlas.h"

struct Animation {
  uint32_t startFrame{};
  uint32_t frameCount{4}, frame{};
  bool loop{true};
  Animation(float fps, uint32_t frameCount) : frameCount(frameCount) {
    setFPS(fps);
    m_Time = glfwGetTime();
  }
  void setFPS(float fps) { m_DelaySec = 1.0f / fps; }

  void UpdateFrame(const TextureAtlas &atlas, Sprite &sprite) {
    float newTime = glfwGetTime();
    float duration = newTime - m_Time;
    if (duration > fabs(m_DelaySec)) {
      m_Time = newTime;
      frame += static_cast<int>(duration / m_DelaySec);
      frame %= frameCount;
      LOG_INFO("frame %i, delay %f s, duration %f s", frame, m_DelaySec,
               duration);
      sprite.setTexCoords(atlas.getTexCoords(startFrame + frame));
    }
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
  void Play(uint32_t index) {
    if (index < animations.size()) {
      animations[index].UpdateFrame(atlas, *this);
    }
  }

private:
  uint32_t m_ActiveAnimation{};
  int m_Autoplay = -1;
};
