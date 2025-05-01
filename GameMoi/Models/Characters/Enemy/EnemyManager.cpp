#include "EnemyManager.h"

EnemyManager::EnemyManager() {
 
}

EnemyManager::~EnemyManager() {

}

void EnemyManager::AddEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void EnemyManager::Update(float deltaTime) {
    for (auto* enemy : enemies) { 
        if (enemy->IsActive()) {
            enemy->Update(deltaTime);
        }
    }
}

void EnemyManager::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
    for (auto& enemy : enemies) {
        if (enemy->IsActive()) {
            enemy->Render(spriteBatch);
        }
    }
}

void EnemyManager::Clear() {
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}
