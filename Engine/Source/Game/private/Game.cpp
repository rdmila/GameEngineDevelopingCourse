#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>
#include <random>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 0.0f, 0.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(1, 1, 1).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_int(0, 2);
		std::uniform_real_distribution<float> uniform_real(15, 40);
		std::uniform_real_distribution<float> uniform_real_little(-3, 3);


		// How many objects do we want to create
		for (int i = 0; i < 100; ++i)
		{
			
			int pick = (i == 0 ? 1 : uniform_int(e1));
			auto pos = Math::Vector3f(uniform_real(e1), uniform_real(e1), uniform_real(e1));

			GameObject* obj;

			switch (pick)
			{
			case 0:
				obj = new Sliding(pos, uniform_real_little(e1));
				break;
			case 1:
				army.cubes.push_back(i);
				obj = new Controlled(pos);
				break;
			case 2:
				obj = new Jumping(pos);
				break;
			}
			m_Objects.push_back(dynamic_cast<GameObject*>(obj));

			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("MoveCubeLeft", [&]() { army.dir = 1; });
		Core::g_InputHandler->RegisterCallback("MoveCubeRight", [&]() { army.dir = -1; });
		Core::g_InputHandler->RegisterCallback("NextCube", [&]() { army.nextCube(); });

	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}
	
	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			if (i == army.getCurId()) {
				m_Objects[i]->MovePosition(Math::Vector3f(30 * army.dir * dt, 0, 0), m_renderThread->GetMainFrame());
				army.dir = 0;
			}
			m_Objects[i]->UpdatePosition(dt, m_renderThread->GetMainFrame());

		}
	}

	inline void Game::Army::nextCube() 
	{
		++cur;
		cur %= cubes.size();
	}

	inline int Game::Army::getCurId()
	{
		return cubes[cur];
	}
}