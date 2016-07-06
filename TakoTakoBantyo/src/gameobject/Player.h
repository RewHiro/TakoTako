#pragma once

#include "GameObject.h"

class PlayerFootManager;

enum class PlayerType
{
	ONE = 1,
	TWO,
	THREE,
	FOUR,
};

class Player : public GameObject
{
public:

	enum class State
	{
		LIVE,
		DAMAGE,
		DOWN,
		DEATH,
		MELEE_ATTACK,
		REVIVE,
		DRAINED,
		DRAINING,
		FALL,
	};

	enum class AnimationState
	{
		WAIT,
		MOVE,
		JUMP,
		DOWN,
		MELEE_ATTACK,
		DRAIN,
		DRAINED,
		DAMAGE,
	};

	Player(int player_num, const ci::Vec3f& pos, const ci::Quatf& rotate, const PlayerType type);
	static std::shared_ptr<Player>const create(int player_num, const ci::Vec3f& pos, const ci::Quatf& rotate, const PlayerType type);

	PlayerType getType()const{ return type_; }
	int getHP()const{ return hp_; }
	AnimationState getAnimationState()const{ return animation_state_; }
	State getState()const{ return state_; }
	const ci::gl::Material& getMaterial()const{ return material_; }
	bool IsResult()const{ return is_result_; }
	bool isInvincible()const{ return is_invincible_; }

	void changeWin(){ is_result_ = true; }
	void changeAnimationState(const AnimationState animation_state){ animation_state_ = animation_state; }
	void changeMeleeAttackState(){ state_ = State::MELEE_ATTACK; }
	void changeInbincible(){ is_invincible_ = true; }

private:

	void start()final;
	void update()final;
	void draw()final;

	void onCollisionEnter(GameObject& gameobject)final;
	void onCollisionExit(GameObject& gameobject)final;

	void meleeAttack();
	void onDamage(GameObject& gameobject);
	void onDrained(GameObject& gameobject);
	void onCollisionEnterBullet(GameObject& gameobject);
	void onCollisionEnterPlayerAttack(GameObject& gameobject);
	void onCollisionEnterRockInFall(GameObject& gameobject);
	void onCollisionExitBackGroundInDown(GameObject& gameobject);
	void inDamage();
	void inDown();
	void inRevive();
	void inDrained();
	void inDraining();
	void inInvincible();
	void inFall();
	void inLive();

	void struggle();

	void easingBounce();
	void easingLanding();

	ci::gl::Texture texture_;
	std::weak_ptr<PlayerFootManager>player_foot_manager_;
	ci::audio::VoiceRef invincible_se_;
	ci::audio::VoiceRef hit_se_;
	ci::TriMesh mesh_;
	ci::gl::Material material_;
	ci::Vec3f fall_direction_;
	ci::Vec3f struggle_;
	float melee_attack_count_;
	float damage_count_;
	float revive_count_;
	float drain_count_;
	float invincible_count_;
	float fall_count_;
	float drained_count_;
	ci::Vec2f axis_value_;
	PlayerType type_;
	State state_;
	AnimationState animation_state_;
	int hp_;
	int down_count_;
	int id_;
	bool is_result_;
	bool is_invincible_;
};