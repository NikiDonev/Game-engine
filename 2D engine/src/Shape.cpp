#include "Shape.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>

#pragma region Circle

Circle::Circle(float radius, glm::vec4 color, uint32_t segments)
	: m_Radius(radius), m_Color(color), m_Segments(segments) {
	generateGeometry();
}

Circle& Circle::setOutline(float thickness, glm::vec4 color) {
	m_OutlineThickness = thickness;
	m_OutlineColor = color;
	generateGeometry();
	return *this;
}

Circle& Circle::setSegments(uint32_t segments) {
	m_Segments = std::max(3u, segments);
	generateGeometry();
	return *this;
}

Circle& Circle::setRadius(float radius) {
	m_Radius = radius;
	generateGeometry();
	return *this;
}

void Circle::generateGeometry() {
	vertices.clear();
	indices.clear();

	float innerRadius = m_Radius;
	float outerRadius = m_Radius;

	if (m_OutlineThickness < -0.0001f) {
		innerRadius = std::max(0.0f, m_Radius + m_OutlineThickness);
	}
	else if (m_OutlineThickness > 0.0001f) {
		outerRadius = m_Radius + m_OutlineThickness;
	}

	vertices.push_back({ { 0.0f, 0.0f }, m_Color });

	for (uint32_t i = 0; i < m_Segments; ++i) {
		float angle = (static_cast<float>(i) / m_Segments) * 2.0f * glm::pi<float>();
		glm::vec2 unitDir = glm::vec2(cos(angle), sin(angle));
		vertices.push_back({ unitDir * innerRadius, m_Color });
	}

	for (uint32_t i = 1; i <= m_Segments; ++i) {
		uint32_t next = (i == m_Segments) ? 1 : i + 1;
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(next);
	}

	if (std::abs(m_OutlineThickness) > 0.0001f) {
		uint32_t outlineInnerOffset = vertices.size();
		for (uint32_t i = 0; i < m_Segments; ++i) {
			float angle = (static_cast<float>(i) / m_Segments) * 2.0f * glm::pi<float>();
			glm::vec2 unitDir = glm::vec2(cos(angle), sin(angle));
			vertices.push_back({ unitDir * innerRadius, m_OutlineColor });
		}

		uint32_t outlineOuterOffset = vertices.size();
		for (uint32_t i = 0; i < m_Segments; ++i) {
			float angle = (static_cast<float>(i) / m_Segments) * 2.0f * glm::pi<float>();
			glm::vec2 unitDir = glm::vec2(cos(angle), sin(angle));
			vertices.push_back({ unitDir * outerRadius, m_OutlineColor });
		}

		for (uint32_t i = 0; i < m_Segments; ++i) {
			uint32_t next = (i + 1) % m_Segments;

			uint32_t outlineInnerCurrent = outlineInnerOffset + i;
			uint32_t outlineInnerNext = outlineInnerOffset + next;
			uint32_t outlineOuterCurrent = outlineOuterOffset + i;
			uint32_t outlineOuterNext = outlineOuterOffset + next;

			indices.push_back(outlineInnerCurrent);
			indices.push_back(outlineOuterCurrent);
			indices.push_back(outlineInnerNext);

			indices.push_back(outlineOuterCurrent);
			indices.push_back(outlineOuterNext);
			indices.push_back(outlineInnerNext);
		}
	}
	m_UpdateBounds = true;
}

#pragma endregion

#pragma region Rect
Rect::Rect(glm::vec2 size, glm::vec4 color)
	: m_Size(size), m_Color(color) {
	generateGeometry();
}

Rect& Rect::setOutline(float thickness, glm::vec4 color) {
	m_OutlineThickness = thickness;
	m_OutlineColor = color;
	generateGeometry();
	return *this;
}

Rect& Rect::setSize(const glm::vec2& size) {
	m_Size = size;
	generateGeometry();
	return *this;
}

Rect& Rect::setColor(const glm::vec4& color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

void Rect::generateGeometry() {
	vertices.clear();
	indices.clear();

	float hw = m_Size.x / 2.0f;
	float hh = m_Size.y / 2.0f;

	float innerW = hw, innerH = hh;
	float outerW = hw, outerH = hh;

	if (m_OutlineThickness < -0.0001f) {
		innerW = std::max(0.0f, hw + m_OutlineThickness);
		innerH = std::max(0.0f, hh + m_OutlineThickness);
	}
	else if (m_OutlineThickness > 0.0001f) {
		outerW = hw + m_OutlineThickness;
		outerH = hh + m_OutlineThickness;
	}

	vertices.push_back({ {-innerW, -innerH}, m_Color });
	vertices.push_back({ { innerW, -innerH}, m_Color });
	vertices.push_back({ {-innerW,  innerH}, m_Color });
	vertices.push_back({ { innerW,  innerH}, m_Color });
	indices = { 0, 1, 2, 1, 3, 2 };

	if (std::abs(m_OutlineThickness) > 0.0001f) {
		uint32_t inOffset = vertices.size();
		vertices.push_back({ {-innerW, -innerH}, m_OutlineColor });
		vertices.push_back({ { innerW, -innerH}, m_OutlineColor });
		vertices.push_back({ {-innerW,  innerH}, m_OutlineColor });
		vertices.push_back({ { innerW,  innerH}, m_OutlineColor });

		uint32_t outOffset = vertices.size();
		vertices.push_back({ {-outerW, -outerH}, m_OutlineColor });
		vertices.push_back({ { outerW, -outerH}, m_OutlineColor });
		vertices.push_back({ {-outerW,  outerH}, m_OutlineColor });
		vertices.push_back({ { outerW,  outerH}, m_OutlineColor });

		std::vector<std::pair<int, int>> edges = { {0, 1}, {1, 3}, {3, 2}, {2, 0} };
		for (const auto& edge : edges) {
			uint32_t iC = inOffset + edge.first;
			uint32_t iN = inOffset + edge.second;
			uint32_t oC = outOffset + edge.first;
			uint32_t oN = outOffset + edge.second;

			indices.push_back(iC); indices.push_back(oC); indices.push_back(iN);
			indices.push_back(oC); indices.push_back(oN); indices.push_back(iN);
		}
	}
}

#pragma endregion

#pragma region Triangle

Triangle::Triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec4 color)
	: m_P0(p0), m_P1(p1), m_P2(p2), m_Color(color) {
	generateGeometry();
}

Triangle& Triangle::setOutline(float thickness, glm::vec4 color) {
	m_OutlineThickness = thickness;
	m_OutlineColor = color;
	generateGeometry();
	return *this;
}

Triangle& Triangle::setColor(glm::vec4 color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

Triangle& Triangle::setPoints(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2) {
	m_P0 = p0; m_P1 = p1; m_P2 = p2;
	generateGeometry();
	return *this;
}

void Triangle::generateGeometry() {
	vertices.clear();
	indices.clear();

	glm::vec2 center = (m_P0 + m_P1 + m_P2) / 3.0f;

	// 1. Calculate normalized edge directions
	glm::vec2 e01 = glm::normalize(m_P1 - m_P0);
	glm::vec2 e12 = glm::normalize(m_P2 - m_P1);
	glm::vec2 e20 = glm::normalize(m_P0 - m_P2);

	// 2. Compute perpendicular edge normals (assuming Counter-Clockwise winding)
	glm::vec2 n01 = glm::vec2(-e01.y, e01.x);
	glm::vec2 n12 = glm::vec2(-e12.y, e12.x);
	glm::vec2 n20 = glm::vec2(-e20.y, e20.x);

	// 3. Ensure edge normals always face OUTWARD relative to the center 
	// (This keeps the code working regardless of vertex winding order)
	if (glm::dot(n01, m_P0 - center) < 0.0f) {
		n01 = -n01;
		n12 = -n12;
		n20 = -n20;
	}

	// 4. Calculate the perfect miter vector for each vertex
	glm::vec2 edgeNormals[3][2] = {
		{ n20, n01 }, // Vertex 0 (P0) is between Edge 20 and Edge 01
		{ n01, n12 }, // Vertex 1 (P1) is between Edge 01 and Edge 12
		{ n12, n20 }  // Vertex 2 (P2) is between Edge 12 and Edge 20
	};

	glm::vec2 localPoints[3] = { m_P0 - center, m_P1 - center, m_P2 - center };
	glm::vec2 innerPoints[3];
	glm::vec2 outerPoints[3];

	for (int i = 0; i < 3; ++i) {
		glm::vec2 n_prev = edgeNormals[i][0];
		glm::vec2 n_next = edgeNormals[i][1];

		// Miter vector formula: gives perfect direction and miter extension length
		glm::vec2 miterVector = (n_prev + n_next) / (1.0f + glm::dot(n_prev, n_next));

		if (m_OutlineThickness < -0.0001f) {
			innerPoints[i] = localPoints[i] + miterVector * m_OutlineThickness;
			outerPoints[i] = localPoints[i];
		}
		else {
			innerPoints[i] = localPoints[i];
			outerPoints[i] = localPoints[i] + miterVector * m_OutlineThickness;
		}
	}

	// 5. Core Fill
	for (int i = 0; i < 3; ++i) vertices.push_back({ innerPoints[i], m_Color });
	indices.push_back(0); indices.push_back(1); indices.push_back(2);

	// 6. Outline Ring
	if (std::abs(m_OutlineThickness) > 0.0001f) {
		uint32_t innerOffset = static_cast<uint32_t>(vertices.size());
		for (int i = 0; i < 3; ++i) vertices.push_back({ innerPoints[i], m_OutlineColor });

		uint32_t outerOffset = static_cast<uint32_t>(vertices.size());
		for (int i = 0; i < 3; ++i) vertices.push_back({ outerPoints[i], m_OutlineColor });

		for (int i = 0; i < 3; ++i) {
			int next = (i + 1) % 3;
			indices.push_back(innerOffset + i); indices.push_back(outerOffset + i); indices.push_back(innerOffset + next);
			indices.push_back(outerOffset + i); indices.push_back(outerOffset + next); indices.push_back(innerOffset + next);
		}
	}

	setPosition(center);
	m_UpdateBounds = true;
}

#pragma endregion

#pragma region Arrow

Arrow::Arrow(glm::vec2 direction, float shaftThickness, float headSize, glm::vec4 color)
	: m_Direction(direction), m_ShaftThickness(shaftThickness), m_HeadSize(headSize), m_Color(color) {
	generateGeometry();
}

Arrow& Arrow::setDirection(const glm::vec2& direction) {
	m_Direction = direction;
	generateGeometry();
	return *this;
}

Arrow& Arrow::setShaftThickness(float thickness) {
	m_ShaftThickness = thickness;
	generateGeometry();
	return *this;
}

Arrow& Arrow::setHeadSize(float size) {
	m_HeadSize = size;
	generateGeometry();
	return *this;
}

Arrow& Arrow::setColor(const glm::vec4& color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

void Arrow::generateGeometry() {
	vertices.clear();
	indices.clear();

	float length = glm::length(m_Direction);
	if (length < 0.0001f) return;

	float shaftLength = std::max(0.0f, length - m_HeadSize);
	float halfShaft = m_ShaftThickness / 2.0f;
	float halfHead = m_HeadSize;

	glm::vec2 basePoints[7] = {
		{ 0.0f,        -halfShaft }, { 0.0f,         halfShaft },
		{ shaftLength, -halfShaft }, { shaftLength,  halfShaft },
		{ shaftLength, -halfHead },  { shaftLength,  halfHead },
		{ length,       0.0f }
	};

	glm::vec2 offsets[7] = {
		{ -1.0f, -1.0f }, { -1.0f, 1.0f },
		{  0.0f, -1.0f }, {  0.0f, 1.0f },
		{ -1.0f, -1.0f }, { -1.0f, 1.0f },
		{  1.4f,  0.0f }
	};

	glm::vec2 innerPoints[7];
	glm::vec2 outerPoints[7];

	float t = m_OutlineThickness;
	for (int i = 0; i < 7; ++i) {
		if (t < -0.0001f) {
			innerPoints[i] = basePoints[i] + offsets[i] * t;
			outerPoints[i] = basePoints[i];
		}
		else {
			innerPoints[i] = basePoints[i];
			outerPoints[i] = basePoints[i] + offsets[i] * t;
		}
	}

	for (int i = 0; i < 7; ++i) vertices.push_back({ innerPoints[i], m_Color });
	indices = { 0, 1, 2,  1, 3, 2,  4, 5, 6 };

	if (std::abs(t) > 0.0001f) {
		uint32_t inOffset = vertices.size();
		for (int i = 0; i < 7; ++i) vertices.push_back({ innerPoints[i], m_OutlineColor });

		uint32_t outOffset = vertices.size();
		for (int i = 0; i < 7; ++i) vertices.push_back({ outerPoints[i], m_OutlineColor });

		std::vector<std::pair<int, int>> edges = {
			{0, 1}, {1, 3}, {3, 5}, {5, 6}, {6, 4}, {4, 2}, {2, 0}
		};

		for (const auto& edge : edges) {
			uint32_t iC = inOffset + edge.first;
			uint32_t iN = inOffset + edge.second;
			uint32_t oC = outOffset + edge.first;
			uint32_t oN = outOffset + edge.second;

			indices.push_back(iC); indices.push_back(oC); indices.push_back(iN);
			indices.push_back(oC); indices.push_back(oN); indices.push_back(iN);
		}
	}

	float angle = glm::degrees(atan2(m_Direction.y, m_Direction.x));
	setRotation(angle);
	m_UpdateBounds = true;
}

#pragma endregion

#pragma region RoundedRect

RoundedRect::RoundedRect(glm::vec2 size, float cornerRadius, glm::vec4 color, uint32_t segments)
	: m_Size(size), m_CornerRadius(cornerRadius), m_Color(color), m_Segments(segments) {
	generateGeometry();
}

RoundedRect& RoundedRect::setOutline(float thickness, glm::vec4 color) {
	m_OutlineThickness = thickness;
	m_OutlineColor = color;
	generateGeometry();
	return *this;
}

RoundedRect& RoundedRect::setPrecision(uint32_t segments) {
	m_Segments = std::max(1u, segments);
	generateGeometry();
	return *this;
}

RoundedRect& RoundedRect::setSize(glm::vec2 size) {
	m_Size = size;
	generateGeometry();
	return *this;
}

RoundedRect& RoundedRect::setColor(glm::vec4 color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

void RoundedRect::generateGeometry() {
	vertices.clear();
	indices.clear();

	float maxRadius = std::min(m_Size.x, m_Size.y) / 2.0f;
	float baseRadius = std::min(m_CornerRadius, maxRadius);

	float innerRadius = baseRadius;
	float outerRadius = baseRadius;

	if (m_OutlineThickness < -0.0001f) {
		innerRadius = std::max(0.0f, baseRadius + m_OutlineThickness);
	}
	else if (m_OutlineThickness > 0.0001f) {
		outerRadius = baseRadius + m_OutlineThickness;
	}

	float hw = m_Size.x / 2.0f;
	float hh = m_Size.y / 2.0f;

	glm::vec2 centers[4] = {
		{  hw - baseRadius,  hh - baseRadius }, // Top Right
		{ -hw + baseRadius,  hh - baseRadius }, // Top Left
		{ -hw + baseRadius, -hh - baseRadius }, // Bottom Left
		{  hw - baseRadius, -hh - baseRadius }  // Bottom Right
	};

	float angles[4] = { 0.0f, glm::half_pi<float>(), glm::pi<float>(), glm::three_over_two_pi<float>() };

	vertices.push_back({ { 0.0f, 0.0f }, m_Color });

	for (int i = 0; i < 4; ++i) {
		for (uint32_t s = 0; s <= m_Segments; ++s) {
			float alpha = angles[i] + (static_cast<float>(s) / m_Segments) * glm::half_pi<float>();
			glm::vec2 dir(cos(alpha), sin(alpha));
			vertices.push_back({ centers[i] + dir * innerRadius, m_Color });
		}
	}

	uint32_t perimeterPoints = vertices.size() - 1;
	for (uint32_t i = 1; i <= perimeterPoints; ++i) {
		uint32_t next = (i == perimeterPoints) ? 1 : i + 1;
		indices.push_back(0); indices.push_back(i); indices.push_back(next);
	}

	if (std::abs(m_OutlineThickness) > 0.0001f) {
		uint32_t outlineInnerOffset = vertices.size();
		for (int i = 0; i < 4; ++i) {
			for (uint32_t s = 0; s <= m_Segments; ++s) {
				float alpha = angles[i] + (static_cast<float>(s) / m_Segments) * glm::half_pi<float>();
				glm::vec2 dir(cos(alpha), sin(alpha));
				vertices.push_back({ centers[i] + dir * innerRadius, m_OutlineColor });
			}
		}

		uint32_t outlineOuterOffset = vertices.size();
		for (int i = 0; i < 4; ++i) {
			for (uint32_t s = 0; s <= m_Segments; ++s) {
				float alpha = angles[i] + (static_cast<float>(s) / m_Segments) * glm::half_pi<float>();
				glm::vec2 dir(cos(alpha), sin(alpha));
				vertices.push_back({ centers[i] + dir * outerRadius, m_OutlineColor });
			}
		}

		for (uint32_t i = 0; i < perimeterPoints; ++i) {
			uint32_t next = (i + 1) % perimeterPoints;
			uint32_t inCurr = outlineInnerOffset + i;
			uint32_t inNext = outlineInnerOffset + next;
			uint32_t outCurr = outlineOuterOffset + i;
			uint32_t outNext = outlineOuterOffset + next;

			indices.push_back(inCurr); indices.push_back(outCurr); indices.push_back(inNext);
			indices.push_back(outCurr); indices.push_back(outNext); indices.push_back(inNext);
		}
	}
	m_UpdateBounds = true;
}

#pragma endregion

#pragma region Line

Line::Line(glm::vec2 p1, glm::vec2 p2, float thickness, glm::vec4 color)
	: m_P1(p1), m_P2(p2), m_Thickness(thickness), m_Color(color) {
	generateGeometry();
}

Line& Line::setOutline(float thickness, glm::vec4 color) {
	m_OutlineThickness = thickness;
	m_OutlineColor = color;
	generateGeometry();
	return *this;
}

Line& Line::setPoints(glm::vec2 p1, glm::vec2 p2) {
	m_P1 = p1; m_P2 = p2;
	generateGeometry();
	return *this;
}

Line& Line::setThickness(float thickness) {
	m_Thickness = thickness;
	generateGeometry();
	return *this;
}

Line& Line::setColor(const glm::vec4& color) {
	m_Color = color;
	generateGeometry();
	return *this;
}

void Line::generateGeometry() {
	vertices.clear();
	indices.clear();

	glm::vec2 dir = m_P2 - m_P1;
	float length = glm::length(dir);
	if (length < 0.0001f) return;

	glm::vec2 unitDir = dir / length;
	glm::vec2 unitNormal = glm::vec2(-unitDir.y, unitDir.x);

	float halfThick = m_Thickness / 2.0f;
	float innerHalf = halfThick;
	float outerHalf = halfThick;
	float innerLenStart = 0.0f, innerLenEnd = length;
	float outerLenStart = 0.0f, outerLenEnd = length;

	if (m_OutlineThickness < -0.0001f) {
		innerHalf = std::max(0.0f, halfThick + m_OutlineThickness);
		innerLenStart = std::min(length, -m_OutlineThickness);
		innerLenEnd = std::max(0.0f, length + m_OutlineThickness);
	}
	else if (m_OutlineThickness > 0.0001f) {
		outerHalf = halfThick + m_OutlineThickness;
		outerLenStart = -m_OutlineThickness;
		outerLenEnd = length + m_OutlineThickness;
	}

	auto getPoint = [&](float len, float normalScale) {
		return unitDir * len + unitNormal * normalScale;
		};

	vertices.push_back({ getPoint(innerLenStart, -innerHalf), m_Color });
	vertices.push_back({ getPoint(innerLenEnd,   -innerHalf), m_Color });
	vertices.push_back({ getPoint(innerLenStart,  innerHalf), m_Color });
	vertices.push_back({ getPoint(innerLenEnd,   innerHalf), m_Color });
	indices = { 0, 1, 2, 1, 3, 2 };

	if (std::abs(m_OutlineThickness) > 0.0001f) {
		uint32_t inOffset = vertices.size();
		vertices.push_back({ getPoint(innerLenStart, -innerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(innerLenEnd,   -innerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(innerLenEnd,    innerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(innerLenStart,  innerHalf), m_OutlineColor });

		uint32_t outOffset = vertices.size();
		vertices.push_back({ getPoint(outerLenStart, -outerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(outerLenEnd,   -outerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(outerLenEnd,    outerHalf), m_OutlineColor });
		vertices.push_back({ getPoint(outerLenStart,  outerHalf), m_OutlineColor });

		for (int i = 0; i < 4; ++i) {
			int next = (i + 1) % 4;
			uint32_t iC = inOffset + i;   uint32_t iN = inOffset + next;
			uint32_t oC = outOffset + i;  uint32_t oN = outOffset + next;

			indices.push_back(iC); indices.push_back(oC); indices.push_back(iN);
			indices.push_back(oC); indices.push_back(oN); indices.push_back(iN);
		}
	}
	setPosition(m_P1);
}

#pragma endregion

#pragma region Point

void Point::generateGeometry() {
	vertices.clear(); indices.clear();
	float hs = m_Size / 2.0f;
	vertices = {
		{ {-hs, -hs}, m_Color }, { { hs, -hs}, m_Color },
		{ {-hs,  hs}, m_Color }, { { hs,  hs}, m_Color }
	};
	indices = { 0, 1, 2, 1, 3, 2 };
}

#pragma endregion

