#pragma once
#include <string>
#include <vector>

#include "Component.h"
#include "Renderer.h"
#include "vec2.hpp"

namespace game
{
	class GridComponent : public dae::Component
	{
	public:

		class SubCell
		{
		public:
			enum class SubState
			{
				Empty,
				Terrain
			};

			SubCell(SubState startState = SubState::Empty)
				: m_SubState{ startState }
			{
			}

			SubState m_SubState{};
		};

		class Cell
		{
		public:
			enum class State
			{
				Empty,
				Dirt,
				Diamond,
				Bag,
			};

			Cell(GridComponent* pGrid, int cellIndex, int cellWidth)
				: m_CellWidth{ cellWidth }
				, cellIndex{ cellIndex }
				, m_pGrid{ pGrid }
			{
				const int subCellCount = 25; // 5x5 grid of subcells
				m_SubCellWidth = m_CellWidth / 5;

				for (int cell{}; cell < subCellCount; ++cell)
				{
					m_SubCells.emplace_back(SubCell{ SubCell::SubState::Terrain });
				}
			}

			void Render() const
			{
				int index{};
				for (const auto& subCell : m_SubCells)
				{
					SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255);

					if (subCell.m_SubState == SubCell::SubState::Terrain)
					{
						auto parentOffset = m_pGrid->GetCellPosition(cellIndex) + glm::vec2{2,2};
						int subCellWidth = m_SubCellWidth / 5 - 4;
						int subCellOffsetX = m_SubCellWidth * (index % 5);
						int subCellOffsetY = m_SubCellWidth * (index / 5);
						auto rect = SDL_Rect{ static_cast<int>(parentOffset.x) + subCellOffsetX, static_cast<int>(parentOffset.y) + subCellOffsetY, subCellWidth, subCellWidth };
						SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);
					}

					//auto rect = SDL_Rect{ 0, 0, static_cast<int>(m_CellWidth), static_cast<int>(m_CellWidth) };
					//SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rect);
					++index;
				}
			}

			std::vector<SubCell>& GetSubCells() { return m_SubCells; }

			void DigTunnel(const glm::vec2& direction);

			State state{ State::Dirt };
		private:
			std::vector<SubCell> m_SubCells{};

			int cellIndex{};
			GridComponent* m_pGrid{ nullptr };

			int m_CellWidth{};
			int m_SubCellWidth{};
		};

		GridComponent(dae::GameObject& go, int rows, int columns, int cellSize, const glm::vec2& offset = {0,0} );

		Cell::State GetCellState(int column, int row) const;
		Cell::State GetCellState(int index) const;

		void SetCellState(int column, int row, Cell::State state);
		void SetCellState(int index, Cell::State state);

		void SetSubCellStates(int column, int row, SubCell::SubState state);
		void SetSubCellStates(int index, SubCell::SubState state);

		glm::vec2 GetCellPosition(const glm::ivec2& cell) const;
		glm::vec2 GetCellPosition(int column, int row) const;
		glm::vec2 GetCellPosition(int index) const;

		glm::vec2 GetCellCenter(const glm::ivec2& cell) const;
		glm::vec2 GetCellCenter(int column, int row) const;
		glm::vec2 GetCellCenter(int index) const;

		bool IsCellValid(const glm::ivec2& cell) const;
		bool IsCellValid(int column, int row) const;
		bool IsCellValid(int index) const;

		bool IsObstacle(const glm::ivec2& cell) const;

		bool IsDirt(const glm::ivec2& cell) const;
		void Dig(const glm::ivec2& cell);

		glm::ivec2 GetCell(const glm::vec2& pos) const;

		int GetCellSize() const { return m_CellSize; }

		glm::vec2 GetGridExtend() const { return glm::vec2{ m_Columns * m_CellSize, m_Rows * m_CellSize }; }

		int GetColumns() const { return m_Columns; }
		int GetRows() const { return m_Rows; }

		void DigTunnel(const glm::ivec2& cell, const glm::vec2& direction);
		bool LoadFromCSV(const std::string& filePath);

	private:

		void Render() const override;

		std::vector<Cell> m_Cells{};

		glm::vec2 m_Offset{};

		int m_Rows{};
		int m_Columns{};

		int m_CellSize{};

	};
}
