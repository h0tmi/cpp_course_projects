#pragma once

#include <string>
#include <vector>

class Minesweeper {
public:
    enum CellType {
        MINE,
        EMPTY,
    };
    enum CellStatus {
        OPENED,
        CLOSED,
    };
    enum CellMarkStatus {
        MARKED,
        UNMARKED,
    };
    struct Cell {
        size_t x = 0;
        size_t y = 0;
        bool operator==(const Cell& a) const {
            return (x == a.x && y == a.y);
        }
        bool operator<(const Cell& a) const {
            if (x == a.x) {
                return y < a.y;
            }
            return x < a.x;
        }
        bool operator>(const Cell& a) const {
            if (x == a.x) {
                return y < a.y;
            }
            return x > a.x;
        }
        void SwapCoordinates() {
            std::swap(x, y);
        }
        bool operator()(const Cell& a, const Cell& b) const {
            if (a.x == b.x) {
                return a.y < b.y;
            }
            return a.x < b.x;
        }
    };
    struct CellOptions {
        CellType cell_type = CellType::EMPTY;
        CellStatus cell_status = CellStatus::CLOSED;
        CellMarkStatus cell_mark_status = CellMarkStatus::UNMARKED;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    std::vector<std::vector<CellOptions>> field_;
    GameStatus game_status_;
    time_t game_time_ = 0;
    size_t remaining_empty_cells_cnt_;
    bool first_click_ = false;

    size_t MinesCounter(const Cell& cell) const;
    bool CellCoordinatesCheck(const Cell& cell) const;
};
