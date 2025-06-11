#pragma once
#include "../GameObject.h"
#include "../../Tilemap/Collider.h"
#include "../../Animation/Animation.h"
#include "../../Utilities/Vector2.h"

class World;

enum class ObjectType {
    MOVING_STAIR,
    BLACK_DOOR,
    DOWN_ARROW,
    UP_ARROW,
    LINE,
    RIGHT_ARROW,
    DOOR_1,
    DOOR_2,
    TRIDENT
};

class Object : public GameObject {
private:
    ObjectType type;
    bool isActivated = false;
    bool hasIdleAnim = false;      // Flag to check if idle animation is set
    bool hasActiveAnim = false;    // Flag to check if active animation is set
    Collider* collider;
    ID3D11ShaderResourceView* texture;
    RECT sourceRect;
    Animation idleAnim;       // Animation for the idle state
    Animation activeAnim;     // Animation for the active state
    World* world = nullptr;
    float scale;

    // Add velocity for moving objects
    Vector2 _velocity = { 0.0f, 0.0f };

    // For trident animation frame-based collider
    std::vector<RECT> colliderFrames; // Different collider sizes for each frame
    int currentFrame = 0;

public:
    Vector2 GetVelocity() const { return _velocity; }
    void SetVelocity(const Vector2& vel) { _velocity = vel; }
    void SetVelocity(float vx, float vy) { _velocity = Vector2(vx, vy); }

    ~Object() { delete collider; };
    void SetWorld(World* w);
    Object(float x, float y, ObjectType type, ID3D11ShaderResourceView* texture);

    void HandleCollision(float elapsedTime);
    void UpdateColliderFromAnimation(); // Update collider based on current animation frame

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    ObjectType GetType() const { return type; }
    Collider* GetCollider() const { return collider; }

    void OnActivate(); // Called when object is activated
    bool IsActivated() const { return isActivated; }
    void MarkForDelete(); // Mark for deletion

    // Set animations directly for the object
    void SetIdleAnimation(const Animation& anim) { idleAnim = anim; hasIdleAnim = true; }
    void SetActiveAnimation(const Animation& anim) { activeAnim = anim; hasActiveAnim = true; }
    void SetColliderFrames(const std::vector<RECT>& frames) { colliderFrames = frames; }
    ObjectType GetObjectType() const { return type; }

    RECT GetCurrentAnimationRect() const {
        if (type == ObjectType::TRIDENT && hasIdleAnim) {
            int frame = idleAnim.GetCurrentFrameIndex();
            if (frame >= 0 && frame < static_cast<int>(colliderFrames.size())) {
                return colliderFrames[frame];
            }
        }
        return { 0, 0, 0, 0 };
    }
};