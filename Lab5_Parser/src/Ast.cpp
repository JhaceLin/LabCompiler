#include "Ast.h"
#include "SymbolTable.h"
#include <string>
#include "Type.h"

extern FILE *yyout;
int Node::counter = 0;

Node::Node()
{
    seq = counter++;
}

void Ast::output()
{
    fprintf(yyout, "program\n");
    if(root != nullptr)
        root->output(4);
}

void BinaryExpr::output(int level)
{
    std::string op_str;
    switch(op)
    {
        case ADD:
            op_str = "add";
            break;
        case SUB:
            op_str = "sub";
            break;
        case AND:
            op_str = "and";
            break;
        case OR:
            op_str = "or";
            break;
        case LESS:
            op_str = "less";
            break;
    }
    if (isConst) {
        fprintf(yyout, "%*cConstBinaryExpr\top: %s\n", level, ' ', op_str.c_str());
    } else {
        fprintf(yyout, "%*cBinaryExpr\top: %s\n", level, ' ', op_str.c_str());
    }
    expr1->output(level + 4);
    expr2->output(level + 4);
}

void Constant::output(int level)
{
    std::string type, value;
    type = symbolEntry->getType()->toStr();
    value = symbolEntry->toStr();
    fprintf(yyout, "%*cIntegerLiteral\tvalue: %s\ttype: %s\n", level, ' ',
            value.c_str(), type.c_str());
}

void ArrayIndexNode::append(ExprNode *next)
{
    exprList.push_back(next);
}

int ArrayIndexNode::getDemension()
{
    return exprList.size();
}

void ArrayIndexNode::output(int level)
{
    if (isConst) {
        fprintf(yyout, "%*cConstArrayIndexNode:\n", level, ' ');
    } else {
        fprintf(yyout, "%*cArrayIndexNode:\n", level, ' ');
    }
    for(std::vector<ExprNode *>::iterator iter = exprList.begin(); 
    iter != exprList.end(); iter++) {
        (*iter)->output(level + 4);
    }
}

bool Id::isArray()
{
    return symbolEntry->getType()->isArray();
}

void Id::output(int level)
{
    std::string name, type;
    int scope;
    name = symbolEntry->toStr();
    type = symbolEntry->getType()->toStr();
    scope = dynamic_cast<IdentifierSymbolEntry*>(symbolEntry)->getScope();
    std::string constStr = isConst ? "Const " : "";
    fprintf(yyout, "%*c%sId\tname: %s\tscope: %d\ttype: %s\n", level, ' ',
        constStr.c_str(), name.c_str(), scope, type.c_str());
    if (isArray() && index != nullptr) {
        index->output(level + 4);
    }
}

void CompoundStmt::output(int level)
{
    fprintf(yyout, "%*cCompoundStmt\n", level, ' ');
    if (stmt == nullptr) {
        fprintf(yyout, "%*cNullStmt\n", level + 4, ' ');
    } else {
        stmt->output(level + 4);
    }
}

void SeqNode::output(int level)
{
    fprintf(yyout, "%*cSequence\n", level, ' ');
    stmt1->output(level + 4);
    stmt2->output(level + 4);
}

void InitValNode::append(InitValNode *next)
{
    initValList.push_back(next);
}

void InitValNode::output(int level)
{
    if (isConst) {
        fprintf(yyout, "%*cConstInitValNode\n", level, ' ');
    } else {
        fprintf(yyout, "%*cInitValNode\n", level, ' ');
    }
    for (std::vector<InitValNode *>::iterator iter = initValList.begin(); 
    iter != initValList.end(); iter++) {
        (*iter)->output(level + 4);
    }
    if (expr != nullptr) {
        expr->output(level + 4);
    }
}

bool DefNode::isArray()
{
    return id->isArray();
}

void DefNode::output(int level)
{
    std::string constStr = isConst ? "Const " : "";
    std::string arrayStr = isArray() ? "Array " : "";
    fprintf(yyout, "%*cDefNode - %sInt %s\n", level, ' ', 
        constStr.c_str(), arrayStr.c_str());
    id->output(level + 4);
    if (initVal != nullptr) {
        initVal->output(level + 4);
    } else {
        fprintf(yyout, "%*cNo initial value\n", level + 4, ' ');
    }
}

void DeclStmt::append(DefNode *next)
{
    varDefList.push_back(next);
}

void DeclStmt::output(int level)
{
    fprintf(yyout, "%*cDeclStmt\n", level, ' ');
    for (std::vector<DefNode *>::iterator iter = varDefList.begin(); 
    iter != varDefList.end(); iter++) {
        (*iter)->output(level + 4);
    }
}

void IfStmt::output(int level)
{
    fprintf(yyout, "%*cIfStmt\n", level, ' ');
    cond->output(level + 4);
    thenStmt->output(level + 4);
}

void IfElseStmt::output(int level)
{
    fprintf(yyout, "%*cIfElseStmt\n", level, ' ');
    cond->output(level + 4);
    thenStmt->output(level + 4);
    elseStmt->output(level + 4);
}

void ReturnStmt::output(int level)
{
    fprintf(yyout, "%*cReturnStmt\n", level, ' ');
    retValue->output(level + 4);
}

void WhileStmt::output(int level)
{
    fprintf(yyout, "%*cWhileStmt\n", level, ' ');
    cond->output(level + 4);
    loopStmt->output(level + 4);
}

void BreakStmt::output(int level)
{
    fprintf(yyout, "%*cBreakStmt\n", level, ' ');
}

void ContinueStmt::output(int level)
{
    fprintf(yyout, "%*cContinueStmt\n", level, ' ');
}

void AssignStmt::output(int level)
{
    fprintf(yyout, "%*cAssignStmt\n", level, ' ');
    lval->output(level + 4);
    expr->output(level + 4);
}

void FunctionDef::output(int level)
{
    std::string name, type;
    name = se->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "%*cFunctionDefine function name: %s, type: %s\n", level, ' ', 
            name.c_str(), type.c_str());
    stmt->output(level + 4);
}
