#pragma once

#include <vector>
#include <memory>
#include "../Utilities/Vector2.h"
#include "../Utilities/define.h"
#include "../Models/Characters/Player/Player.h"
#include "../Models/Characters/Enemy/Enemy.h"
#include "../Models/Items/Item.h"
#include "../Models/Items/BreakableItem.h"
#include "../Models/Weapons/Weapon.h"


class World {
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<BreakableItem>> breakables;
    std::vector<std::unique_ptr<Weapon>> weapons;
    std::vector<Collider*> groundColliders;
   


public:
    World();
    ~World();

    // Player
    void SetPlayer(std::unique_ptr<Player> p);
    Player* GetPlayer() const;

    // Enemy
    void AddEnemy(std::unique_ptr<Enemy> enemy);
    void RemoveEnemy(Enemy* enemy);
    const std::vector<Enemy*>& GetEnemies() const;

    // Item
    void AddItem(std::unique_ptr<Item> item);
    void RemoveItem(Item* item);
    const std::vector<Item*>& GetItems() const;

    // Breakable
    void AddBreakable(std::unique_ptr<BreakableItem> obj);
    void RemoveBreakable(BreakableItem* obj);
    const std::vector<BreakableItem*>& GetBreakables() const;

    // Weapon
    void AddWeapon(std::unique_ptr<Weapon> weapon);
    void RemoveWeapon(Weapon* weapon);
    const std::vector<Weapon*>& GetWeapons() const;
    void CheckWeaponEnemyCollision();

    //GroundCollider
    void SetGroundColliders(std::vector<Collider*> colliders);
    std::vector<Collider*>& GetGroundColliders();

    void Update(float deltaTime);
    void Render(std::unique_ptr<SpriteBatch>& spriteBatch);
    void Clear();
};
