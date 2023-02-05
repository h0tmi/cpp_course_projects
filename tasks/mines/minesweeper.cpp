#include "minesweeper.h"
#include <algorithm>
#include <map>
#include <random>
#include <queue>
#include <time.h>
#include <utility>

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    std::swap(width, height);
    remaining_empty_cells_cnt_ = width * height - mines_count;
    game_status_ = GameStatus::NOT_STARTED;
    field_.resize(width, std::vector<CellOptions>(height));
    std::vector<Minesweeper::Cell> mines_coordinates;
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            mines_coordinates.push_back(Minesweeper::Cell{x, y});
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mines_coordinates.begin(), mines_coordinates.end(), g);
    for (size_t i = 0; i < mines_count; ++i) {
        field_[mines_coordinates[i].x][mines_coordinates[i].y].cell_type = CellType::MINE;
    }
}
Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    std::swap(width, height);
    remaining_empty_cells_cnt_ = width * height - cells_with_mines.size();
    game_status_ = GameStatus::NOT_STARTED;
    field_.resize(width, std::vector<CellOptions>(height));
    for (const auto& [x, y] : cells_with_mines) {
        field_[y][x].cell_type = CellType::MINE;
    }
}
void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    std::swap(width, height);
    first_click_ = false;
    remaining_empty_cells_cnt_ = width * height - mines_count;
    game_status_ = GameStatus::NOT_STARTED;
    field_.clear();
    field_.resize(width, std::vector<CellOptions>(height, CellOptions()));
    std::vector<Minesweeper::Cell> mines_coordinates;
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            mines_coordinates.push_back(Minesweeper::Cell{x, y});
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mines_coordinates.begin(), mines_coordinates.end(), g);
    for (size_t i = 0; i < mines_count; ++i) {
        field_[mines_coordinates[i].x][mines_coordinates[i].y].cell_type = CellType::MINE;
    }
}
void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    std::swap(width, height);
    first_click_ = false;
    remaining_empty_cells_cnt_ = width * height - cells_with_mines.size();
    game_status_ = GameStatus::NOT_STARTED;
    field_.clear();
    field_.resize(width, std::vector<CellOptions>(height, CellOptions()));
    for (auto [x, y] : cells_with_mines) {
        field_[y][x].cell_type = CellType::MINE;
    }
}
void Minesweeper::OpenCell(const Minesweeper::Cell& cell) {
    if (!first_click_) {
        game_time_ = time(nullptr);
        first_click_ = true;
    }
    Minesweeper::Cell swapped_cell = cell;
    swapped_cell.SwapCoordinates();
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY ||
        field_[swapped_cell.x][swapped_cell.y].cell_mark_status == CellMarkStatus::MARKED ||
        field_[swapped_cell.x][swapped_cell.y].cell_status == CellStatus::OPENED) {
        return;
    }
    if (field_[swapped_cell.x][swapped_cell.y].cell_type == CellType::MINE) {
        game_status_ = GameStatus::DEFEAT;
        for (size_t x = 0; x < field_.size(); ++x) {
            for (size_t y = 0; y < field_[x].size(); ++y) {
                field_[x][y].cell_status = CellStatus::OPENED;
                field_[x][y].cell_mark_status = CellMarkStatus::UNMARKED;
            }
        }
        return;
    }
    if (MinesCounter(swapped_cell) == 0) {
        std::vector<std::pair<int8_t, int8_t>> move_directions = {
            {-1, -1}, {0, -1}, {1, 1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, -1},
        };
        std::queue<Minesweeper::Cell> remaining_cells_to_check;
        std::map<Minesweeper::Cell, bool, Minesweeper::Cell> visited_cells;
        visited_cells[swapped_cell] = true;
        remaining_cells_to_check.push(swapped_cell);
        while (!remaining_cells_to_check.empty()) {
            Minesweeper::Cell current_cell = remaining_cells_to_check.front();
            remaining_cells_to_check.pop();

            field_[current_cell.x][current_cell.y].cell_status = CellStatus::OPENED;
            --remaining_empty_cells_cnt_;
            if (remaining_empty_cells_cnt_ == 0) {
                game_status_ = GameStatus::VICTORY;
            }
            size_t mines = MinesCounter(current_cell);
            if (mines != 0) {
                continue;
            }
            for (const auto& [x, y] : move_directions) {
                Minesweeper::Cell edited_cell = Minesweeper::Cell{current_cell.x + x, current_cell.y + y};
                if (CellCoordinatesCheck(edited_cell) &&
                    field_[edited_cell.x][edited_cell.y].cell_mark_status != CellMarkStatus::MARKED &&
                    visited_cells.find(edited_cell) == visited_cells.end()) {
                    visited_cells[edited_cell] = true;
                    remaining_cells_to_check.push(edited_cell);
                }
            }
        }
        return;
    }
    field_[swapped_cell.x][swapped_cell.y].cell_status = CellStatus::OPENED;
    --remaining_empty_cells_cnt_;
    if (remaining_empty_cells_cnt_ == 0) {
        game_status_ = GameStatus::VICTORY;
    }
}
void Minesweeper::MarkCell(const Minesweeper::Cell& cell) {
    Minesweeper::Cell swapped_cell = cell;
    swapped_cell.SwapCoordinates();
    if (field_[swapped_cell.x][swapped_cell.y].cell_status == CellStatus::OPENED ||
        game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    switch (field_[swapped_cell.x][swapped_cell.y].cell_mark_status) {
        case CellMarkStatus::MARKED:
            field_[swapped_cell.x][swapped_cell.y].cell_mark_status = CellMarkStatus::UNMARKED;
            break;
        case CellMarkStatus::UNMARKED:
            field_[swapped_cell.x][swapped_cell.y].cell_mark_status = CellMarkStatus::MARKED;
    }
}
Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}
time_t Minesweeper::GetGameTime() const {
    if (game_status_ == GameStatus::NOT_STARTED) {
        return 0;
    }
    return game_time_ - time(nullptr);
}
Minesweeper::RenderedField Minesweeper::RenderField() const {
    Minesweeper::RenderedField rendered_field(field_.size());
    for (size_t x = 0; x < field_.size(); ++x) {
        for (size_t y = 0; y < field_[x].size(); ++y) {
            if (field_[x][y].cell_mark_status == CellMarkStatus::MARKED) {
                rendered_field[x].push_back('?');
            } else if (field_[x][y].cell_status == CellStatus::CLOSED) {
                rendered_field[x].push_back('-');
            } else if (field_[x][y].cell_status == CellStatus::OPENED) {
                switch (field_[x][y].cell_type) {
                    case CellType::MINE:
                        rendered_field[x].push_back('*');
                        break;
                    case CellType::EMPTY:
                        size_t mines_cnt = MinesCounter(Minesweeper::Cell{x, y});
                        if (mines_cnt == 0) {
                            rendered_field[x].push_back('.');
                        } else {
                            rendered_field[x].push_back(static_cast<char>(mines_cnt + '0'));
                        }
                        break;
                }
            }
        }
    }
    return rendered_field;
}
size_t Minesweeper::MinesCounter(const Minesweeper::Cell& cell) const {
    size_t mines_cnt = 0;
    std::vector<std::pair<int8_t, int8_t>> move_directions = {
        {-1, -1}, {0, -1}, {1, 1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, -1},
    };
    for (const auto& [x, y] : move_directions) {
        Minesweeper::Cell edited_cell = Minesweeper::Cell{cell.x + x, cell.y + y};
        if (CellCoordinatesCheck(edited_cell) && field_[edited_cell.x][edited_cell.y].cell_type == CellType::MINE) {
            ++mines_cnt;
        }
    }
    return mines_cnt;
}
bool Minesweeper::CellCoordinatesCheck(const Minesweeper::Cell& cell) const {
    return cell.x != std::numeric_limits<size_t>::max() && cell.x < field_.size() &&
           cell.y != std::numeric_limits<size_t>::max() && cell.y < field_.back().size();
}
