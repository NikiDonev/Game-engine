#include "ShapeRenderer.h"
#include <iostream>

void ShapeRenderer::Init() {
	defaultShader.create(SHADERS "shape.vert", SHADERS "SHAPE.frag");

	shapeLayout.size = sizeof(ShapeVertex);
	shapeLayout.attributes = {
		{2, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, position)},
		{4, GL_FLOAT, GL_FALSE, offsetof(ShapeVertex, color)}
	};

	renderer.Init(shapeLayout);
}

glm::vec2 ShapeRenderer::transformPosition(const Transformable& transformable, glm::vec2 position) {
	if (abs(transformable.getRotation()) < 0.001f) {
		return (position * transformable.getScale()) + transformable.getPosition();
	}
	else {
		return glm::vec2(transformable.getTransformMatrix() * glm::vec4(position, 0.0f, 1.0f));
	}
}

AABB ShapeRenderer::getWorldBounds(const std::vector<ShapeVertex>& transformedVertices) {
	if (transformedVertices.empty()) return AABB{};

	glm::vec2 firstWorldPos = transformedVertices[0].position;
	AABB worldBounds{ firstWorldPos, firstWorldPos };

	for (size_t i = 1; i < transformedVertices.size(); ++i) {
		glm::vec2 worldPos = transformedVertices[i].position;
		
		worldBounds.min.x = std::min(worldBounds.min.x, worldPos.x);
		worldBounds.min.y = std::min(worldBounds.min.y, worldPos.y);
		worldBounds.max.x = std::max(worldBounds.max.x, worldPos.x);
		worldBounds.max.y = std::max(worldBounds.max.y, worldPos.y);
	}

	float padding = 1.0f; 
	worldBounds.min.x -= padding;
	worldBounds.min.y -= padding;
	worldBounds.max.x += padding;
	worldBounds.max.y += padding;

	return worldBounds;
}

void ShapeRenderer::Add(const Shape& shape) {

	glm::mat4 modelMatrix = shape.getTransformMatrix();

	std::vector<ShapeVertex> transformedVertices;
	transformedVertices.reserve(shape.vertices.size());

	for (const auto& localVertex : shape.vertices) {
		ShapeVertex worldVertex;
		worldVertex.color = localVertex.color;
		worldVertex.position = transformPosition(shape, localVertex.position);
		transformedVertices.push_back(worldVertex);
	}

	AABB shapeBounds = getWorldBounds(transformedVertices);
	AABB viewBounds = view.getFrustumBounds();

	bool isVisible = (shapeBounds.max.x >= viewBounds.min.x && shapeBounds.min.x <= viewBounds.max.x) &&
		(shapeBounds.max.y >= viewBounds.min.y && shapeBounds.min.y <= viewBounds.max.y);

	if (!isVisible) return;

	renderer.PushGeometry(
		transformedVertices.data(),
		transformedVertices.size(),
		shape.indices.data(),
		shape.indices.size()
	);
}

void ShapeRenderer::Draw() {
	renderer.flushCount = 0;
	glm::mat4 viewProj = view.getViewProjMatrix();
	defaultShader.use();
	defaultShader.setMat4("viewProj", viewProj);

	renderer.Flush();
}