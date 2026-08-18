#pragma once

#include "GenericBatchRenderer.h"
#include "Sprite.h"
#include "Shape.h"
#include "Shader.h"


class ShapeRenderer {
public:
	VertexLayout shapeLayout;
	Ref<Shader> defaultShader;
	View view;
	GenericBatchRenderer renderer;
	void Init();

	glm::vec2 transformPosition(const Transformable& transformable, glm::vec2 position);

	AABB getWorldBounds(const std::vector<ShapeVertex>& transformedVertices);

	void Add(const Shape& shape);

	void Draw();

	void setView(const View& newView) { view = newView; }

private:

};