#include "engine.h"
#include <SFML/Graphics/Sprite.hpp>
#include <system_renderer.h>
#include <system_resources.h>

using namespace std;
using namespace sf;

class MainScene : public Scene {

public:
  MainScene() = default;
  ~MainScene() = default;
  sf::Sprite sprite;
  sf::Texture tex;
  Vector2f target;
  void Load() override {
    tex = *Resources::load<Texture>("dat_boi.png");
    target = {640, 360};
    sprite.setPosition(640, 360);
    sprite.setOrigin(115, 64);
    sprite.setTexture(tex);
  }

  void Update(const double& dt) override {
    Scene::Update(dt);
    auto d = target - sprite.getPosition();
    if (length(d) < 64) {
      target = {100.f + (rand() % 1000), 200.f + (rand() % 500)};
    }
    sprite.setScale(d.x > 0.f ? -1.f : 1.f, 1.f);
    sprite.move((float)dt * 100.0f * normalize(d));
  }

  void Render() override { Renderer::queue(&sprite); }
};

MainScene scene;

int main() {
  Engine::Start(1280, 720, "Deployed", &scene);
  return 0;
}