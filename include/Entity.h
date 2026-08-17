#pragma once
#include "ShapeRenderer.h"
#include "random.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"


struct Entity{
	int id{};
	float maxSpeed = 50.0f;
	float maxForce = 1.0f;
	float wanderAngle{0.0f};
	glm::vec2 pos{ 0.0f }, vel{ 0.0f }, acc{ 0.0f }, desired{ 0.0f };
	glm::vec4 bounds{};

	Triangle body{ { -1.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 2.0f } };
	Arrow direction{ vel, 0.1f, 0.3f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	Arrow wanderDir{ vel, 0.1f, 0.3f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) };

	Entity(int id) :id(id){
	}
	void Update(float deltaTime, glm::vec2 target, glm::vec2 otherVel) {
		if (id == 0) desired = chase(target);
		//else if (id == 1) desired = runFrom(target);
		//else if (id == 1) desired = wander(5.0f, 8.0f) * 0.3f + runFrom(target) * 0.7f;
		else if (id == 1) desired = runFromAdvanced(target, otherVel) * 0.9f + wander(5.0f, 8.0f) * 0.1f;
		//chaseTarget(target);
		stayOnScreen();
		glm::vec2 steer = desired - vel;
		acc = steer;
		vel += acc * deltaTime;
		pos += vel * deltaTime;


	}
	void setBounds(glm::vec4 Bounds) {
		bounds = Bounds;
	}
	void stayOnScreen(float offset = 3.0f) {
		float left = bounds.x, bottom = bounds.y, right = bounds.z, top = bounds.w;
		if (pos.x < left) vel.x *= -1;
		if (pos.x > right) vel.x *= -1;
		if (pos.y < bottom) vel.y *= -1;
		if (pos.y > top) vel.y *= -1;
	}
	glm::vec2 runFromAdvanced(glm::vec2 enemyPos, glm::vec2 enemyVel) {
		return runFrom(enemyPos) * 0.8f + runFrom(enemyPos + enemyVel * 0.1f) * 0.2f;
	}
	glm::vec2 runFrom(glm::vec2 enemy) {
		glm::vec2 awayFromEnemy = pos - enemy;
		return glm::vec2(maxSpeed) * glm::normalize(awayFromEnemy);
	}
	glm::vec2 wander(float distance, float radius) {
		glm::vec2 dir = vel;
		if (glm::length(dir) < 0.001f) dir = { 1.0f, 0.0f };
		glm::vec2 center = pos + glm::normalize(dir) * distance;
		float range = 0.15f;
		float randomAngle = rng.number(-range, range);
		wanderAngle += randomAngle;
		glm::vec2 randomVector = glm::vec2(radius, 0.0f);
		randomVector = glm::rotate(randomVector, wanderAngle);
		glm::vec2 randomTarget = center + randomVector;

		wanderDir.setPosition(center);
		wanderDir.setDirection(randomVector);

		return chase(randomTarget);

	}
	glm::vec2 chase(glm::vec2 target) {
		glm::vec2 targetVel = target - pos;
		return glm::vec2(maxSpeed) * glm::normalize(targetVel);

	}
	void Draw(ShapeRenderer& renderer) {
		body.setPosition(pos);
		float angle = glm::degrees(atan2(vel.y, vel.x)) - 90.0f;
		body.setRotation(angle);
		direction.setPosition(pos);
		direction.setDirection(vel);


		renderer.Add(body);
		renderer.Add(wanderDir);
		renderer.Add(direction);

		ImGui::PushID(id);
		ImGui::Text("Position %f, %f", pos.x, pos.y);
		ImGui::Text("Acceleration %f, %f", acc.x, acc.y);
		ImGui::DragFloat("Max Speed", &maxSpeed, 1.0f, 0.01f, 100.0f);
		ImGui::DragFloat("Max Force", &maxForce, 1.0f, 0.01f, 100.0f);

		ImGui::PopID();
	}
};