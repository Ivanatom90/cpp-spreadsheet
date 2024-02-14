#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <vector>

using Table = std::vector<std::vector<std::unique_ptr<Cell>>>;

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;


    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override; //константный и неконстантный геттеры, которые возвращают указатель на ячейку, расположенную по индексу pos. Если ячейка пуста, возвращают nullptr;
    Cell* Get_Cell(Position pos);
    const Cell* Get_Cell(Position pos) const;
    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами


private:
    Table cells_;
	// Можете дополнить ваш класс нужными полями и методами
};
