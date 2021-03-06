#pragma once

#include <ether.hpp>
#include <token.hpp>
#include <stmt.hpp>
#include <expr.hpp>
#include <data_type.hpp>

struct ParserOutput {
	Stmt** stmts;
	Stmt** decls;
	error_code error_occured;
};

enum ParserErrorLocation {
	STRUCT_HEADER,
	STRUCT_BODY,
	FUNCTION_HEADER,
	FUNCTION_BODY,
	IF_HEADER,
	IF_BODY,
	FOR_HEADER,
	FOR_BODY,
	SWITCH_HEADER,
	SWITCH_BRANCH,
	GLOBAL,
};

struct Parser {
	Token** tokens;
	SourceFile* srcfile;

	Stmt** stmts;
	Stmt** decls;
	
	u64 token_idx;
	u64 tokens_len;

	u64 error_count;
	bool error_panic;
	bool dont_sync;
	ParserErrorLocation error_loc;
	u64 error_brace_count;
	bool error_lbrace_parsed = false;

	Stmt* current_struct;
	char** pending_imports;
	
	ParserOutput parse(Token** _tokens, SourceFile* _srcfile);
	void add_pending_imports();

private:
	Stmt* decl_global();
	Stmt* decl(bool is_global);
	Stmt* struct_field();
	Stmt* stmt();
	Stmt* if_branch(Stmt* if_stmt, IfBranchType type);
	Stmt* switch_branch(Stmt* switch_stmt);
	Stmt* expr_stmt();

	Stmt* struct_create(Stmt* stmt, Token* identifier, Stmt** fields);
	Stmt* func_decl_create(Token* identifier, Stmt** params, DataType* return_data_type, Stmt** body, bool is_function, bool is_public);
	Stmt* global_var_decl_create(Token* identifier, DataType* data_type, Expr* initializer, bool is_variable);
	Stmt* var_decl_create(Token* identifier, DataType* data_type, Expr* initializer, bool is_variable);
	Stmt* for_stmt_create(Stmt* counter, Expr* end, Stmt** body);
	Stmt* return_stmt_create(Expr* to_return);
	Stmt* block_create(Stmt** block);
	Stmt* expr_stmt_create(Expr* expr);

	Expr* expr();
	Expr* expr_precedence_14();
	Expr* expr_precedence_12();
	Expr* expr_precedence_11();
	Expr* expr_precedence_9();
	Expr* expr_precedence_8();
	Expr* expr_precedence_7();
	Expr* expr_precedence_6();
	Expr* expr_precedence_5();
	Expr* expr_precedence_4();
	Expr* expr_precedence_3();
	Expr* expr_precedence_2();
	Expr* expr_precedence_1();
	Expr* expr_precedence_0();
	Expr* expr_grouping();

	Expr* binary_create(Expr* left, Expr* right, Token* op);
	Expr* unary_create(Token* op, Expr* right);
	Expr* cast_create(Token* start, DataType* cast_to, Expr* right);
	Expr* func_call_create(Expr* left, Expr** args);
	Expr* array_access_create(Expr* left, Expr* index, Token* end);
	Expr* member_access_create(Expr* left, Token* right);
	Expr* variable_ref_create(Token* identifier);
	Expr* number_create(Token* number);
	Expr* string_create(Token* string);
	Expr* char_create(Token* chr);
	Expr* constant_create(Token* constant);
	
	bool match_identifier();
	bool match_keyword(char* keyword);
	bool match_by_type(TokenType type);
	bool match_double_colon();
	bool match_lparen();
	bool match_rparen();
	bool match_lbrace();
	bool match_rbrace();
	bool match_lbracket();
	bool match_rbracket();
	bool match_langbkt();
	bool match_rangbkt();
	bool match_semicolon();
	DataType* match_data_type();
	void previous_data_type(DataType* data_type);
	
	Token* consume_identifier();
	DataType* consume_data_type();
	void consume_double_colon();
	void consume_lparen();
	void consume_rparen();
	void consume_lbrace();
	void consume_rbrace();
	void consume_lbracket();
	void consume_rbracket();
	void consume_langbkt();
	void consume_rangbkt();
	void consume_semicolon();

	void expect_by_type(TokenType type, const char* fmt, ...);
	
	Token* current();
	Token* previous();

	void goto_next_token();
	void goto_previous_token();

	void verror(const char* fmt, va_list ap);
	void error(const char* fmt, ...);
	void sync_to_next_statement();

public:
	void error_root(SourceFile* _srcfile, u64 line, u64 column, u64 char_count, const char* fmt, va_list ap);
	void warning_root(SourceFile* _srcfile, u64 line, u64 column, u64 char_count, const char* fmt, va_list ap);
}; 
