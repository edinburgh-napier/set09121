#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class BulletComponent : public Component {
protected:
  float _lifetime;

public:
  void update(double dt) override;
  void render() override {}
  explicit BulletComponent(Entity* p, float lifetime = 3.f);
  BulletComponent() = delete;
};
