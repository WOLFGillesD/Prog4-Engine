#pragma once

#include "Component.h"
#include "Command.h"
#include "DaeTime.h"
#include "States.h"
#include "GridComponent.h"
#include "Renderer.h"

namespace game
{
	class PlayerComponent;

	// +----------------------------------------+
	// |               States		            |
	// +----------------------------------------+

    struct MoveState : IState
    {
        PlayerComponent& player;
        int dx{};
    	int dy{};

        glm::ivec2 startCell{};

        MoveState(PlayerComponent& p, int _dx, int _dy)
            : player(p), dx(_dx), dy(_dy)
        {
        }

        void OnEnter() override;
        void OnExit() override;
        void Update() override;
    };

    struct IdleState : IState
    {
        PlayerComponent& player;

        IdleState(PlayerComponent& p)
            : player(p)
        {
        }

        void OnEnter() override;
        void OnExit() override{}
        void Update() override;
    };

    class PlayerComponent final : public dae::Component
    {
    public:
        PlayerComponent(dae::GameObject& go, GridComponent* pGrid, int startRow, int startColumn, float speed = 50.f)
            : Component(go)
            , idleState { *this }
            , moveUp    { *this,  0, -1 }
            , moveDown  { *this,  0, +1 }
            , moveLeft  { *this, -1,  0 }
            , moveRight { *this, +1,  0 }
            , m_pGrid{ pGrid }
            , m_Row{ startRow }
            , m_Column{ startColumn }
			, m_Speed(speed)
        {
            ChangeState(&idleState);
        }

        void ChangeState(IState* newState);
        void Update() override;
        void Render() const override
        {
            SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255);

            SDL_Rect rect{ static_cast<int>(GetOwner()->GetLocalPosition().x), static_cast<int>(GetOwner()->GetLocalPosition().y), 20, 20};
            SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);
        }

        GridComponent* GetGrid() const { return m_pGrid; }
        glm::ivec2 GetCell() const { return { m_Row, m_Column }; }
        void SetCell(int r, int c) { m_Row = r; m_Column = c; }

        float GetSpeed() const { return m_Speed; }

        bool IsMovingUp()    const { return m_MovingDirection.y == -1.f; }
        bool IsMovingDown()  const { return m_MovingDirection.y == 1.f; }
        bool IsMovingLeft()  const { return m_MovingDirection.x == -1.f; }
        bool IsMovingRight() const { return m_MovingDirection.x == 1.f; }

        glm::vec2 m_MovingDirection{};

        IdleState   idleState;
        MoveState   moveUp;
        MoveState   moveDown;
        MoveState   moveLeft;
    	MoveState   moveRight;
        IState* currentState{ nullptr };
    private:

        GridComponent*  m_pGrid;
        int             m_Row;
    	int             m_Column;

        float m_Speed = 80.f; 
    };

    class MoveCommand final : public dae::Command
	{
		PlayerComponent* m_pActor;
		glm::vec2 m_InputDirection{};
	public:
		MoveCommand(PlayerComponent* pActor, const glm::vec2& inputDirection);

		void Execute() override;
        void SetInput(const glm::vec2& v2) { m_InputDirection = v2; }
	};

}

//namespace game
//{
//
//	// +----------------------------------------+
//	// |               States		            |
//	// +----------------------------------------+
//
//	class MovingState : public SMState
//	{
//	public:
//		// Inherited via SMState
//		void OnEnter(SMData* const pData) override;
//		void OnExit(SMData* const pData) override;
//		void Update(SMData* const pData) override;
//	};
//
//	class IdleState : public SMState
//	{
//	public:
//		void OnEnter(SMData* const pData) override;
//		void OnExit(SMData* const pData) override;
//		void Update(SMData* const pData) override;
//	};
//
//	// +----------------------------------------+
//	// |               Conditions	            |
//	// +----------------------------------------+
//
//	class ReverseCondition : public SMCondition
//	{
//	public:
//		bool Evaluate(SMData* const pBlackboard) const override;
//	};
//
//	// +----------------------------------------+
//	// |               Components	            |
//	// +----------------------------------------+
//
//	class PlayerComponent final : public dae::Component
//	{
//	public:
//		PlayerComponent(dae::GameObject& go, GridComponent* pGrid, int row, int column);
//
//		SMData* GetSMData() const
//		{
//			return m_movementState->GetSMData();
//		}
//
//	private:
//
//		std::unique_ptr<SMData> CreateSMData() const;
//
//		int m_Row{};
//		int m_Column{};
//
//		GridComponent* m_pGrid{ nullptr };
//
//		std::unique_ptr<StateMachine> m_movementState;
//
//		std::unique_ptr<MovingState> m_movingState{ std::make_unique<MovingState>()};
//		std::unique_ptr<IdleState> m_idleState{ std::make_unique<IdleState>() };
//
//		std::unique_ptr<ReverseCondition> m_reverseCondition{ std::make_unique<ReverseCondition>() };
//	};
//
//	// +----------------------------------------+
//	// |               Commands		            |
//	// +----------------------------------------+
//
//	class MoveCommand final : public dae::Command
//	{
//		PlayerComponent* m_pActor;
//		glm::vec2 m_InputDirection{};
//	public:
//		MoveCommand(PlayerComponent& pActor, const glm::vec2& inputDirection, float speed = 25.f);
//
//		void Execute() override;
//		void SetInput(const glm::vec2& v2);
//	};
//
//}