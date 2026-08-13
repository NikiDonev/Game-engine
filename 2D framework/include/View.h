#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//class View {
//public:
//	glm::vec2 pos{}, origin{};
//	float rotation{};
//	int width{}, height{};
//	glm::vec2 scale = glm::vec2(1.0f);
//
//	View() = default;
//	View(int w, int h) : width(w), height(h) {}
//
//	void setSize(int Width, int Height) {
//		width = Width;
//		height = Height;
//	}
//	glm::mat4 getViewMatrix() const {
//		glm::mat4 cameraTransform = glm::mat4(1.0f);
//
//		//scale.x = std::max(0.001f, scale.x);
//		//scale.y = std::max(0.001f, scale.y);
//
//		cameraTransform = glm::translate(cameraTransform, glm::vec3(pos, 0.0f));
//		cameraTransform = glm::translate(cameraTransform, glm::vec3(origin, 0.0f));
//		cameraTransform = glm::rotate(cameraTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
//		cameraTransform = glm::scale(cameraTransform, glm::vec3(scale, 1.0f));
//		cameraTransform = glm::translate(cameraTransform, glm::vec3(-origin, 0.0f));
//		return cameraTransform;
//		//return glm::inverse(cameraTransform);
//	}
//	glm::mat4 getProjectionMatrix() const {
//		return glm::ortho(-(float)width / 2.0f, (float)width / 2.0f, -(float)height / 2.0f, (float)height / 2.0f);
//	}
//	glm::mat4 getViewProjMatrix() const {
//		return getProjectionMatrix()* getViewMatrix();
//	}
//
//	operator glm::mat4() {
//		return getViewMatrix();
//	}
//};

class Transformable {
protected:
    glm::vec2 m_Position{ 0.0f };
    glm::vec2 m_Origin{ 0.0f };
    glm::vec2 m_Scale{ 1.0f };
    float m_Rotation{ 0.0f }; // In degrees

public:
    virtual ~Transformable() = default;

    Transformable& setPosition(const glm::vec2& pos) { m_Position = pos; return *this; }
    Transformable& setOrigin(const glm::vec2& origin) { m_Origin = origin; return *this; }
    Transformable& setScale(const glm::vec2& scale) {
        m_Scale.x = std::max(scale.x, 0.0001f);
        m_Scale.y = std::max(scale.y, 0.0001f);
        return *this;
    }
    Transformable& setRotation(float degrees) { m_Rotation = degrees; return *this; }

    const glm::vec2& getPosition() const { return m_Position; }
    const glm::vec2& getOrigin() const { return m_Origin; }
    const glm::vec2& getScale() const { return m_Scale; }
    float getRotation() const { return m_Rotation; }

    void move(const glm::vec2& offset) { m_Position += offset; }
    void rotate(float degrees) { m_Rotation += degrees; }
    void scale(const glm::vec2& factor) { if (factor.x > 0.0f && factor.y > 0.0f) m_Scale *= factor; }

    glm::mat4 getTransformMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_Position, 0.0f));
        model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_Scale, 1.0f));
        model = glm::translate(model, glm::vec3(-m_Origin, 0.0f));
        return model;
    }
};

class View : public Transformable {
public:

	View() = default;
	View(float width, float height) : m_Size(width, height) {}
	View(const glm::vec2& size) : m_Size(size) {}

	void setSize(float width, float height) { m_Size = { width, height }; }
	void setSize(const glm::vec2& size) { m_Size = size; }
	const glm::vec2& getSize() const { return m_Size; }
	
	void setZoom(float zoom) { m_Zoom = std::max(zoom, 0.0001f); }
	float getZoom() const { return m_Zoom; }
	void zoom(float factor) { m_Zoom = std::max(m_Zoom * factor, 0.0001f); }


	glm::mat4 getProjectionMatrix() const {
		glm::vec2 halfSize = m_Size * m_Zoom / 2.0f;
		return glm::ortho(-halfSize.x, halfSize.x, -halfSize.y, halfSize.y, -1.0f, 1.0f);
	}
	glm::mat4 getViewMatrix() const {
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-m_Position, 0.0f));
		return view;
	}
	glm::mat4 getViewProjMatrix() const {
		return getProjectionMatrix() * getViewMatrix();
	}
    void setScale(const glm::vec2& scale) = delete;
    const glm::vec2& getScale() = delete;
    glm::mat4 getTransformMatrix() = delete;

private:
	glm::vec2 m_Size{ 800.0f, 600.0f };
	float m_Zoom{ 1.0f };
};