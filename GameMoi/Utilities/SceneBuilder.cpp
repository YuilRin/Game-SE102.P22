#include "SceneBuilder.h"
#include "WICTextureLoader.h"
#include "../Models/Characters/Player/Player.h"
#include "../Models/Characters/Enemy/Zombie.h"
#include <tuple>
#include <fstream>

bool SceneBuilder::LoadSceneWithData(
    World* world,
    TileMap* tileMap,
    const LevelData& data,
    ID3D11Device* device,
    ID3D11DeviceContext* context
) {
    // Load tilemap
    if (!tileMap->LoadMapData(const_cast<std::string&>(data.mapFile))) return false;
    if (!tileMap->LoadTexture(device, data.tileTexture.c_str())) return false;

    auto rawMap = tileMap->GetMapData();
    for (auto& row : rawMap) {
        for (auto& tile : row) {
            if (tile == 0 )
                tile = 0; // Ground
            else if (tile == 27)
                tile = 3; // Stair top
            else if (tile == 4 || tile == 14)
                tile = 1; // Stair
            else
                tile = -1;
        }
    }
    
    auto groundColliders = CreateOptimizedCollidersFromTileMap( rawMap, tileMap->GetTileSize(), 0);
    world->SetGroundColliders(groundColliders);
   

    auto stairColliders = CreateStairCollidersFromTileMap(rawMap, tileMap->GetTileSize());
	world->SetStairColliders(stairColliders);
    

    // Load player
    ID3D11ShaderResourceView* playerTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.playerTexture.c_str(), nullptr, &playerTexture);
    world->SetPlayerTexture(playerTexture);

    std::map<PlayerState, Animation> playerAnimations = {
        {PlayerState::Idle             , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::TakingDamage     , Animation(playerTexture, {{395,	0,	425,	64},{0,0,0,0},{395,	0,	425,	64}}, 0.3f)},
        {PlayerState::Dead             , Animation(playerTexture, {{32,	0,	96,	64  }}, 0.3f)},
        {PlayerState::PickingUpItem    , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Falling          , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Jumping          , Animation(playerTexture, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::SitDown          , Animation(playerTexture, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::Walking          , Animation(playerTexture, {{585, 0 , 615, 64} , {618, 0 , 640, 64 }, {643,  0, 673, 64 }}, 0.3f)},
        {PlayerState::Stand_Hit        , Animation(playerTexture, {{460, 0 , 507, 64} , {510, 0 , 541, 64 }, {542,  0, 584, 64 }}, 0.3f)},
        {PlayerState::Climbing         , Animation(playerTexture, {{364,	0,	392,	64}}, 0.3f)},
         {PlayerState::Up         , Animation(playerTexture, {{364,	0,	392,	64}}, 0.3f)},
          {PlayerState::Down        , Animation(playerTexture, {{334,	0,	364,	64	}}, 0.3f)},
        {PlayerState::Up_Hit           , Animation(playerTexture, {{96 , 0 , 144, 64} , {144, 0 , 176, 64 }, {176,  0, 216, 64 }}, 0.3f)},
        {PlayerState::Down_Hit         , Animation(playerTexture, {{216, 0 , 258, 64} , {258, 0 , 289, 64 }, {290,  0, 334, 64 }}, 0.3f)}
    };

    auto player = std::make_unique<Player>(data.startPos, playerAnimations, device);
    world->SetPlayer(std::move(player));

    // Load items
    ID3D11ShaderResourceView* itemTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.itemTexture.c_str(), nullptr, &itemTexture);
    world->SetItemTexture(itemTexture);

    for (const auto& item : data.itemPositions) {
        float x = std::get<0>(item);
        float y = std::get<1>(item);
        ItemType type = std::get<2>(item);
        //world->AddItem(std::make_unique<Item>(x, y, type, itemTexture));
    }

    // Load breakable items
    ID3D11ShaderResourceView* breakableItemTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.breakableItemTexture.c_str(), nullptr, &breakableItemTexture);
    world->SetBreakableItemTexture(breakableItemTexture);

    if (breakableItemTexture != nullptr) {
        Animation bigCandleIdle = Animation(breakableItemTexture, { { 0, 0, 32, 64 }, { 32, 0, 64, 64 } }, 0.3f);
        Animation bigCandleBreak = Animation(breakableItemTexture, { { 0, 0, 32, 64 }, { 32, 0, 64, 64 } }, 0.2f);

        Animation candleIdle = Animation(breakableItemTexture, { { 64, 0, 80, 32 }, { 80, 0, 96, 32 } }, 0.3f);
        Animation candleBreak = Animation(breakableItemTexture, { { 64, 0, 80, 32 }, { 80, 0, 96, 32 } }, 0.2f);

       
		Animation trident = Animation(breakableItemTexture, { 
                {0, 160, 64, 196},      // Frame 0: matches animation frame exactly
                    {64, 160, 128, 213},    // Frame 1: matches animation frame exactly  
                    {128, 160, 192, 228},   // Frame 2: matches animation frame exactly
                    {192, 160, 256, 244},   // Frame 3: matches animation frame exactly
                    {256, 160, 320, 260},   // Frame 4: matches animation frame exactly
                    {320, 160, 384, 278},   // Frame 5: matches animation frame exactly
                    {384, 160, 448, 292}
            }, 0.5f);
        for (const auto& item : data.breakableItemPositions) {
            float x = std::get<0>(item);
            float y = std::get<1>(item);
            BreakableItemType type = std::get<2>(item);

            auto breakableItem = std::make_unique<BreakableItem>(x, y, type, breakableItemTexture);

            // Set appropriate animations based on item type
            switch (type) {
            case BreakableItemType::BIG_CANDLE:
                breakableItem->SetIdleAnimation(bigCandleIdle);
                breakableItem->SetBreakAnimation(bigCandleBreak);
                break;

            case BreakableItemType::CANDLE1:
            case BreakableItemType::CANDLE:
                breakableItem->SetIdleAnimation(candleIdle);
                breakableItem->SetBreakAnimation(candleBreak);
                break;

            case BreakableItemType::STAIR:
                break;
            }
            world->AddBreakable(std::move(breakableItem));
        }

        for (const auto& item : data.objectPositions) {
            float x = std::get<0>(item);
            float y = std::get<1>(item);
            ObjectType type = std::get<2>(item);

            auto object = std::make_unique<Object>(x, y, type, breakableItemTexture);

            switch (type) {
            case ObjectType::TRIDENT:
                object->SetIdleAnimation(trident);

                // Collider frames match exactly with animation frames:
                // Animation: { 0, 160, 64, 132 }, { 64, 160, 128, 292 }, { 128, 160, 192, 292 }, etc.
                object->SetColliderFrames({
                    {0, 160, 64, 196},      // Frame 0: matches animation frame exactly
                    {64, 160, 128, 213},    // Frame 1: matches animation frame exactly  
                    {128, 160, 192, 228},   // Frame 2: matches animation frame exactly
                    {192, 160, 256, 244},   // Frame 3: matches animation frame exactly
                    {256, 160, 320, 260},   // Frame 4: matches animation frame exactly
                    {320, 160, 384, 278},   // Frame 5: matches animation frame exactly
                    {384, 160, 448, 292}    // Frame 6: matches animation frame exactly
                    });
                break;
            case ObjectType::MOVING_STAIR:
                break;
            }
            world->AddObject(std::move(object));
        }
    }
    else {
        // Nếu texture không load được, vẫn tạo các đối tượng nhưng không có animation
        for (const auto& item : data.breakableItemPositions) {
            float x = std::get<0>(item);
            float y = std::get<1>(item);
            BreakableItemType type = std::get<2>(item);

            auto breakableItem = std::make_unique<BreakableItem>(x, y, type, breakableItemTexture);
            world->AddBreakable(std::move(breakableItem));
        }
    }

    // Load enemies
    ID3D11ShaderResourceView* enemyTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.enemyTexture.c_str(), nullptr, &enemyTexture);
    world->SetEnemyTexture(enemyTexture);

    for (const auto& pos : data.enemyPositions) {
        float x = pos.first;
        float y = pos.second;
        Zombie* z = new Zombie(x, y, enemyTexture);
        z->LoadAnimations(device);
        //z->SetPlayerReference(world->GetPlayer());
        world->AddEnemy(std::unique_ptr<Enemy>(z));
    }

    return true;
}