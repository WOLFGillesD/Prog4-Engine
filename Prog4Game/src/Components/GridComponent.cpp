#include "GridComponent.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "GameObject.h"
#include "Renderer.h"
#include "SDL_render.h"

game::GridComponent::GridComponent(dae::GameObject& go, int rows, int columns, int cellSize, const glm::vec2& offset)
	: dae::Component(go)
	, m_Rows(rows)
	, m_Columns(columns)
	, m_CellSize(cellSize)
	, m_Offset(offset)
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

game::GridComponent::Cell::State game::GridComponent::GetCellState(int column, int row) const
{
	if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
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

void game::GridComponent::SetCellState(int column, int row, Cell::State state)
{
	if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
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

glm::vec2 game::GridComponent::GetCellPosition(const glm::ivec2& cell) const
{
	if (!IsCellValid(cell))
	{
		throw std::out_of_range("Cell index out of range");
	}
	return glm::vec2{ m_Offset.x + cell.x * m_CellSize, m_Offset.y + cell.y * m_CellSize };
}

glm::vec2 game::GridComponent::GetCellPosition(int column, int row) const
{
	if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
	{
		throw std::out_of_range("Cell index out of range");
	}
	return glm::vec2{ m_Offset.x + column * m_CellSize, m_Offset.y + row * m_CellSize };
}

glm::vec2 game::GridComponent::GetCellPosition(int index) const
{
	if (index >= m_Cells.size())
	{
		throw std::out_of_range("Cell index out of range");
	}
	int row = index / m_Columns;
	int column = index % m_Columns;
	return glm::vec2{ m_Offset.x + column * m_CellSize, m_Offset.y + row * m_CellSize };
}

glm::vec2 game::GridComponent::GetCellCenter(const glm::ivec2& cell) const
{
	return GetCellCenter(cell.x, cell.y);
}

glm::vec2 game::GridComponent::GetCellCenter(int column, int row) const
{
	return glm::vec2{ m_Offset.x + (column * m_CellSize + m_CellSize / 2), m_Offset.y + (row * m_CellSize + m_CellSize / 2) };
}

glm::vec2 game::GridComponent::GetCellCenter(int index) const
{
	int row = index / m_Columns;
	int column = index % m_Columns;
	return glm::vec2{ m_Offset.x + (row * m_CellSize + m_CellSize / 2),m_Offset.y+ (column * m_CellSize + m_CellSize / 2) };
}

bool game::GridComponent::IsCellValid(const glm::ivec2& cell) const
{
	return IsCellValid(cell.x, cell.y);
}

bool game::GridComponent::IsCellValid(int column, int row) const
{
	return column >= 0 && column < m_Columns
		&& row >= 0 && row < m_Rows;
}

bool game::GridComponent::IsCellValid(int index) const
{
	return index < m_Cells.size();
}

bool game::GridComponent::IsObstacle(const glm::ivec2& cell) const
{
	return IsCellValid(cell) && GetCellState(cell.x, cell.y) == Cell::State::Bag;
}

bool game::GridComponent::IsDirt(const glm::ivec2& cell) const
{
	return IsCellValid(cell) && GetCellState(cell.x, cell.y) == Cell::State::Dirt;
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
	return glm::ivec2{ static_cast<int>(pos.x - m_Offset.x) / m_CellSize, static_cast<int>(pos.y - m_Offset.y) / m_CellSize};
}

// Adds to Cell class in GridComponent.h
void game::GridComponent::Cell::DigTunnel(const glm::vec2& direction)
{
	const int width = 5;
	for (int i = 0; i < width * width; ++i)
	{
		int row = i / width;
		int col = i % width;
		bool dig = false;

		if (direction.x != 0 && row >= 1 && row <= 3) // horizontal: dig middle 3 rows
			dig = true;
		if (direction.y != 0 && col >= 1 && col <= 3) // vertical: dig middle 3 cols
			dig = true;

		if (dig)
			m_SubCells[i].m_SubState = SubCell::SubState::Empty;
	}
}

// Adds to GridComponent.cpp (outside class definition)
void game::GridComponent::DigTunnel(const glm::ivec2& cell, const glm::vec2& direction)
{
	if (!IsCellValid(cell)) return;
	int cellIndex{ cell.y * m_Columns + cell.x };
	m_Cells[cellIndex].DigTunnel(direction);
	m_Cells[cellIndex].state = Cell::State::Empty;
}

bool game::GridComponent::LoadFromCSV(const std::string& csvData)
{
	std::stringstream ss(csvData);
	std::string line;
	int currentRow{};

	while (std::getline(ss, line))
	{
		if (line.empty())
			continue;

		std::stringstream lineStream(line);
		std::string cellStr;
		int currentColumn{};

		while (std::getline(lineStream, cellStr, ';'))
		{
			if (currentRow >= m_Rows || currentColumn >= m_Columns)
			{
				std::cout << "CSV data exceeds grid size at row " << currentRow << " column " << currentColumn << "\n";
				return false; // or just break or ignore excess
			}

			int cellValue = std::stoi(cellStr);
			Cell::State state = Cell::State::Empty;

			state = static_cast<Cell::State>(cellValue);

			SetCellState(currentColumn, currentRow, state);
			++currentColumn;
		}

		if (currentColumn != m_Columns)
		{
			std::cout << "Warning: CSV row " << currentRow << " column count (" << currentColumn << ") does not match grid columns (" << m_Columns << ")\n";
		}

		++currentRow;
	}

	if (currentRow != m_Rows)
	{
		std::cout << "Warning: CSV row count (" << currentRow << ") does not match grid rows (" << m_Rows << ")\n";
	}

	return true;
}

void game::GridComponent::Render() const
{
	for (int cell = 0; cell < static_cast<int>(m_Cells.size()); ++cell)
	{
		m_Cells[cell].Render();

		int row = cell / m_Columns;
		int column = cell % m_Columns;

		auto centerPos = glm::ivec2{ m_Offset.x + column * m_CellSize, m_Offset.y + row * m_CellSize } + m_CellSize / 2;

		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);

		// Draw line to the cell in the next column (right neighbor)
		if (column + 1 < m_Columns)
		{
			int rightNeighbor = cell + 1;
			if (rightNeighbor < m_Cells.size())
			{
				auto rightPos = glm::ivec2{ m_Offset.x + (column + 1) * m_CellSize, m_Offset.y + (row) * m_CellSize } + m_CellSize / 2;
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
				auto bottomPos = glm::ivec2{ m_Offset.x + column * m_CellSize, m_Offset.y + (row + 1) * m_CellSize } + m_CellSize / 2;
				SDL_RenderDrawLine(dae::Renderer::GetInstance().GetSDLRenderer(),
					centerPos.x, centerPos.y,
					bottomPos.x,bottomPos.y);
			}
		}

		SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);

		SDL_Rect rect{ static_cast<int>(m_Offset.x) + (column)*m_CellSize, static_cast<int>(m_Offset.y) + (row)*m_CellSize , m_CellSize, m_CellSize };
		SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);

	}
}
