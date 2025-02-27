#pragma once

#include <RenderObject.h>
#include <RenderThread.h>
#include <Vector.h>
#include <cmath>

namespace GameEngine
{
	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(Math::Vector3f pos): GameObject() {
			m_Position = pos;
		}


	public:
		Render::RenderObject** GetRenderObjectRef() { return &m_RenderObject; }

		void SetPosition(Math::Vector3f position, size_t frame)
		{
			m_Position = position;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(position, frame);
			}
		}

		Math::Vector3f GetPosition()
		{
			return m_Position;
		}

		void MovePosition(Math::Vector3f shift, size_t frame) {
			m_Position = m_Position + shift;

			if (m_RenderObject) [[likely]]
			{
				m_RenderObject->SetPosition(m_Position, frame);
			}
		}

		virtual void UpdatePosition(float dt, size_t frame) {
			SetPosition(m_Position, frame);
		}

	protected:
		Render::RenderObject* m_RenderObject = nullptr;

		Math::Vector3f m_Position = Math::Vector3f::Zero();
	};


	class Sliding final : public GameObject
	{
	private:
		Math::Vector3f init_pos;
		float cycle = 0;
		float speed = 1;
		float amp = 3;

	public:
		Sliding() : GameObject(), init_pos(m_Position) {}
		Sliding(Math::Vector3f pos, float speed) : GameObject(pos), init_pos(pos), speed(speed) {}

		virtual void UpdatePosition(float dt, size_t frame) override 
		{
			cycle += dt * speed;
			auto pos = init_pos + Math::Vector3f(1, 1, 1) * std::cos(cycle) * amp;
			SetPosition(pos, frame);
		}
	};

	class Controlled final : public GameObject {
	public:
		Controlled() : GameObject() {}
		Controlled(Math::Vector3f pos) : GameObject(pos) {}
	};

	class Jumping final : public GameObject
	{
	private:
		float acc = 9;
		float speed = 0;

		Math::Vector3f init_pos;

	public:
		Jumping() : GameObject(), init_pos(m_Position) {}
		Jumping(Math::Vector3f pos) : GameObject(pos), init_pos(pos) {}

		virtual void UpdatePosition(float dt, size_t frame) override
		{
			if (m_Position.y <= init_pos.y) {
				speed = -15;
			}
			else {
				speed += acc * dt;
			}
			MovePosition(Math::Vector3f(0, -1, 0) * speed * dt, frame);
		}
	};
}