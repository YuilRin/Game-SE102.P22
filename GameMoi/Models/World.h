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
#include "../QuadTree.h"
#include "../Tilemap/Collider.h"
#include "../Tilemap/ColliderWrapper.h"
#include "../Player2.h"

class World {
private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Player2> player2;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<BreakableItem>> breakableItems;
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Weapon>> weapons;

    // Colliders for physics
    std::vector<Collider*> groundColliders;
    std::vector<Collider*> stairColliders;

    // QuadTree for spatial partitioning
    std::unique_ptr<QuadTree> gameObjectQuadTree;
    std::unique_ptr<QuadTree> colliderQuadTree;

    // Camera viewport for culling
    RECT cameraViewport;

    // Textures
    ID3D11ShaderResourceView* itemTexture;
    ID3D11ShaderResourceView* enemyTexture;
    ID3D11ShaderResourceView* breakableItemTexture;
    ID3D11ShaderResourceView* playerTexture;
    ID3D11ShaderResourceView* player2Texture;

    // Internal helper methods
    void UpdateQuadTrees();
    void UpdateGameObjectQuadTree();
    void UpdateColliderQuadTree();
    std::vector<GameObject*> GetNearbyGameObjects(const RECT& bounds);
    std::vector<Collider*> GetNearbyColliders(const RECT& bounds);

public:
    World();
    ~World();

    bool isInViewport(float x, float y, const RECT& viewport);

    bool isIntersectedInGame(const RECT& rect1, const RECT& rect2);

    bool isNearViewport(const RECT& bounds, const RECT& viewport, float margin);

    // Initialization
    void Initialize(const RECT& worldBounds);
    void SetCameraViewport(const RECT& viewport);

    // Player
    void SetPlayer(std::unique_ptr<Player> p);
    Player* GetPlayer() const;

    void SetPlayer2(std::unique_ptr<Player2> p);
    Player2* GetPlayer2() const;


    void CheckPlayerVsPlayerCollision();
    void HandlePlayerCombat(Player* attacker, Player* defender);
    void ResetPlayersToSpawn();



    // Input handling for both players
    void HandlePlayer1Input(WPARAM key, bool isPressed);
    void HandlePlayer2Input(WPARAM key, bool isPressed);

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

    // Collision detection with spatial optimization
    void CheckWeaponEnemyCollision();
    void CheckPlayerObjectCollision();
    void CheckWeaponBreakableCollision();

    // Collider management
    void SetGroundColliders(std::vector<Collider*> colliders);
    std::vector<Collider*>& GetGroundColliders();
    void SetStairColliders(std::vector<Collider*> colliders);
    std::vector<Collider*>& GetStairColliders();

    // Optimized collision queries
    std::vector<Collider*> GetNearbyGroundColliders(float x, float y, float width, float height);
    std::vector<GameObject*> GetNearbyEnemies(float x, float y, float width, float height);
    std::vector<GameObject*> GetNearbyItems(float x, float y, float width, float height);

    // Main update and render
    void Update(float deltaTime);
    void Render(std::unique_ptr<SpriteBatch>& spriteBatch);
    void Clear();

    // Texture setters
    void SetItemTexture(ID3D11ShaderResourceView* tex);
    void SetBreakableItemTexture(ID3D11ShaderResourceView* tex);
    void SetEnemyTexture(ID3D11ShaderResourceView* tex);
    void SetPlayerTexture(ID3D11ShaderResourceView* tex);
    void SetPlayer2Texture(ID3D11ShaderResourceView* tex);
};