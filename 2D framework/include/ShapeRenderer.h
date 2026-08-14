#pragma once

#include "GenericBatchRenderer.h"
#include "Sprite.h"

struct ShapeVertex {
	glm::vec2 position;
	glm::vec4 color;
};

enum class ShapeTypes {
	Triangle,
	Rectangle,
	Circle,
	Quad,
	Line,
	Point,
	Arrow,
	Custom
};


struct Shape : public Transformable {
	std::vector<ShapeVertex> vertices;
	std::vector<uint32_t> indices;
	GLenum primitiveType = GL_TRIANGLES;
};

struct Rect : public Shape {
public:
	Rect(glm::vec2 size = glm::vec2(1.0f),glm::vec2 position = glm::vec2(0.0f), glm::vec4 color = glm::vec4(1.0f)) {
		float hw = size.x / 2.0f;
		float hh = size.y / 2.0f;

		vertices = {
			{ {-hw, -hh}, color },
			{ { hw, -hh}, color },
			{ {-hw,  hh}, color },
			{ { hw,  hh}, color }
		};
		indices = { 0, 1, 2, 1, 3, 2 };

		setPosition(position);
	}

};

class Line : public Shape {
public:
	Line(glm::vec2 p1, glm::vec2 p2, float thickness = 2.0f, glm::vec4 color = glm::vec4(1.0f)) {
		glm::vec2 dir = p2 - p1;
		float length = glm::length(dir);
		if (length < 0.0001f) return;

		glm::vec2 normal = glm::normalize(glm::vec2(-dir.y, dir.x)) * (thickness / 2.0f);

		// Define local vertices relative to the starting point p1
		vertices = {
			{ -normal, color },            // Bottom-Left
			{  normal, color },            // Top-Left
			{ dir - normal, color },       // Bottom-Right
			{ dir + normal, color }        // Top-Right
		};
		indices = { 0, 1, 2, 1, 3, 2 };

		// Use p1 as the world transform anchor point
		setPosition(p1);
	}
};

class Triangle : public Shape {
public:
	Triangle() = default;
	Triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec4 color = glm::vec4(1.0f)) {
		glm::vec2 center = (p0 + p1 + p2) / 3.0f;

		vertices = {
			{ p0 - center, color },
			{ p1 - center, color },
			{ p2 - center, color }
		};
		indices = { 0, 1, 2 };

		setPosition(center);
	}

	Triangle& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Triangle& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Triangle& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Triangle& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }
	Triangle& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
	Triangle& rotate(float degrees) { Transformable::rotate(degrees); return *this; }
};

class Arrow : public Shape {
public:
	Arrow() {
		generateGeometry();
	}

	Arrow(glm::vec2 direction, float shaftThickness = 2.0f, float headSize = 10.0f, glm::vec4 color = glm::vec4(1.0f))
		: m_Direction(direction), m_ShaftThickness(shaftThickness), m_HeadSize(headSize), m_Color(color) {
		generateGeometry();
	}

	Arrow& setDirection(const glm::vec2& direction) {
		m_Direction = direction;
		generateGeometry();
		return *this;
	}

	Arrow& setShaftThickness(float thickness) {
		m_ShaftThickness = thickness;
		generateGeometry();
		return *this;
	}

	Arrow& setHeadSize(float size) {
		m_HeadSize = size;
		generateGeometry();
		return *this;
	}

	Arrow& setColor(const glm::vec4& color) {
		m_Color = color;
		for (auto& vertex : vertices) {
			vertex.color = color;
		}
		return *this;
	}

	// Getters for inspection
	const glm::vec2& getDirection() const { return m_Direction; }
	float getShaftThickness() const { return m_ShaftThickness; }
	float getHeadSize() const { return m_HeadSize; }
	const glm::vec4& getColor() const { return m_Color; }

	// OVERRIDES: Covariant method chaining updates for Transformable base methods
	Arrow& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Arrow& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Arrow& setScale(const glm::vec2& scale) { Transformable::setScale(scale); return *this; }
	Arrow& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }
	Arrow& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
	Arrow& rotate(float degrees) { Transformable::rotate(degrees); return *this; }

private:

	glm::vec2 m_Direction{ 1.0f, 0.0f };
	float m_ShaftThickness{ 2.0f };
	float m_HeadSize{ 10.0f };
	glm::vec4 m_Color{ 1.0f };

	void generateGeometry() {
		vertices.clear();
		indices.clear();

		float length = glm::length(m_Direction);
		if (length < 0.0001f) return;

		float shaftLength = std::max(0.0f, length - m_HeadSize);
		float halfShaft = m_ShaftThickness / 2.0f;
		float halfHead = m_HeadSize;


		vertices = {

			{ { 0.0f,        -halfShaft }, m_Color },
			{ { 0.0f,         halfShaft }, m_Color },
			{ { shaftLength, -halfShaft }, m_Color },
			{ { shaftLength,  halfShaft }, m_Color },

			{ { shaftLength, -halfHead },  m_Color },
			{ { shaftLength,  halfHead },  m_Color },
			{ { length,       0.0f },      m_Color }
		};

		indices = {
			0, 1, 2, 1, 3, 2, 
			4, 5, 6           
		};

		float angle = glm::degrees(atan2(m_Direction.y, m_Direction.x));
		setRotation(angle);
	}
};

class Circle : public Shape {
private:
	float m_radius{ 1.0f };
	glm::vec4 m_Color{ 1.0f };

public:
	Circle() = default;
	Circle(float radius, glm::vec4 color = glm::vec4(1.0f), uint32_t segments = 32)
		: m_radius(radius), m_Color(color) {
		regenerateGeometry(segments); // Default resolution
	}

	Circle& setSegments(uint32_t segments) {
		if (segments < 3) segments = 3;
		regenerateGeometry(segments);
		return *this;
	}

	Circle& setRadius(float radius) {
		m_radius = radius;
		regenerateGeometry(32);
		return *this;
	}

	Circle& setScale(const glm::vec2& scale) { Transformable::setScale(glm::vec2(scale)); return *this; }
	Circle& setPosition(const glm::vec2& pos) { Transformable::setPosition(pos); return *this; }
	Circle& setRotation(float degrees) { Transformable::setRotation(degrees); return *this; }
	Circle& setOrigin(const glm::vec2& origin) { Transformable::setOrigin(origin); return *this; }
	Circle& move(const glm::vec2& offset) { Transformable::move(offset); return *this; }
	Circle& rotate(float degrees) { Transformable::rotate(degrees); return *this; }

private:
	void regenerateGeometry(uint32_t segments) {
		vertices.clear();
		indices.clear();
		if (segments < 3) segments = 3;

		vertices.push_back({ { 0.0f, 0.0f }, m_Color });

		for (uint32_t i = 0; i < segments; ++i) {
			float angle = (static_cast<float>(i) / segments) * 2.0f * glm::pi<float>();
			glm::vec2 pos = glm::vec2(cos(angle), sin(angle)) * m_radius;
			vertices.push_back({ pos, m_Color });
		}

		for (uint32_t i = 1; i <= segments; ++i) {
			uint32_t next = (i == segments) ? 1 : i + 1;
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(next);
		}
	}
};

class Point : public Shape {
public:
	Point(glm::vec4 color = glm::vec4(1.0f), float size = 2.0f) {
		float halfSize = size / 2.0f;
		vertices = {
			{ {-halfSize, -halfSize}, color },
			{ { halfSize, -halfSize}, color },
			{ {-halfSize,  halfSize}, color },
			{ { halfSize,  halfSize}, color }
		};
		indices = { 0, 1, 2, 1, 3, 2 };
	}
};


class ShapeRenderer {
public:
	VertexLayout shapeLayout;
	Shader defaultShader;
	View view;
	GenericBatchRenderer renderer;
	void Init() {
		defaultShader.create(SHADERS "shape.vert", SHADERS "SHAPE.frag");

		shapeLayout.size = sizeof(ShapeVertex);
		shapeLayout.attributes = {
			{2, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, position)},
			{4, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, color)}
		};

		renderer.Init(shapeLayout);
	}

	glm::vec2 transformPosition(const Transformable& transformable, glm::vec2 position) {
		if (abs(transformable.getRotation()) < 0.001f) {
			return (position * transformable.getScale()) + transformable.getPosition();
		}
		else {
			return glm::vec2(transformable.getTransformMatrix() * glm::vec4(position, 0.0f, 1.0f));
		}
	}

	void Add(const Shape& shape) {
		glm::mat4 modelMatrix = shape.getTransformMatrix();

		std::vector<ShapeVertex> transformedVertices;
		transformedVertices.reserve(shape.vertices.size());

		for (const auto& localVertex : shape.vertices) {
			ShapeVertex worldVertex;
			worldVertex.color = localVertex.color;
			worldVertex.position = glm::vec2(modelMatrix * glm::vec4(localVertex.position, 0.0f, 1.0f));
			transformedVertices.push_back(worldVertex);
		}

		renderer.PushGeometry(
			transformedVertices.data(),
			transformedVertices.size(),
			shape.indices.data(),
			shape.indices.size()
		);
	}

	void Draw() {
		glm::mat4 viewProj = view.getViewProjMatrix();
		defaultShader.use();
		defaultShader.setMat4("viewProj", viewProj);

		renderer.Flush();
	}

	void setView(const View& newView) {
		view = newView;
	}

private:

};