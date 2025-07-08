/**
 * This file contains the definitions for the code generation methods
 * for blocks of statements and the return statement.
*/

#include "semantics.hpp"

void StatementList::Generate(State* St) {
    for (auto statement : statements)
        statement->Generate(St);
}
