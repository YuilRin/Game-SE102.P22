#include "SceneBuilder.h"
#include "WICTextureLoader.h"
#include "../Models/Characters/Player/Player.h"
#include "../Models/Characters/Enemy/Zombie.h"
#include <tuple>

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
            if (tile == 0 || tile == 7 || tile == 8 || tile == 17 || tile == 18 || tile == 9)
                tile = 0; // Ground
            //else if (tile == 27)
              //  tile = 2; // Stair top
            else if (tile == 4 || tile == 14||tile == 27)
                tile = 1; // Stair
            else
                tile = -1;
        }
    }

    auto groundColliders = CreateOptimizedCollidersFromTileMap(rawMap, tileMap->GetTileSize(), 0);
    world->SetGroundColliders(groundColliders);

    auto stairColliders = CreateStairCollidersFromTileMap(rawMap, tileMap->GetTileSize());

    // Load player
    ID3D11ShaderResourceView* playerTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.playerTexture.c_str(), nullptr, &playerTexture);

    std::map<PlayerState, Animation> animations = {
        {PlayerState::Idle             , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::TakingDamage     , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Dead             , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::PickingUpItem    , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Falling          , Animation(playerTexture, {{585, 0 , 615, 64}}, 0.3f)},
        {PlayerState::Jumping          , Animation(playerTexture, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::SitDown          , Animation(playerTexture, {{429, 0 , 455, 48}}, 0.3f)},
        {PlayerState::Walking          , Animation(playerTexture, {{585, 0 , 615, 64} , {618, 0 , 640, 64 }, {643,  0, 673, 64 }}, 0.3f)},
        {PlayerState::Stand_Hit        , Animation(playerTexture, {{460, 0 , 507, 64} , {510, 0 , 541, 64 }, {542,  0, 584, 64 }}, 0.3f)},
        {PlayerState::Climbing         , Animation(playerTexture, {{421, 64, 453, 123}, {392, 64, 421, 123}, {365, 64, 394, 123}}, 0.3f)},
        {PlayerState::Up_Hit           , Animation(playerTexture, {{96 , 0 , 144, 64} , {144, 0 , 176, 64 }, {176,  0, 216, 64 }}, 0.3f)},
        {PlayerState::Down_Hit         , Animation(playerTexture, {{216, 0 , 258, 64} , {258, 0 , 289, 64 }, {290,  0, 334, 64 }}, 0.3f)}
    };

    auto player = std::make_unique<Player>(30, 428, animations, device);
    player->SetStairColliders(stairColliders);
    world->SetPlayer(std::move(player));

    // Load items
    ID3D11ShaderResourceView* itemTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.itemTexture.c_str(), nullptr, &itemTexture);
    for (const auto& item : data.itemPositions) {
        float x = std::get<0>(item);
        float y = std::get<1>(item);
        ItemType type = std::get<2>(item);
        world->AddItem(std::make_unique<Item>(x, y, type, itemTexture));
    }

    // Load enemies
    ID3D11ShaderResourceView* enemyTexture = nullptr;
    DirectX::CreateWICTextureFromFile(device, context, data.enemyTexture.c_str(), nullptr, &enemyTexture);
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
