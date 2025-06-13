#include "World.h"
#include <algorithm>

World::World() = default;
World::~World() = default;

bool World::isInViewport(float x, float y, const RECT& viewport) {
    return (x >= viewport.left && x <= viewport.right &&
        y >= viewport.bottom && y <= viewport.top);
}

// Kiểm tra xem hai RECT có giao nhau không
bool World::isIntersectedInGame(const RECT& rect1, const RECT& rect2) {
    return !(rect1.right <= rect2.left ||
        rect1.left >= rect2.right ||
        rect1.top <= rect2.bottom ||
        rect1.bottom >= rect2.top);
}

// Kiểm tra xem một bounds có gần viewport trong khoảng margin không
bool World::isNearViewport(const RECT& bounds, const RECT& viewport, float margin) {
    RECT expandedViewport;
    expandedViewport.left = viewport.left - (LONG)margin;
    expandedViewport.right = viewport.right + (LONG)margin;
    expandedViewport.top = viewport.top + (LONG)margin;
    expandedViewport.bottom = viewport.bottom - (LONG)margin;

    return isIntersectedInGame(bounds, expandedViewport);
}

void World::Initialize(const RECT& worldBounds) {
    // Initialize QuadTrees for spatial partitioning
    gameObjectQuadTree = std::make_unique<QuadTree>(worldBounds, 0);
    colliderQuadTree = std::make_unique<QuadTree>(worldBounds, 0);

    // Set camera viewport to world bounds initially
    cameraViewport = worldBounds;
}

void World::SetCameraViewport(const RECT& viewport) {
    cameraViewport = viewport;
}

void World::UpdateQuadTrees() {
    UpdateGameObjectQuadTree();
    UpdateColliderQuadTree();
}

void World::UpdateGameObjectQuadTree() {
    // Clear and rebuild GameObject QuadTree
    gameObjectQuadTree.reset();
    gameObjectQuadTree = std::make_unique<QuadTree>(
        RECT{ cameraViewport.left - 200, cameraViewport.top + 200,
             cameraViewport.right + 200, cameraViewport.bottom - 200 }, 0);

    // Insert active GameObjects
    for (const auto& enemy : enemies) {
        if (enemy->IsActive() && !enemy->IsDead()) {
            gameObjectQuadTree->Insert(enemy.get());
        }
    }

    for (const auto& item : items) {
        if (!item->IsExpired()) {
            gameObjectQuadTree->Insert(item.get());
        }
    }

    for (const auto& breakable : breakableItems) {
        if (!breakable->IsMarkedForDelete()) {
            gameObjectQuadTree->Insert(breakable.get());
        }
    }

    for (const auto& obj : objects) {
        if (!obj->IsMarkedForDelete()) {
            gameObjectQuadTree->Insert(obj.get());
        }
    }
}

void World::UpdateColliderQuadTree() {
    // Clear and rebuild Collider QuadTree
    colliderQuadTree.reset();
    colliderQuadTree = std::make_unique<QuadTree>(
        RECT{ cameraViewport.left - 200, cameraViewport.top + 200,
             cameraViewport.right + 200, cameraViewport.bottom - 200 }, 0);

    // Insert ground colliders that are near camera
    for (Collider* collider : groundColliders) {
        if (collider) {
            float l, t, r, b;
            collider->GetBoundingBox(l, t, r, b);
            RECT colliderBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

            // Only insert if collider is near camera viewport
            if (isIntersectedInGame(cameraViewport, colliderBounds) ||
                isNearViewport(colliderBounds, cameraViewport, 100)) {
                // Create a wrapper GameObject for the collider
                ColliderWrapper* wrapper = new ColliderWrapper(collider);
                colliderQuadTree->Insert(wrapper);
            }
        }
    }

    // Insert stair colliders
    for (Collider* collider : stairColliders) {
        if (collider) {
            float l, t, r, b;
            collider->GetBoundingBox(l, t, r, b);
            RECT colliderBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

            if (isIntersectedInGame(cameraViewport, colliderBounds) ||
                isNearViewport(colliderBounds, cameraViewport, 100)) {
                ColliderWrapper* wrapper = new ColliderWrapper(collider);
                colliderQuadTree->Insert(wrapper);
            }
        }
    }
}

std::vector<GameObject*> World::GetNearbyGameObjects(const RECT& bounds) {
    if (gameObjectQuadTree) {
        return gameObjectQuadTree->Retrieve(bounds);
    }
    return std::vector<GameObject*>();
}

std::vector<Collider*> World::GetNearbyColliders(const RECT& bounds) {
    std::vector<Collider*> result;
    if (colliderQuadTree) {
        auto wrappers = colliderQuadTree->Retrieve(bounds);
        for (GameObject* obj : wrappers) {
            ColliderWrapper* wrapper = dynamic_cast<ColliderWrapper*>(obj);
            if (wrapper) {
                result.push_back(wrapper->GetCollider());
            }
        }
    }
    return result;
}

std::vector<Collider*> World::GetNearbyGroundColliders(float x, float y, float width, float height) {
    RECT queryBounds = { (LONG)x, (LONG)(y + height), (LONG)(x + width), (LONG)y };
    return GetNearbyColliders(queryBounds);
}

std::vector<GameObject*> World::GetNearbyEnemies(float x, float y, float width, float height) {
    RECT queryBounds = { (LONG)x, (LONG)(y + height), (LONG)(x + width), (LONG)y };
    auto nearbyObjects = GetNearbyGameObjects(queryBounds);

    std::vector<GameObject*> enemies;
    for (GameObject* obj : nearbyObjects) {
        if (obj->GetTypeObject() == GameObjectType::ENEMY) {
            enemies.push_back(obj);
        }
    }
    return enemies;
}

std::vector<GameObject*> World::GetNearbyItems(float x, float y, float width, float height) {
    RECT queryBounds = { (LONG)x, (LONG)(y + height), (LONG)(x + width), (LONG)y };
    auto nearbyObjects = GetNearbyGameObjects(queryBounds);

    std::vector<GameObject*> items;
    for (GameObject* obj : nearbyObjects) {
        if (obj->GetTypeObject() == GameObjectType::ITEM) {
            items.push_back(obj);
        }
    }
    return items;
}

void World::CheckWeaponEnemyCollision() {
    Weapon* playerWeapon = player ? player->GetCurrentWeapon() : nullptr;
    if (!playerWeapon || !playerWeapon->IsActive()) return;

    auto weaponCollider = playerWeapon->GetCollider();
    if (!weaponCollider || weaponCollider->width <= 0 || weaponCollider->height <= 0) return;

    // Get weapon bounds for spatial query
    float l, t, r, b;
    weaponCollider->GetBoundingBox(l, t, r, b);
    RECT weaponBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

    // Use QuadTree to get nearby enemies instead of checking all enemies
    auto nearbyEnemies = GetNearbyGameObjects(weaponBounds);

    for (GameObject* obj : nearbyEnemies) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy || !enemy->IsActive() || enemy->IsDead() || enemy->GetInfo()->GetHeart() <= 0) {
            continue;
        }

        auto enemyCollider = enemy->GetCollider();
        if (!enemyCollider || enemyCollider->width <= 0 || enemyCollider->height <= 0) {
            continue;
        }

        // AABB collision detection
        float l1, t1, r1, b1;
        float l2, t2, r2, b2;

        weaponCollider->GetBoundingBox(l1, t1, r1, b1);
        enemyCollider->GetBoundingBox(l2, t2, r2, b2);

        bool isColliding = !(l1 >= r2 || r1 <= l2 || t1 >= b2 || b1 <= t2);

        if (isColliding) {
            enemy->TakeDamage(playerWeapon->GetDamage());
            if (playerWeapon->GetType() != WeaponType::WHIP) {
                playerWeapon->SetActive(false);
            }

            if (enemy->GetInfo()->GetHeart() == 0) {
                enemy->setIsDead(true);
                enemy->setIsActive(false);
                AddItem(std::make_unique<Item>(enemy->GetX(), enemy->GetY(), ItemType::SMALL_HEART, itemTexture));
            }
        }
    }
}

void World::CheckPlayerObjectCollision() {
    if (!player) return;

    auto playerCollider = player->GetCollider();
    if (!playerCollider) return;

    // Get player bounds for spatial query
    float l, t, r, b;
    playerCollider->GetBoundingBox(l, t, r, b);
    RECT playerBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

    // Use QuadTree to get nearby objects
    auto nearbyObjects = GetNearbyGameObjects(playerBounds);

    for (GameObject* obj : nearbyObjects) {
        Object* gameObj = dynamic_cast<Object*>(obj);
        if (!gameObj) continue;

        auto objCollider = gameObj->GetCollider();
        if (!objCollider) continue;

        // AABB collision detection
        float l1, t1, r1, b1;
        float l2, t2, r2, b2;

        playerCollider->GetBoundingBox(l1, t1, r1, b1);
        objCollider->GetBoundingBox(l2, t2, r2, b2);

        bool isColliding = !(l1 >= r2 || r1 <= l2 || t1 >= b2 || b1 <= t2);

        if (isColliding) {
            if (gameObj->GetType() == ObjectType::TRIDENT) {
                player->TakeDamage(100);

                if (player->GetInfo()->GetHeart() > 0) {
                    float playerX = player->GetX();
                    float tridentX = gameObj->GetX();
                    bool tridentIsLeft = tridentX < playerX;
                    player->ApplyKnockback(tridentIsLeft, 200.0f);
                }
                gameObj->MarkForDelete();
            }
        }
    }
}

void World::CheckWeaponBreakableCollision() {
    Weapon* playerWeapon = player ? player->GetCurrentWeapon() : nullptr;
    if (!playerWeapon || !playerWeapon->IsActive()) return;

    auto weaponCollider = playerWeapon->GetCollider();
    if (!weaponCollider || weaponCollider->width <= 0 || weaponCollider->height <= 0) return;

    // Get weapon bounds for spatial query
    float l, t, r, b;
    weaponCollider->GetBoundingBox(l, t, r, b);
    RECT weaponBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

    // Use QuadTree to get nearby breakable items
    auto nearbyObjects = GetNearbyGameObjects(weaponBounds);

    for (GameObject* obj : nearbyObjects) {
        BreakableItem* item = dynamic_cast<BreakableItem*>(obj);
        if (!item) continue;

        auto itemCollider = item->GetCollider();
        if (!itemCollider || itemCollider->width <= 0 || itemCollider->height <= 0) {
            continue;
        }

        float l1, t1, r1, b1;
        float l2, t2, r2, b2;

        weaponCollider->GetBoundingBox(l1, t1, r1, b1);
        itemCollider->GetBoundingBox(l2, t2, r2, b2);

        bool isColliding = !(l1 >= r2 || r1 <= l2 || t1 >= b2 || b1 <= t2);

        if (isColliding) {
            if (item->GetBreakableType() == BreakableItemType::BIG_CANDLE1)
                AddItem(std::make_unique<Item>(item->GetX(), item->GetY(), ItemType::SMALL_HEART, itemTexture));
            else if (item->GetBreakableType() == BreakableItemType::STAIR)
                AddItem(std::make_unique<Item>(item->GetX(), item->GetY(), ItemType::YELLOW_MONEY, itemTexture));
            item->MarkForDelete();
        }
    }
}

void World::Update(float deltaTime) {
    // Update QuadTrees for spatial optimization
    UpdateQuadTrees();

    if (!player) return;

    auto* playerCollider = player->GetCollider();
    if (!playerCollider) return;

    // Get nearby ground colliders for player physics using spatial partitioning
    float px, py;
    player->GetPosition(px, py);
    auto nearbyGroundColliders = GetNearbyGroundColliders(px - 32, py - 32, 64, 64);

    // Handle player physics with nearby colliders only
    player->Update(deltaTime);

    // Enemy collision detection using spatial partitioning
    float l, t, r, b;
    playerCollider->GetBoundingBox(l, t, r, b);
    RECT playerBounds = { (LONG)l, (LONG)t, (LONG)r, (LONG)b };

    auto nearbyEnemies = GetNearbyGameObjects(playerBounds);

    // Process enemy collisions
    for (GameObject* obj : nearbyEnemies) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if (!enemy || !enemy->IsActive() || enemy->GetInfo()->GetHeart() <= 0) continue;
        if (player->GetInfo()->GetHeart() <= 0) continue;

        auto enemyCollider = enemy->GetCollider();
        if (!enemyCollider) continue;

        // Simple AABB collision
        float el, et, er, eb;
        enemyCollider->GetBoundingBox(el, et, er, eb);

        bool isColliding = !(l >= er || r <= el || t >= eb || b <= et);

        if (isColliding) {
            float playerX = player->GetX();
            float enemyX = enemy->GetX();
            bool enemyIsLeft = enemyX < playerX;

            player->TakeDamage(10);

            if (player->GetInfo()->GetHeart() > 0) {
                player->ApplyKnockback(enemyIsLeft, 150.0f);
            }
            break;
        }
    }

    // Handle item collision using spatial partitioning
    auto nearbyItems = GetNearbyGameObjects(playerBounds);

    for (GameObject* obj : nearbyItems) {
        Item* item = dynamic_cast<Item*>(obj);
        if (!item || item->IsExpired()) continue;

        auto itemCollider = item->GetCollider();
        if (!itemCollider) continue;

        float il, it, ir, ib;
        itemCollider->GetBoundingBox(il, it, ir, ib);

        bool isColliding = !(l >= ir || r <= il || t >= ib || b <= it);

        if (isColliding) {
            if (item->GetType() == ItemType::SMALL_HEART) {
                player->GetInfo()->AddHeart(2);
            }
            item->MarkForDelete();
        }
    }

    // Collision checks using spatial optimization
    CheckWeaponEnemyCollision();
    CheckWeaponBreakableCollision();
    CheckPlayerObjectCollision();

    // Update game objects
    player->Update(deltaTime);

    for (const auto& e : enemies) {
        if (e->IsActive()) {
            e->Update(deltaTime);
        }
    }

    for (const auto& i : items) {
        if (!i->IsExpired()) {
            i->Update(deltaTime);
        }
    }

    for (const auto& b : breakableItems) {
        if (!b->IsMarkedForDelete()) {
            b->Update(deltaTime);
        }
    }

    for (const auto& o : objects) {
        if (!o->IsMarkedForDelete()) {
            o->Update(deltaTime);
        }
    }

    for (const auto& w : weapons) {
        w->Update(deltaTime);
    }

    // Remove expired/deleted objects
    items.erase(std::remove_if(items.begin(), items.end(),
        [](const std::unique_ptr<Item>& i) { return i->IsExpired(); }), items.end());

    breakableItems.erase(std::remove_if(breakableItems.begin(), breakableItems.end(),
        [](const std::unique_ptr<BreakableItem>& b) { return b->IsMarkedForDelete(); }), breakableItems.end());

    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [](const std::unique_ptr<Object>& o) { return o->IsMarkedForDelete(); }), objects.end());
}
void World::Render(std::unique_ptr<SpriteBatch>& spriteBatch) {
    // Always render player first (without viewport check for now)
    if (player) {
        player->Render(spriteBatch);
    }

    // Render all active enemies (fallback to original method temporarily)
    for (const auto& e : enemies) {
        if (e->IsActive() && !e->IsDead()) {
            e->Render(spriteBatch);
        }
    }

    // Render all non-expired items
    for (const auto& i : items) {
        if (!i->IsExpired()) {
            i->Render(spriteBatch);
        }
    }

    // Render all non-deleted breakable items
    for (const auto& b : breakableItems) {
        if (!b->IsMarkedForDelete()) {
            b->Render(spriteBatch);
        }
    }

    // Render all non-deleted objects
    for (const auto& o : objects) {
        if (!o->IsMarkedForDelete()) {
            o->Render(spriteBatch);
        }
    }

    // Render weapons
    for (const auto& w : weapons) {
        w->Render(spriteBatch);
    }
}

// Rest of the methods remain the same...
void World::SetPlayer(std::unique_ptr<Player> p) {
    p->SetWorld(this);
    player = std::move(p);
}

Player* World::GetPlayer() const {
    return player.get();
}

void World::AddEnemy(std::unique_ptr<Enemy> enemy) {
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
    result.clear();
    for (const auto& i : items) result.push_back(i.get());
    return result;
}

void World::AddBreakable(std::unique_ptr<BreakableItem> item) {
    item->SetWorld(this);
    breakableItems.push_back(std::move(item));
}

void World::RemoveBreakable(BreakableItem* target) {
    auto it = std::remove_if(breakableItems.begin(), breakableItems.end(),
        [target](const std::unique_ptr<BreakableItem>& b) { return b.get() == target; });
    breakableItems.erase(it, breakableItems.end());
}

const std::vector<BreakableItem*>& World::GetBreakables() const {
    static std::vector<BreakableItem*> result;
    result.clear();
    for (const auto& b : breakableItems) result.push_back(b.get());
    return result;
}

void World::AddObject(std::unique_ptr<Object> obj) {
    obj->SetWorld(this);
    objects.push_back(std::move(obj));
}

void World::RemoveObject(Object* target) {
    auto it = std::remove_if(objects.begin(), objects.end(),
        [target](const std::unique_ptr<Object>& o) { return o.get() == target; });
    objects.erase(it, objects.end());
}

const std::vector<Object*>& World::GetObjects() const {
    static std::vector<Object*> result;
    result.clear();
    for (const auto& o : objects) result.push_back(o.get());
    return result;
}

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

void World::SetGroundColliders(std::vector<Collider*> colliders) {
    groundColliders = colliders;
}

void World::SetStairColliders(std::vector<Collider*> colliders) {
    stairColliders = colliders;
}

std::vector<Collider*>& World::GetGroundColliders() {
    return groundColliders;
}

std::vector<Collider*>& World::GetStairColliders() {
    return stairColliders;
}

void World::Clear() {
    player.reset();
    enemies.clear();
    items.clear();
    breakableItems.clear();
    objects.clear();
    weapons.clear();
    gameObjectQuadTree.reset();
    colliderQuadTree.reset();
}

void World::SetItemTexture(ID3D11ShaderResourceView* tex) {
    itemTexture = tex;
}

void World::SetBreakableItemTexture(ID3D11ShaderResourceView* tex) {
    breakableItemTexture = tex;
}

void World::SetEnemyTexture(ID3D11ShaderResourceView* tex) {
    enemyTexture = tex;
}

void World::SetPlayerTexture(ID3D11ShaderResourceView* tex) {
    playerTexture = tex;
}