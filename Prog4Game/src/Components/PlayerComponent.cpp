#include "PlayerComponent.h"

#include "DaeTime.h"

namespace game
{
	void IdleState::OnEnter()
	{
		const auto cell = player.GetCell();
		const glm::vec2 center = player.GetGrid()->GetCellCenter(cell.x, cell.y);
		player.GetOwner()->SetLocalPosition(center.x, center.y);
	}

	void IdleState::Update()
	{
		auto cell = player.GetCell();
		const auto& grid = *player.GetGrid();

		if (player.IsMovingUp() && grid.IsCellValid(cell.x - 1, cell.y))    player.ChangeState(&player.moveUp);
		else if (player.IsMovingDown() && grid.IsCellValid(cell.x + 1, cell.y))  player.ChangeState(&player.moveDown);
		else if (player.IsMovingLeft() && grid.IsCellValid(cell.x, cell.y - 1))  player.ChangeState(&player.moveLeft);
		else if (player.IsMovingRight() && grid.IsCellValid(cell.x, cell.y + 1)) player.ChangeState(&player.moveRight);
	}

	void PlayerComponent::ChangeState(IState* newState)
	{
		if (currentState != nullptr) currentState->OnExit();
		currentState = newState;
		currentState->OnEnter();
	}

	void PlayerComponent::Update()
	{
		//currentState->Update();
	}

	MoveCommand::MoveCommand(PlayerComponent* pActor, const glm::vec2& inputDirection)
		: m_pActor(pActor), m_InputDirection(inputDirection)
	{
	}

	void MoveCommand::Execute()
	{
		m_pActor->m_MovingDirection = m_InputDirection;
		m_pActor->currentState->Update();

	}

	void MoveState::OnEnter()
	{
		startCell = player.GetCell();
		// if next cell is invalid, abort move
		//if (!player.GetGrid()->IsCellValid(startCell.x + dy, startCell.y + dx))
		//{
		//	player.ChangeState(&player.idleState);
		//}
	}

	void MoveState::OnExit()
	{
		
	}

	void MoveState::Update()
	{
		if ((dx != 0 && ((dx > 0 && player.IsMovingLeft()) || (dx < 0 && player.IsMovingRight()))) ||
			(dy != 0 && ((dy > 0 && player.IsMovingUp()) || (dy < 0 && player.IsMovingDown()))))
		{
			if (dx > 0) 
				player.ChangeState(&player.moveLeft);
			else if (dx < 0) 
				player.ChangeState(&player.moveRight);
			else if (dy > 0) 
				player.ChangeState(&player.moveUp);
			else           
				player.ChangeState(&player.moveDown);
			return;
		}

		
		glm::vec2 targetCenter = player.GetGrid()->GetCellCenter(startCell.x + dx, startCell.y + dy);
		glm::vec2 pos = glm::vec2{ player.GetOwner()->GetLocalPosition().x, player.GetOwner()->GetLocalPosition().y };

		// Move toward that center
		const float dt = dae::Time::m_DeltaTime;

		pos.x = pos.x + dx * player.GetSpeed() * dt;
		pos.y = pos.y + dy * player.GetSpeed() * dt;

		pos.x = glm::clamp(pos.x, static_cast<float>(player.GetGrid()->GetCellSize()) / 2.f, player.GetGrid()->GetGridExtend().x - static_cast<float>(player.GetGrid()->GetCellSize()) / 2.f);
		pos.y = glm::clamp(pos.y, static_cast<float>(player.GetGrid()->GetCellSize()) / 2.f, player.GetGrid()->GetGridExtend().y - static_cast<float>(player.GetGrid()->GetCellSize()) / 2.f);

		player.GetOwner()->SetLocalPosition(pos.x, pos.y);
		// TODO: Fix bug with case on the edges. TargetCenter is out of bounds and can never be reached.

		//targetCenter.x = glm::clamp(targetCenter.x, player.GetGrid()->GetCellCenter(0).x, player.GetGrid()->GetCellCenter(player.GetGrid()->GetColumns() - 1).x );
		//targetCenter.y = glm::clamp(targetCenter.y, player.GetGrid()->GetCellCenter(0).y, player.GetGrid()->GetCellCenter(player.GetGrid()->GetRows() - 1).y );

		const float eps = 1.f;
		if (glm::length(pos - targetCenter) <= eps)
		{
			pos = targetCenter;
			player.GetOwner()->SetLocalPosition(pos.x, pos.y);
			player.SetCell(startCell.x + dx, startCell.y + dy);

			if (dx != 0)
			{
				if (player.IsMovingUp()) { player.ChangeState(&player.moveUp);    return; }
				else if (player.IsMovingDown()) { player.ChangeState(&player.moveDown);  return; }
			}
			else if (dy != 0)
			{
				if (player.IsMovingLeft()) { player.ChangeState(&player.moveLeft);  return; }
				else if (player.IsMovingRight()) { player.ChangeState(&player.moveRight); return; }
			}

			player.ChangeState(&player.idleState);
		}

	}
}

//namespace game
//{
	//void MovingState::OnEnter(SMData* const pData)
	//{
	//	// Set initial target position
	//	int row, column;
	//	glm::vec2 direction;
	//	pData->GetData("Row", row);
	//	pData->GetData("Column", column);
	//	pData->GetData("Direction", direction);

	//	row += static_cast<int>(direction.y);
	//	if (row == static_cast<int>(direction.y))
	//		column += static_cast<int>(direction.x);

	//	pData->ChangeData("TargetRow", row);
	//	pData->ChangeData("TargetColumn", column);
	//}

	//void MovingState::Update(SMData* const pData)
	//{
	//	int row, column, targetRow, targetColumn;
	//	float speed;
	//	glm::vec2 direction;
	//	PlayerComponent* pActor;

	//	pData->GetData("Actor", pActor);
	//	pData->GetData("Row", row);
	//	pData->GetData("Column", column);
	//	pData->GetData("TargetRow", targetRow);
	//	pData->GetData("TargetColumn", targetColumn);
	//	pData->GetData("Direction", direction);
	//	pData->GetData("Speed", speed);

	//	//assert(pActor != nullptr);

	//	//auto currentPos = m_pGrid->GetCellPosition(row, column);
	//	//auto targetPos = m_pGrid->GetCellPosition(targetRow, targetColumn);

	//	//// Interpolate position
	//	//auto newPos = glm::mix(currentPos, targetPos, speed * dae::Time::m_DeltaTime);
	//	//GetOwner()->SetLocalPosition(newPos.x, newPos.y);

	//	//// Check if target reached
	//	//if (glm::distance(newPos, targetPos) < 0.1f)
	//	//{
	//	//	pData->ChangeData("Row", targetRow);
	//	//	pData->ChangeData("Column", targetColumn);
	//	//	pData->ChangeData("Direction", glm::vec2(0, 0)); // Stop moving
	//	//}
	//}

	//void IdleState::OnEnter(SMData* const pData)
	//{
	//	// Reset direction
	//	pData->ChangeData("Direction", glm::vec2(0, 0));
	//}

	//void IdleState::Update(SMData* const pData)
	//{
	//	glm::vec2 direction;
	//	pData->GetData("Direction", direction);
	//}

	//void IdleState::OnExit(SMData* const pData)
	//{
	//	// Cleanup or reset state if needed
	//}

	//PlayerComponent::PlayerComponent(dae::GameObject& go, GridComponent* pGrid, int row, int column)
	//	: dae::Component(go), m_Row(row), m_Column(column), m_pGrid(pGrid)
	//{
	//	auto pos = m_pGrid->GetCellPosition(row, column);
	//	m_pGrid->GetOwner()->SetLocalPosition(pos.x, pos.y);

	//	auto smData = CreateSMData();
	//	m_movementState = std::make_unique<StateMachine>(m_idleState.get(), std::move(smData));
	//	m_movementState->AddTransition(m_idleState.get(), m_movingState.get(), m_reverseCondition.get());
	//	m_movementState->AddTransition(m_movingState.get(), m_idleState.get(), m_reverseCondition.get()); 
	//}

	//std::unique_ptr<SMData> PlayerComponent::CreateSMData() const
	//{
	//	auto pData = std::make_unique<SMData>();
	//	pData->AddData("Actor", this);
	//	pData->AddData("Row", m_Row);
	//	pData->AddData("Column", m_Column);

	//	pData->AddData("InputDirection", glm::vec2(0, 0));
	//	pData->AddData("TargetCell", glm::ivec2(0, 0));

	//	return std::move(pData);
	//}

	//MoveCommand::MoveCommand(PlayerComponent& pActor, const glm::vec2& inputDirection, float speed)
	//	: m_pActor(&pActor)
	//	, m_InputDirection(inputDirection)
	//	, m_Speed(speed)
	//{
	//	
	//}

	//void MoveCommand::Execute()
	//{
	//	auto pData = m_pActor->GetSMData();

	//	if (pData == nullptr)
	//		return;

	//	pData->ChangeData("InputDirection", m_InputDirection);
	//	pData->ChangeData("Speed", m_Speed);
	//}

	//void MoveCommand::SetInput(const glm::vec2& v2)
	//{
	//	m_InputDirection = v2;
	//}

//}
