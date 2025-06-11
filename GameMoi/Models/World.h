#pragma once

#include <vector>
#include <memory>
#include "../Utilities/Vector2.h"
#include "../Utilities/define.h"
#include "../Models/Characters/Player/Player.h"
#include "../Models/Characters/Enemy/Enemy.h"
#include "../Models/Items/Item.h"
#include "../Models/Items/BreakableItem.h"
#include "../Models/Items/Object.h"
#include "../Models/Weapons/Weapon.h"


class World {
private:
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<BreakableItem>> breakableItems;
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Weapon>> weapons;
    std::vector<Collider*> groundColliders;
	std::vector<Collider*> stairColliders;
   
    ID3D11ShaderResourceView* itemTexture;
	ID3D11ShaderResourceView* enemyTexture;
    ID3D11ShaderResourceView* breakableItemTexture;
	ID3D11ShaderResourceView* playerTexture;

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

	// Object
	void AddObject(std::unique_ptr<Object> obj);
	void RemoveObject(Object* obj);
	const std::vector<Object*>& GetObjects() const;

    // Weapon
    void AddWeapon(std::unique_ptr<Weapon> weapon);
    void RemoveWeapon(Weapon* weapon);
    const std::vector<Weapon*>& GetWeapons() const;
    void CheckWeaponEnemyCollision();
    void CheckPlayerObjectCollision();
    void CheckWeaponBreakableCollision();

    //GroundCollider
    void SetGroundColliders(std::vector<Collider*> colliders);
    std::vector<Collider*>& GetGroundColliders();

    //StairCollider
    void SetStairColliders(std::vector<Collider*> colliders);
    std::vector<Collider*>& GetStairColliders();

    void Update(float deltaTime);
    void Render(std::unique_ptr<SpriteBatch>& spriteBatch);
    void Clear();

    void SetItemTexture(ID3D11ShaderResourceView* tex);
    void SetBreakableItemTexture(ID3D11ShaderResourceView* tex);
	void SetEnemyTexture(ID3D11ShaderResourceView* tex);
	void SetPlayerTexture(ID3D11ShaderResourceView* tex);
};
