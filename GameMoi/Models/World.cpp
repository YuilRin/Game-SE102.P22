#include "World.h"
#include <algorithm> // Cho std::remove_if

World::World() = default;
World::~World() = default;

// === Player ===
void World::SetPlayer(std::unique_ptr<Player> p) {
    player = std::move(p);
}

Player* World::GetPlayer() const {
    return player.get();
}

// === Enemy ===
void World::AddEnemy(std::unique_ptr<Enemy> enemy) {
    enemies.push_back(std::move(enemy));
}

void World::RemoveEnemy(Enemy* target) {
    auto it = std::remove_if(enemies.begin(), enemies.end(),
        [target](const std::unique_ptr<Enemy>& e) { return e.get() == target; });
    enemies.erase(it, enemies.end());
}

const std::vector<Enemy*>& World::GetEnemies() const {
    static std::vector<Enemy*> result;
    result.clear();
    for (const auto& e : enemies) result.push_back(e.get());
    return result;
}

// === Item ===
void World::AddItem(std::unique_ptr<Item> item) {
    items.push_back(std::move(item));
}

void World::RemoveItem(Item* target) {
    auto it = std::remove_if(items.begin(), items.end(),
        [target](const std::unique_ptr<Item>& i) { return i.get() == target; });
    items.erase(it, items.end());
}

const std::vector<Item*>& World::GetItems() const {
    static std::vector<Item*> result;
    result.clear();
    for (const auto& i : items) result.push_back(i.get());
    return result;
}

// === Breakable ===
void World::AddBreakable(std::unique_ptr<BreakableItem> obj) {
    breakables.push_back(std::move(obj));
}

void World::RemoveBreakable(BreakableItem* target) {
    auto it = std::remove_if(breakables.begin(), breakables.end(),
        [target](const std::unique_ptr<BreakableItem>& b) { return b.get() == target; });
    breakables.erase(it, breakables.end());
}

const std::vector<BreakableItem*>& World::GetBreakables() const {
    static std::vector<BreakableItem*> result;
    result.clear();
    for (const auto& b : breakables) result.push_back(b.get());
    return result;
}

// === Weapon ===
void World::AddWeapon(std::unique_ptr<Weapon> weapon) {
    weapons.push_back(std::move(weapon));
}

void World::RemoveWeapon(Weapon* target) {
    auto it = std::remove_if(weapons.begin(), weapons.end(),
        [target](const std::unique_ptr<Weapon>& w) { return w.get() == target; });
    weapons.erase(it, weapons.end());
}

const std::vector<Weapon*>& World::GetWeapons() const {
    static std::vector<Weapon*> result;
    result.clear();
    for (const auto& w : weapons) result.push_back(w.get());
    return result;
}

// === Update & Render ===
void World::Update(float deltaTime) {
    if (player) player->Update(deltaTime);

    for (const auto& e : enemies) e->Update(deltaTime);
    for (const auto& i : items) i->Update(deltaTime);
    for (const auto& b : breakables) b->Update(deltaTime);
    for (const auto& w : weapons) w->Update(deltaTime);
}

void World::Render(std::unique_ptr<SpriteBatch>& spriteBatch) {
    if (player) player->Render(spriteBatch);

    for (const auto& e : enemies) e->Render(spriteBatch);
    for (const auto& i : items) i->Render(spriteBatch);
    for (const auto& b : breakables) b->Render(spriteBatch);
    for (const auto& w : weapons) w->Render(spriteBatch);
}

// === Clear ===
void World::Clear() {
    player.reset();
    enemies.clear();
    items.clear();
    breakables.clear();
    weapons.clear();
}
