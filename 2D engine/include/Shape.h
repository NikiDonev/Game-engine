#pragma once
#include <vector>
#include <algorithm>
#include "View.h"

struct ShapeVertex {
	glm::vec2 position;
	glm::vec4 color;
};

enum class ShapeTypes {
	Triangle,
	Rectangle,
	RoundRectangle,
	Circle,
	Line,
	Point,
	Arrow,
	Custom
};


class Shape : public Transformable {
protected:
	AABB m_Bounds;
	bool m_UpdateBounds{ true };
public:
	std::vector<ShapeVertex> vertices;
	std::vector<uint32_t> indices;
	AABB getLocalBounds() {
		if (vertices.empty()) return AABB{};

		if (m_UpdateBounds) {
			m_Bounds = AABB { vertices[0].position, vertices[0].position };
			for (const auto& vertex : vertices) {
				m_Bounds.min.x = std::min(m_Bounds.min.x, vertex.position.x);
				m_Bounds.min.y = std::min(m_Bounds.min.y, vertex.position.y);
				m_Bounds.max.x = std::max(m_Bounds.max.x, vertex.position.x);
				m_Bounds.max.y = std::max(m_Bounds.max.y, vertex.position.y);
			}
			return m_Bounds;
			m_UpdateBounds = false;
		}
	}
};


class Circle : public Shape {
public:
	Circle() = default;
	Circle(float radius, glm::vec4 color = glm::vec4(1.0f), uint32_t segments = 32);

	Circle& setOutline(float thickness, glm::vec4 color);
	Circle& setSegments(uint32_t segments);
	Circle& setRadius(float radius);

	Circle& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Circle& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Circle& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Circle& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	float m_Radius{ 1.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	uint32_t m_Segments{ 32 };
};


class Rect : public Shape {
public:
	Rect() = default;
	Rect(glm::vec2 size, glm::vec4 color = glm::vec4(1.0f));

	Rect& setOutline(float thickness, glm::vec4 color);
	Rect& setSize(const glm::vec2& size);
	Rect& setColor(const glm::vec4& color);

	Rect& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Rect& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Rect& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Rect& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	glm::vec2 m_Size{ 1.0f, 1.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};


class Triangle : public Shape {

public:
	Triangle() = default;
	Triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec4 color = glm::vec4(1.0f));

	Triangle& setOutline(float thickness, glm::vec4 color);
	Triangle& setColor(glm::vec4 color);
	Triangle& setPoints(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2);

	Triangle& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Triangle& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Triangle& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Triangle& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	glm::vec2 m_P0{ -1.0f, 0.0f }, m_P1{ 1.0f, 0.0f }, m_P2{ 0.0f, 2.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};


class Arrow : public Shape {

public:
	Arrow() = default;
	Arrow(glm::vec2 direction, float shaftThickness = 2.0f, float headSize = 10.0f, glm::vec4 color = glm::vec4(1.0f));

	Arrow& setOutline(float thickness, glm::vec4 color) {
		m_OutlineThickness = thickness;
		m_OutlineColor = color;
		generateGeometry();
		return *this;
	}

	Arrow& setDirection(const glm::vec2& direction);
	Arrow& setShaftThickness(float thickness);
	Arrow& setHeadSize(float size);
	Arrow& setColor(const glm::vec4& color);

	Arrow& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Arrow& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Arrow& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Arrow& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	glm::vec2 m_Direction{ 1.0f, 0.0f };
	float m_ShaftThickness{ 2.0f };
	float m_HeadSize{ 10.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};


class RoundedRect : public Shape {

public:
	RoundedRect() = default;
	RoundedRect(glm::vec2 size, float cornerRadius, glm::vec4 color = glm::vec4(1.0f), uint32_t segments = 8);

	RoundedRect& setOutline(float thickness, glm::vec4 color);
	RoundedRect& setPrecision(uint32_t segments);
	RoundedRect& setSize(glm::vec2 size);
	RoundedRect& setColor(glm::vec4 color);

	RoundedRect& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	RoundedRect& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	RoundedRect& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	RoundedRect& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	glm::vec2 m_Size{ 1.0f, 1.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_CornerRadius{ 0.1f };
	uint32_t m_Segments{ 8 };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};


class Line : public Shape {

public:
	Line() = default;
	Line(glm::vec2 p1, glm::vec2 p2, float thickness = 2.0f, glm::vec4 color = glm::vec4(1.0f));

	Line& setOutline(float thickness, glm::vec4 color);
	Line& setPoints(glm::vec2 p1, glm::vec2 p2);
	Line& setThickness(float thickness);
	Line& setColor(const glm::vec4& color);

	Line& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Line& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Line& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Line& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }

private:
	void generateGeometry();

	glm::vec2 m_P1{ 0.0f }, m_P2{ 0.0f };
	float m_Thickness{ 2.0f };
	glm::vec4 m_Color{ 1.0f };
	float m_OutlineThickness{ 0.0f };
	glm::vec4 m_OutlineColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};


class Point : public Shape {
public:
	Point() = default;
	Point(glm::vec4 color, float size = 2.0f) : m_Size(size), m_Color(color) {
		generateGeometry();
	}

	Point& setSize(float size) { m_Size = size; generateGeometry(); return *this; }
	Point& setColor(const glm::vec4& color) { m_Color = color; generateGeometry(); return *this; }
	Point& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }

private:
	void generateGeometry();
	float m_Size{ 2.0f };
	glm::vec4 m_Color{ 1.0f };
};