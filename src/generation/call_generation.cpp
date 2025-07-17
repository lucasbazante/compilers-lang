/**
 * This file contains the definitions for the code generation methods
 * for call statements, including those regarding the STD library
 * of the language.
*/

#include "semantics.hpp"
#include "array"
#include <algorithm>

constexpr std::array std_ = {
    "readint",
    "readfloat",
    "readchar",
    "readstr",
    "readline",
    "printint",
    "printfloat",
    "printstr",
    "printline"
};

void Call::Generate(State* St) {
    if (std::find(std_.begin(), std_.end(), this->f_name) != std_.end())
        this->Generate_Std(St);
    else {
        if (exp_list != nullptr) {
            for (auto exp : exp_list->exp_list)
                exp->Generate(St), this->exp_list_repr.push_back(exp->Repr());

            St->Emit_Call_Params(this->f_name, this->f_symbol, this->exp_list_repr);
        }

        St->Emit_Call(this->f_name);
    }
}

void Call::Generate_Std(State* St) {
    if (not this->exp_list->exp_list.empty()) {
        this->exp_list->exp_list[0]->Generate(St);
        St->Emit(
            this->f_name +
            "(" +
            this->exp_list->exp_list[0]->Repr() +
            ");"
        );
    } else {
        St->Emit(this->f_name + "();");
    }
}

void Call::Internal_Generation(State* St) {
    if (std::find(std_.begin(), std_.end(), this->f_name) != std_.end())
        this->Internal_Std_Generation(St);
    else {
        this->Generate(St);
        this->Set_Repr("_" + this->f_name + "_return");
    }
}

void Call::Internal_Std_Generation(State* St) {
    this->Set_Repr( this->f_name + "()");
}
