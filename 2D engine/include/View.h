#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Transformable {
protected:
    glm::vec2 m_Position{ 0.0f };
    glm::vec2 m_Origin{ 0.0f };
    glm::vec2 m_Scale{ 1.0f };
    float m_Rotation{ 0.0f }; // In degrees
    glm::mat4 m_Matrix{ 1.0f };
    bool m_NeedUpdate{ true };
    virtual void markDirty() { m_NeedUpdate = true; }
public:
    virtual ~Transformable() = default;

    Transformable& setPosition(const glm::vec2& pos) { m_Position = pos; markDirty(); return *this; }
    Transformable& setOrigin(const glm::vec2& origin) { m_Origin = origin; markDirty(); return *this; }
    Transformable& setScale(const glm::vec2& scale) {
        m_Scale.x = std::max(scale.x, 0.0001f);
        m_Scale.y = std::max(scale.y, 0.0001f);
        markDirty();
        return *this;
    }
    Transformable& setRotation(float degrees) { m_Rotation = degrees; markDirty(); return *this; }

    const glm::vec2& getPosition() const { return m_Position; }
    const glm::vec2& getOrigin() const { return m_Origin; }
    const glm::vec2& getScale() const { return m_Scale; }
    float getRotation() const { return m_Rotation; }

    void move(const glm::vec2& offset) { m_Position += offset; markDirty(); }
    void rotate(float degrees) { m_Rotation += degrees; markDirty();}
    void scale(const glm::vec2& factor) { if (factor.x > 0.0f && factor.y > 0.0f) m_Scale *= factor; markDirty();}

    glm::mat4 getTransformMatrix() {
        if (m_NeedUpdate) {
            m_Matrix = glm::mat4(1.0f);
            m_Matrix = glm::translate(m_Matrix, glm::vec3(m_Position, 0.0f));
            m_Matrix = glm::rotate(m_Matrix, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            m_Matrix = glm::scale(m_Matrix, glm::vec3(m_Scale, 1.0f));
            m_Matrix = glm::translate(m_Matrix, glm::vec3(-m_Origin, 0.0f));
            m_NeedUpdate = false;
        }
        return m_Matrix;
    }
};

struct AABB {
    glm::vec2 min{ 0.0f };
    glm::vec2 max{ 0.0f };
};

class View : public Transformable {
public:

	View() = default;
	View(float width, float height) : m_Size(width, height) {}
	View(const glm::vec2& size) : m_Size(size) {}

	void setSize(float width, float height) { m_Size = { width, height }; markDirty(); }
    void setSize(const glm::vec2& size) { m_Size = size; markDirty(); }
	const glm::vec2& getSize() const { return m_Size; }
	
	void setZoom(float zoom) { m_Zoom = std::max(zoom, 0.0001f); markDirty(); }
	float getZoom() const { return m_Zoom; }
    void zoom(float factor) { m_Zoom = std::max(m_Zoom * factor, 0.0001f); markDirty(); }


	glm::mat4 getProjectionMatrix() const {
		glm::vec2 halfSize = getSize() * getZoom() / 2.0f;
		return glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, -1.0f, 1.0f);
	}
	glm::mat4 getViewMatrix() const {
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-m_Position, 0.0f));
		return view;
	}
	glm::mat4 getViewProjMatrix() {
        if (m_NeedUpdate) {
            glm::vec2 halfSize = m_Size * (m_Zoom / 2.0f);
            glm::mat4 projection = glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, -1.0f, 1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::rotate(view, glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            view = glm::translate(view, glm::vec3(-m_Position, 0.0f));
            m_Matrix = projection * view;
            m_NeedUpdate = false;
        }
        return m_Matrix;
	}

    void setScale(const glm::vec2& scale) = delete;
    const glm::vec2& getScale() = delete;
    glm::mat4 getTransformMatrix() = delete;
    

    AABB getFrustumBounds() {
        if (m_UpdateFrustumBounds) {
            glm::mat4 invViewProj = glm::inverse(getViewProjMatrix());

            // Screen corners in Normalized Device Coordinates (NDC)
            glm::vec4 ndcCorners[4] = {
                {-1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-Left
                { 1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-Right
                {-1.0f,  1.0f, 0.0f, 1.0f}, // Top-Left
                { 1.0f,  1.0f, 0.0f, 1.0f}  // Top-Right
            };

            glm::vec4 worldCorner = invViewProj * ndcCorners[0];
            m_WorldFrustum.min = glm::vec2(worldCorner.x, worldCorner.y);
            m_WorldFrustum.max = m_WorldFrustum.min;

            for (int i = 1; i < 4; ++i) {
                worldCorner = invViewProj * ndcCorners[i];
                m_WorldFrustum.min.x = std::min(m_WorldFrustum.min.x, worldCorner.x);
                m_WorldFrustum.min.y = std::min(m_WorldFrustum.min.y, worldCorner.y);
                m_WorldFrustum.max.x = std::max(m_WorldFrustum.max.x, worldCorner.x);
                m_WorldFrustum.max.y = std::max(m_WorldFrustum.max.y, worldCorner.y);
            }
            m_UpdateFrustumBounds = false;
        }
        return m_WorldFrustum;
    }

private:
	glm::vec2 m_Size{ 800.0f, 600.0f };
	float m_Zoom{ 1.0f };
    AABB m_WorldFrustum;
    bool m_UpdateFrustumBounds{ true };
protected:
    void markDirty() override { m_NeedUpdate = true; m_UpdateFrustumBounds = true; }
};