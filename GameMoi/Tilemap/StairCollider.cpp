// Collider.cpp
#include "Collider.h"
#include <cmath>

Collider::Collider(float x, float y, float width, float height, float vx, float vy, bool isBlocking)
    : x(x), y(y), width(width), height(height), vx(vx), vy(vy), isBlocking(isBlocking) {
}

void Collider::GetBoundingBox(float& left, float& top, float& right, float& bottom) const {
    left = x;
    top = y;
    right = x + width;
    bottom = y + height;
}

void Collider::GetSpeed(float& out_vx, float& out_vy) const {
    out_vx = vx;
    out_vy = vy;
}

bool Collider::IsBlocking() const {
    return isBlocking;
}

void Collider::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Collider::GetPosition(float& out_x, float& out_y) const {
    out_x = x;
    out_y = y;
}

std::vector<Collider*> CreateOptimizedCollidersFromTileMap(std::vector<std::vector<int>>& tileMap, int tileSize, int solidTileValue) {
    std::vector<Collider*> colliders;
    int rows = tileMap.size();
    int cols = tileMap[0].size();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (tileMap[row][col] == solidTileValue) {
                int startCol = col;
                int width = 0;

                // Tìm chiều rộng của vùng liền nhau theo hàng
                while (col < cols && tileMap[row][col] == solidTileValue) {
                    tileMap[row][col] = -1; // đánh dấu đã xử lý
                    width++;
                    col++;
                }

                // Tìm chiều cao tối đa có thể gộp
                int height = 1;
                bool canExpand;
                do {
                    canExpand = true;
                    if (row + height >= rows) break;
                    for (int i = 0; i < width; ++i) {
                        if (tileMap[row + height][startCol + i] != solidTileValue) {
                            canExpand = false;
                            break;
                        }
                    }
                    if (canExpand) {
                        for (int i = 0; i < width; ++i)
                            tileMap[row + height][startCol + i] = -1;
                        height++;
                    }
                } while (canExpand);

                float x = startCol * tileSize;
                float y = row * tileSize;
                float w = width * tileSize;
                float h = height * tileSize;
                colliders.push_back(new Collider(x, y, w, h, 0.0f, 0.0f, true));

                col--; // trừ lại vì vòng for sẽ tăng tiếp
            }
        }
    }

    return colliders;
}

std::vector<Collider*> CreateStairCollidersFromTileMap(const std::vector<std::vector<int>>& tileMap, int tileSize)
{
    std::vector<Collider*> colliders;

    int rows = tileMap.size();
    int cols = tileMap[0].size();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int tile = tileMap[row][col];
            if (tile == 1 || tile == 2) { // ddeer thanh tile-enum
                float x = col * tileSize;
                float y = row * tileSize;
                float w = tileSize;
                float h = tileSize;

                colliders.push_back(new Collider(x, y, w, h, 0.0f, 0.0f, false)); // false = non-solid
            }
        }
    }

    return colliders;
}

// ============================ Collision Event ============================

CollisionEvent::CollisionEvent(float t, float nx, float ny, float dx, float dy, Collider* src, Collider* dest)
    : t(t), nx(nx), ny(ny), dx(dx), dy(dy), src(src), dest(dest) {
}

bool CollisionEvent::WasCollided() const {
    return t >= 0.0f && t <= 1.0f;
}

bool CollisionEvent::compare(const CollisionEvent* a, const CollisionEvent* b) {
    return a->t < b->t;
}

// ============================ Collision Manager ============================

CollisionManager* CollisionManager::instance = nullptr;

CollisionManager* CollisionManager::GetInstance() {
    if (!instance) instance = new CollisionManager();
    return instance;
}

void CollisionManager::SweptAABB(
    float ml, float mt, float mr, float mb,
    float dx, float dy,
    float sl, float st, float sr, float sb,
    float& t, float& nx, float& ny) {

    float dx_entry, dx_exit, tx_entry, tx_exit;
    float dy_entry, dy_exit, ty_entry, ty_exit;
    float t_entry, t_exit;

    t = -1.0f; nx = ny = 0;

    float bl = dx > 0 ? ml : ml + dx;
    float bt = dy > 0 ? mt : mt + dy;
    float br = dx > 0 ? mr + dx : mr;
    float bb = dy > 0 ? mb + dy : mb;

    if (br < sl || bl > sr || bb < st || bt > sb) return;

    if (dx > 0) { dx_entry = sl - mr; dx_exit = sr - ml; }
    else { dx_entry = sr - ml; dx_exit = sl - mr; }

    if (dy > 0) { dy_entry = st - mb; dy_exit = sb - mt; }
    else { dy_entry = sb - mt; dy_exit = st - mb; }

    tx_entry = dx == 0 ? -INFINITY : dx_entry / dx;
    tx_exit = dx == 0 ? INFINITY : dx_exit / dx;
    ty_entry = dy == 0 ? -INFINITY : dy_entry / dy;
    ty_exit = dy == 0 ? INFINITY : dy_exit / dy;

    if ((tx_entry < 0 && ty_entry < 0) || tx_entry > 1 || ty_entry > 1) return;

    t_entry = std::max(tx_entry, ty_entry);
    t_exit = std::min(tx_exit, ty_exit);
    if (t_entry > t_exit) return;

    t = t_entry;
    if (tx_entry > ty_entry) { ny = 0; nx = dx > 0 ? -1 : 1; }
    else { nx = 0; ny = dy > 0 ? -1 : 1; }
}

CollisionEvent* CollisionManager::SweptAABB(Collider* src, float dt, Collider* dest) {
    float sl, st, sr, sb;
    float ml, mt, mr, mb;
    float t, nx, ny;

    float mvx, mvy, svx, svy;
    src->GetSpeed(mvx, mvy);
    dest->GetSpeed(svx, svy);
    float mdx = mvx * dt;
    float mdy = mvy * dt;
    float sdx = svx * dt;
    float sdy = svy * dt;

    float dx = mdx - sdx;
    float dy = mdy - sdy;

    src->GetBoundingBox(ml, mt, mr, mb);
    dest->GetBoundingBox(sl, st, sr, sb);

    SweptAABB(ml, mt, mr, mb, dx, dy, sl, st, sr, sb, t, nx, ny);

    return new CollisionEvent(t, nx, ny, dx, dy, src, dest);
}

void CollisionManager::Scan(Collider* src, float dt, const std::vector<Collider*>& objects, std::vector<CollisionEvent*>& outEvents) {
    for (auto& obj : objects) {
        CollisionEvent* e = SweptAABB(src, dt, obj);
        if (e->WasCollided()) outEvents.push_back(e);
        else delete e;
    }
}

void CollisionManager::Filter(Collider* src, std::vector<CollisionEvent*>& events, CollisionEvent*& colX, CollisionEvent*& colY, int filterBlock, int filterX, int filterY) {
    float min_tx = 1.0f, min_ty = 1.0f;
    int min_ix = -1, min_iy = -1;

    for (size_t i = 0; i < events.size(); i++) {
        CollisionEvent* c = events[i];
        if (c->isDeleted) continue;
        if (filterBlock && !c->dest->IsBlocking()) continue;

        if (c->t < min_tx && c->nx != 0 && filterX) { min_tx = c->t; min_ix = i; }
        if (c->t < min_ty && c->ny != 0 && filterY) { min_ty = c->t; min_iy = i; }
    }

    if (min_ix >= 0) colX = events[min_ix];
    if (min_iy >= 0) colY = events[min_iy];
}

void CollisionManager::Process(Collider* src, float dt, std::vector<Collider*>& objects) {
    std::vector<CollisionEvent*> events;
    CollisionEvent* colX = nullptr;
    CollisionEvent* colY = nullptr;

    if (src->IsBlocking()) Scan(src, dt, objects, events);

    float x, y, vx, vy;
    src->GetPosition(x, y);
    src->GetSpeed(vx, vy);
    float dx = vx * dt;
    float dy = vy * dt;

    if (events.empty()) {
        src->SetPosition(x + dx, y + dy);
    }
    else {
        Filter(src, events, colX, colY);
        if (colX && colY) {
            if (colY->t < colX->t) {
                y += colY->t * dy + colY->ny * 0.01f;
                src->SetPosition(x, y);
            }
            else {
                x += colX->t * dx + colX->nx * 0.01f;
                src->SetPosition(x, y);
            }
        }
        else if (colX) {
            x += colX->t * dx + colX->nx * 0.01f;
            y += dy;
            src->SetPosition(x, y);
        }
        else if (colY) {
            x += dx;
            y += colY->t * dy + colY->ny * 0.01f;
            src->SetPosition(x, y);
        }
        else {
            x += dx;
            y += dy;
            src->SetPosition(x, y);
        }
    }

    for (auto& e : events) delete e;
}
