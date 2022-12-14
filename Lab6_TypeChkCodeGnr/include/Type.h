#ifndef __TYPE_H__
#define __TYPE_H__
#include <vector>
#include <string>
#include "SymbolTable.h"

class Type
{
private:
    int kind;
protected:
    bool constant;
    int size;
    enum {INT, VOID, FLOAT, FUNC, PTR, ARRAY, STRING};
public:
    Type(int kind, bool constant = false) : kind(kind), constant(constant) {};
    virtual ~Type() {};
    virtual std::string toStr() = 0;
    bool isConst() {return constant; }
    void setConst(bool value = true) {constant = value; }
    bool isInt() const {return kind == INT;};
    bool isFloat() const { return kind == FLOAT;};
    bool isVoid() const {return kind == VOID;};
    bool isFunc() const {return kind == FUNC;};
    bool isPtr() const { return kind == PTR; };
    bool isArray() const { return kind == ARRAY; };
    bool isIntFamily();
    bool isFloatFamily();
    int getSize() {return size; }
    int getKind() {return kind; }
    virtual Type *deepCopy() = 0;
    virtual bool sameType(Type *type) = 0;
};

class IntType : public Type
{
private:

public:
    IntType(int size, bool constant = false) : 
        Type(Type::INT, constant) {this->size = size; }
    std::string toStr();
    Type *deepCopy();
    bool sameType(Type *type);
};

class FloatType : public Type {
private:

public:
    FloatType(int size, bool constant = false) :
        Type(Type::FLOAT, constant) {this->size = size; }
    std::string toStr();
    Type *deepCopy();
    bool sameType(Type *type);
};

class VoidType : public Type
{
public:
    VoidType() : Type(Type::VOID){};
    std::string toStr();
    int getSize();
    Type *deepCopy();
    bool sameType(Type *type);
};

class FunctionType : public Type
{
private:
    Type *returnType;
    std::vector<Type *> paramsType;
    std::vector<SymbolEntry *> paramsSe;
public:
    FunctionType(Type *returnType, std::vector<Type *> paramsType, std::vector<SymbolEntry *> paramsSe) : 
        Type(Type::FUNC), returnType(returnType), paramsType(paramsType), paramsSe(paramsSe) {};
    Type *getRetType() {return returnType; };
    std::vector<Type *>& getParamsType() {return paramsType; };
    std::vector<SymbolEntry *>& getParamsSe() {return paramsSe; };
    std::string toStr();
    int getSize();
    Type *deepCopy();
    bool sameType(Type *type);
    bool sameParamsType(std::vector<Type *> cmpParamsType);
};

class ArrayType : public Type
{
private:
    Type *elementType, *parentType;
    int length;
public:
    ArrayType(Type *elementType, int length, bool constant = false);
    int getLength() {return length; }
    Type *getElementType() {return elementType; }
    void setParentType(Type *type) {parentType = type; }
    Type *getParentType() {return parentType; }
    std::string toStr();
    Type *deepCopy();
    void deepSetConst(bool value = true);
    bool sameType(Type *type);
};

class PointerType : public Type
{
private:
    Type *valueType;
public:
    PointerType(Type* valueType, bool constant = false);
    Type *getValueType() {return valueType; }
    std::string toStr();
    Type *deepCopy();
    bool sameType(Type *type);
};

class StringType : public Type {
private:
    int length;

public:
    StringType(int length) : Type(Type::STRING, true), length(length) {};
    int getLength() const { return length; };
    std::string toStr();
    Type *deepCopy();
    bool sameType(Type *type) {return true; }
};

class TypeSystem
{
private:
    static IntType commonInt;
    static IntType commonBool;
    static IntType commonInt8;
    static VoidType commonVoid;
    static FloatType commonFloat;
    static IntType commonConstInt;
    static IntType commonConstBool;
    static IntType commonConstInt8;
    static FloatType commonConstFloat;
public:
    static Type *intType;
    static Type *voidType;
    static Type *boolType;
    static Type* int8Type;
    static Type* floatType;
    static Type* constIntType;
    static Type* constBoolType;
    static Type* constInt8Type;
    static Type* constFloatType;
};

#endif
