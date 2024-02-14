#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>


using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#ARITHM!";
}

namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:
    explicit Formula(std::string expression): ast_(ParseFormulaAST(expression)){}
    Value Evaluate(const SheetInterface& sheet) const override {
        try {
            std::function<double(Position)> args = [&sheet](const Position pos)->double {

                if (!pos.IsValid()) {
                    throw FormulaError(FormulaError::Category::Ref);
                }

                    const auto* cell = sheet.GetCell(pos);
                    if (!cell) {
                        return 0.0;
                    }

                        if (std::holds_alternative<double>(cell->GetValue())) {
                            return std::get<double>(cell->GetValue());

                        } else if (std::holds_alternative<std::string>(cell->GetValue())) {

                            auto str_value = std::get<std::string>(cell->GetValue());
                            if (str_value != "") {
                                bool IsNum = true;
                                for(char ch:str_value){
                                    if((ch<'0' || ch>'9') && ch != '.'){
                                        IsNum = false;
                                    }

                                }
                                double num = std::stod(str_value);

                                if (IsNum) {
                                    return num;
                                } else {
                                    throw FormulaError(FormulaError::Category::Value);
                                }

                            } else {
                                return 0.0;
                            }

                        } else {
                            throw FormulaError(std::get<FormulaError>(cell->GetValue()));
                        }



            };

            return ast_.Execute(args);

        } catch (const FormulaError& evaluate_error) {
            return evaluate_error;
        }
    }

    std::string GetExpression() const override {
        std::ostringstream out;
        ast_.PrintFormula(out);
        return out.str();
    }

    std::vector<Position> GetReferencedCells() const override {
        std::vector<Position> cells;
        if(ast_.GetCells().empty()){
            return cells;
        }
        for (const auto& cell : ast_.GetCells()) {

            if (cell.IsValid()) {
                bool repeat = std::find(cells.begin(), cells.end(), cell) != cells.end();
                if(!repeat){
                   cells.push_back(cell);
                }
            }
        }
        return cells;
    }

private:
    FormulaAST ast_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    try{

        return std::make_unique<Formula>(std::move(expression));

    }  catch(...){
             throw FormulaException("unknoun formula");
        }
    }






