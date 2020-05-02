#pragma once

#include <vector>

#include <data_type.hpp>
#include <token.hpp>

enum StmtType {
	S_STRUCT,
	S_FUNC_DECL,
	S_VAR_DECL,
	S_IF,
	S_EXPR_STMT,
};

struct Expr;

struct Stmt {
	StmtType type;
	union {
		struct {
			Token* identifier;
			Stmt** fields; // fields is a vector of Stmts
		} struct_stmt;
		
		struct {
			Token* identifier;
			Stmt** params; // params is a vector of Stmts, to ease scope later
			DataType* return_data_type;
			Stmt** body;
		} func_decl;
		
		struct {
			Token* identifier;
			DataType* data_type;
			Expr* initializer;
		} var_decl;
		
		Expr* expr_stmt;
	};
};