#include "PlayerComponent.h"

#include "DaeTime.h"

namespace game
{
	//void IdleState::OnEnter()
	//{
	//	const auto cell = player.GetCell();
	//	const glm::vec2 center = player.GetGrid()->GetCellCenter(cell.x, cell.y);
	//	player.GetOwner()->SetLocalPosition(center.x, center.y);
	//}

	//void IdleState::Update()
	//{
	//	auto cell = player.GetCell();
	//	const auto& grid = *player.GetGrid();

	//	if (player.IsMovingUp() && grid.IsCellValid(cell.x - 1, cell.y))    player.ChangeState(&player.moveUp);
	//	else if (player.IsMovingDown() && grid.IsCellValid(cell.x + 1, cell.y))  player.ChangeState(&player.moveDown);
	//	else if (player.IsMovingLeft() && grid.IsCellValid(cell.x, cell.y - 1))  player.ChangeState(&player.moveLeft);
	//	else if (player.IsMovingRight() && grid.IsCellValid(cell.x, cell.y + 1)) player.ChangeState(&player.moveRight);
	//}

	void PlayerComponent::ChangeState(IState* newState)
	{
		if (currentState != nullptr) currentState->OnExit();
		currentState = newState;
		currentState->OnEnter();
	}

	void PlayerComponent::Update()
	{
		
	}

	//MoveCommand::MoveCommand(PlayerComponent* pActor, const glm::vec2& inputDirection)
	//	: m_pActor(pActor), m_InputDirection(inputDirection)
	//{
	//}

	MoveCommand::MoveCommand(MovementComponent* pMovementComponent, const glm::vec2& inputDirection)
		: m_pMovementComponent(pMovementComponent)
		, m_InputDirection(inputDirection)
	{
	}

	void MoveCommand::Execute()
	{
		m_pMovementComponent->HandleInput(m_InputDirection);
	}

	MovementComponent::MovementComponent(dae::GameObject& go, GridComponent* grid, float speed, const glm::ivec2& startCell, bool canDig)
		: Component(go)
		, m_pGrid(grid)
		, m_Movespeed(speed)
		, m_CanDig(canDig)
	{
		GetOwner()->SetLocalPosition(glm::vec2(grid->GetCellPosition(startCell)));
	}

	void MovementComponent::HandleInput(const glm::vec2& input)
	{
		if (m_IsMoving) return;

		// Restrict to cardinal direction
		glm::vec2 dir(0.0f);
		if (fabs(input.x) > fabs(input.y))
			dir = glm::vec2((input.x > 0) ? 1.0f : -1.0f, 0.0f);
		else
			dir = glm::vec2(0.0f, (input.y > 0) ? 1.0f : -1.0f);
		if (dir == glm::vec2(0.0f)) return;

		// Compute current and next grid cells
		glm::ivec2 currentCell = m_pGrid->GetCell(GetOwner()->GetWorldPosition());
		glm::ivec2 nextCell = currentCell + glm::ivec2(dir);
		if (!m_pGrid->IsCellValid(nextCell) || m_pGrid->IsObstacle(nextCell)) return;

		if (m_CanDig)
		{
			//m_pGrid->Dig(nextCell); // dig the path if needed
			m_pGrid->DigTunnel(currentCell, dir);
			m_pGrid->DigTunnel(nextCell, dir);
		}
		// Begin smooth movement to the next cell
		m_TargetCell = nextCell;
		m_IsMoving = true;
		m_Velocity = dir * m_Movespeed;
	}

	void MovementComponent::Update()
	{
		if (!m_IsMoving) return;

		const glm::vec2& position = GetOwner()->GetLocalPosition();

		glm::vec2 targetPos = m_pGrid->GetCellPosition(m_TargetCell);
		glm::vec2 diff = targetPos - position;

		float distToTarget = glm::length(diff);
		float step = m_Movespeed * dae::Time::m_DeltaTime;

		if (step >= distToTarget) 
		{
			// Reached or overshot the target cell center
			GetOwner()->SetLocalPosition(targetPos);
			m_CurrentCell = m_TargetCell;
			m_IsMoving = false;
		}
		else 
		{
			// Move a fraction of the way
			glm::vec2 direction = diff / distToTarget; // normalize
			GetOwner()->SetLocalPosition(position + direction * step);
		}
	}

	bool MovementComponent::IsCellWalkable(const glm::ivec2& cell) const
	{
		return m_pGrid->IsCellValid(cell) && !m_pGrid->IsObstacle(cell);
	}

	void MoveState::OnEnter()
	{
		
	}

	void MoveState::OnExit()
	{
		
	}

	IState* MoveState::Update()
	{
		return this;
	}
}
