#include "GridComponent.h"

#include <stdexcept>

#include "GameObject.h"
#include "Renderer.h"
#include "SDL_render.h"

game::GridComponent::GridComponent(dae::GameObject& go, int rows, int columns, int cellSize)
	: dae::Component(go)
	, m_Rows(rows)
	, m_Columns(columns)
	, m_CellSize(cellSize)
{
	//m_Cells.resize(rows * columns);
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
		{
			int index = row * columns + column;
			m_Cells.push_back(Cell{this, index, m_CellSize });
		}
		GetOwner()->SetDepthIndex(1);
	}
}

game::GridComponent::Cell::State game::GridComponent::GetCellState(int row, int column) const
{
	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Columns)
	{
		throw std::out_of_range("Cell index out of range");
	}
	return m_Cells[row * m_Columns + column].state;
}

game::GridComponent::Cell::State game::GridComponent::GetCellState(int index) const
{
	if (index >= m_Cells.size())
	{
		throw std::out_of_range("Cell index out of range");
	}

	return m_Cells[index].state;
}

void game::GridComponent::SetCellState(int row, int column, Cell::State state)
{
	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Columns)
	{
		throw std::out_of_range("Cell index out of range");
	}
	m_Cells[row * m_Columns + column].state = state;
}

void game::GridComponent::SetCellState(int index, Cell::State state)
{
	if (index >= m_Cells.size())
	{
		throw std::out_of_range("Cell index out of range");
	}
	m_Cells[index].state = state;
}

glm::vec2 game::GridComponent::GetCellPosition(int row, int column) const
{
	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Columns)
	{
		throw std::out_of_range("Cell index out of range");
	}
	return glm::vec2{ column * m_CellSize, row * m_CellSize };
}

glm::vec2 game::GridComponent::GetCellPosition(int index) const
{
	if (index >= m_Cells.size())
	{
		throw std::out_of_range("Cell index out of range");
	}
	int row = index / m_Columns;
	int column = index % m_Columns;
	return glm::vec2{ column * m_CellSize, row * m_CellSize };
}

glm::vec2 game::GridComponent::GetCellCenter(const glm::ivec2& cell) const
{
	return GetCellCenter(cell.x, cell.y);
}

glm::vec2 game::GridComponent::GetCellCenter(int row, int column) const
{
	return glm::vec2{ row * m_CellSize + m_CellSize / 2, column * m_CellSize + m_CellSize / 2 };
}

glm::vec2 game::GridComponent::GetCellCenter(int index) const
{
	int row = index / m_Columns;
	int column = index % m_Columns;
	return glm::vec2{ row * m_CellSize + m_CellSize / 2, column * m_CellSize + m_CellSize / 2 };
}

bool game::GridComponent::IsCellValid(const glm::ivec2& cell) const
{
	return IsCellValid(cell.x, cell.y);
}

bool game::GridComponent::IsCellValid(int row, int col) const
{
	return row >= 0 && row < m_Rows
		&& col >= 0 && col < m_Columns;
}

bool game::GridComponent::IsCellValid(int index) const
{
	return index < m_Cells.size();
}

bool game::GridComponent::IsObstacle(const glm::ivec2& cell) const
{
	return IsCellValid(cell) && GetCellState(cell.x, cell.y) == Cell::State::Blocked;
}

bool game::GridComponent::IsDirt(const glm::ivec2& cell) const
{
	return IsCellValid(cell) && GetCellState(cell.x, cell.y) == Cell::State::Occupied;
}

void game::GridComponent::Dig(const glm::ivec2& cell)
{
	if (IsDirt(cell))
	{
		SetCellState(cell.x, cell.y, Cell::State::Empty);
	}
}

glm::ivec2 game::GridComponent::GetCell(const glm::vec2& pos) const
{
	return glm::ivec2{ static_cast<int>(pos.x) / m_CellSize, static_cast<int>(pos.y) / m_CellSize};
}

void game::GridComponent::Render() const
{
	for (int cell = 0; cell < static_cast<int>(m_Cells.size()); ++cell)
	{
		m_Cells[cell].Render();

		int row = cell / m_Columns;
		int column = cell % m_Columns;

		auto centerPos = glm::ivec2{ column * m_CellSize, row * m_CellSize } + m_CellSize / 2;

		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);

		// Draw line to the cell in the next column (right neighbor)
		if (column + 1 < m_Columns)
		{
			int rightNeighbor = cell + 1;
			if (rightNeighbor < m_Cells.size())
			{
				auto rightPos = glm::ivec2{ (column + 1) * m_CellSize, (row) * m_CellSize } + m_CellSize / 2;
				SDL_RenderDrawLine(dae::Renderer::GetInstance().GetSDLRenderer(),
					centerPos.x, centerPos.y,
					rightPos.x, rightPos.y);
			}
		}

		// Draw line to the cell in the next row (bottom neighbor)
		if (row + 1 < m_Rows)
		{
			int bottomNeighbor = cell + m_Columns;
			if (bottomNeighbor < m_Cells.size())
			{
				auto bottomPos = glm::ivec2{ column * m_CellSize, (row + 1) * m_CellSize } + m_CellSize / 2;
				SDL_RenderDrawLine(dae::Renderer::GetInstance().GetSDLRenderer(),
					centerPos.x, centerPos.y,
					bottomPos.x,bottomPos.y);
			}
		}

		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);

		SDL_Rect rect{ (column)*m_CellSize, (row)*m_CellSize , m_CellSize, m_CellSize };
		SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);

	}
}
