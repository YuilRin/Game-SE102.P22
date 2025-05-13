#include "World.h"
#include <algorithm> // For std::remove_if

World::World() = default;
World::~World() = default;

// === Player ===
void World::SetPlayer(std::unique_ptr<Player> p) {
    p->SetWorld(this);
    player = std::move(p);
}

Player* World::GetPlayer() const {
    return player.get();
}

// === Enemy ===
void World::AddEnemy(std::unique_ptr<Enemy> enemy) {
    // Set the enemy's world and player reference
    enemy->SetWorld(this);
    if (player) {
        enemy->SetPlayer(player.get());
    }
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
    item->SetWorld(this);
    items.push_back(std::move(item));
}

void World::RemoveItem(Item* target) {
    auto it = std::remove_if(items.begin(), items.end(),
        [target](const std::unique_ptr<Item>& i) { return i.get() == target; });
    items.erase(it, items.end());
}

const std::vector<Item*>& World::GetItems() const {
    static std::vector<Item*> result;
    result.clear(); // Clear previous results
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
    result.clear(); // Clear previous results
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
    result.clear(); // Clear previous results
    for (const auto& w : weapons) result.push_back(w.get());
    return result;
}

void World::SetGroundColliders(std::vector<Collider*> colliders)
{
    groundColliders = colliders;
}

std::vector<Collider*>& World::GetGroundColliders()
{
    return groundColliders;
}

// === Update & Render ===
void World::Update(float deltaTime) {

    auto* playerCollider = player->GetCollider();

    // Collect active enemy colliders
    std::vector<Collider*> enemyColliders;
    for (const auto& enemy : enemies) {
        if (enemy->IsActive() && enemy->GetInfo() && enemy->GetInfo()->GetHeart() > 0) {
            enemyColliders.push_back(enemy->GetCollider());
        }
    }

    std::vector<CollisionEvent*> enemyEvents;
    CollisionEvent* colX = nullptr;
    CollisionEvent* colY = nullptr;

    CollisionManager::GetInstance()->Scan(playerCollider, deltaTime, enemyColliders, enemyEvents);
    CollisionManager::GetInstance()->Filter(playerCollider, enemyEvents, colX, colY, 0, 1, 1);

    // Process enemy collisions
    for (auto& enemy : enemies) {
        if (!enemy->IsActive() || enemy->GetInfo()->GetHeart() <= 0) continue;
        CollisionEvent* chosen = colY ? colY : colX;

        if (chosen && chosen->WasCollided()) {
            // Determine if enemy is to the left or right of player
            float playerX = player->GetX();
            float enemyX = enemy->GetX();
            bool enemyIsLeft = enemyX < playerX;

            // Apply damage
            player->TakeDamage(10);

            // Apply knockback (from left, strength)
            player->ApplyKnockback(enemyIsLeft, 150.0f);

            // Break after first collision to prevent multiple knockbacks at once
            break;
        }
    }

    // === HANDLE ITEM COLLISION ===
    std::vector<Collider*> itemColliders;
    for (const auto& item : items) {
        if (!item->IsExpired()) {
            itemColliders.push_back(item->GetCollider());
        }
    }

    std::vector<CollisionEvent*> itemEvents;
    colX = nullptr;
    colY = nullptr;

    CollisionManager::GetInstance()->Scan(playerCollider, deltaTime, itemColliders, itemEvents);
    CollisionManager::GetInstance()->Filter(playerCollider, itemEvents, colX, colY, 0, 1, 1);

    // Prioritize Y-axis collision (falling onto an item), then X-axis
    CollisionEvent* chosen = colY ? colY : colX;

    if (chosen && chosen->WasCollided()) {
        Item* item = dynamic_cast<Item*>(chosen->dest->GetOwner());
        if (item) {
            if (item->GetType() == ItemType::SMALL_HEART) {
                player->GetInfo()->AddHeart(2);
            }
            item->MarkForDelete();
        }
    }

    // Clean up collision events
    for (auto& e : itemEvents) delete e;

    // Remove expired or consumed items
    items.erase(std::remove_if(items.begin(), items.end(),
        [](const std::unique_ptr<Item>& i) {
            return i->IsExpired();
        }), items.end());

    // Update game objects
    if (player) {
        player->Update(deltaTime);
    }

    for (const auto& e : enemies) {
        e->Update(deltaTime);
    }

    for (const auto& i : items) {
        i->Update(deltaTime);
    }

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