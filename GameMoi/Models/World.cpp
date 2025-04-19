//#include "World.h"
//
//World* World::instance = nullptr;
//
//World* World::GetInstance() {
//    if (!instance)
//        instance = new World();
//    return instance;
//}
//
//void World::SetPlayer(Player* p) {
//    player = p;
//    allObjects.push_back(p);
//}
//
//void World::AddEnemy(Enemy* e) {
//    enemies.push_back(e);
//    allObjects.push_back(e);
//}
//
//void World::AddBreakable(BreakableItem* obj) {
//    breakables.push_back(obj);
//    allObjects.push_back(obj);
//}
//
//void World::SpawnItem(ItemType type, float x, float y) {
//    Item* item = new Item(x, y, type);
//    items.push_back(item);
//    allObjects.push_back(item);
//}
//
//const vector<Enemy*>& World::GetEnemies() const {
//    return enemies;
//}
//
//const vector<BreakableItem*>& World::GetBreakables() const {
//    return breakables;
//}
//
//const vector<Item*>& World::GetItems() const {
//    return items;
//}
//
//Player* World::GetPlayer() const {
//    return player;
//}
//
//void World::Update(float dt) {
//    for (auto obj : allObjects)
//        obj->Update(dt);
//
//    // Optionally: Xóa object bị đánh dấu xóa (MarkForDelete)
//    // (bạn có thể viết thêm logic này sau)
//}
//
//void World::Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
//    for (auto obj : allObjects)
//        obj->Render(spriteBatch);
//}
//
//void World::Clear() {
//    allObjects.clear();
//    enemies.clear();
//    items.clear();
//    breakables.clear();
//    player = nullptr;
//}
