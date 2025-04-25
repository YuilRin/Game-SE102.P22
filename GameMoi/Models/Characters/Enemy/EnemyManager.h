#pragma once
#include <vector>
#include "Enemy.h"

class EnemyManager {
private:
    std::vector<Enemy*> enemies;

public:
    EnemyManager();
    ~EnemyManager();
    void AddEnemy(Enemy* enemy);
    void Update(float deltaTime);
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch);
    void Clear();

    std::vector<Enemy*>& GetEnemies() { return enemies; }
};
