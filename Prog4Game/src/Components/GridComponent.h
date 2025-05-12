#pragma once
#include <vector>

#include "Component.h"
#include "vec2.hpp"

namespace game
{
	class GridComponent : public dae::Component
	{
	public:

		class Cell
		{
		public:
			enum class State
			{
				Empty,
				Occupied,
				Blocked
			};

			State state{ State::Empty };
		private:
		};

		GridComponent(dae::GameObject& go, int rows, int columns, int cellSize);

		Cell::State GetCellState(int row, int column) const;
		Cell::State GetCellState(int index) const;

		void SetCellState(int row, int column, Cell::State state);
		void SetCellState(int index, Cell::State state);

		glm::vec2 GetCellPosition(int row, int column) const;
		glm::vec2 GetCellPosition(int index) const;

		glm::vec2 GetCellCenter(int row, int column) const;
		glm::vec2 GetCellCenter(int index) const;

		bool IsCellValid(int row, int col) const;
		bool IsCellValid(int index) const;

		glm::ivec2 GetCell(const glm::vec2& pos) const;

		int GetCellSize() const { return m_CellSize; }

		glm::vec2 GetGridExtend() const { return glm::vec2{ m_Rows * m_CellSize, m_Columns * m_CellSize}; }

		int GetColumns() const { return m_Columns; }
		int GetRows() const { return m_Rows; }


	private:

		void Render() const override;

		std::vector<Cell> m_Cells{};

		int m_Rows{};
		int m_Columns{};

		int m_CellSize{};

	};
}
