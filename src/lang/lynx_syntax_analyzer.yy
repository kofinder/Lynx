%language "C++"
%debug
%expect 0

%defines
%define api.namespace {LynxLang}
%define api.parser.class {Parser}
%define parse.error verbose
%define api.value.type variant

%{
    #include "LynxScanner.hpp"
    #include "LynxDriver.hpp"
    #include "LexerState.hpp"
    
    #include "TypeNestingLimiter.hpp"
    #include "LiteralNestingLimiter.hpp"

    #include "location.hh"
    #include "parser.hpp"
    #include <iostream>
    #include <stdio.h>
    #include <string>
    #include <vector>
    #include <limits>
    #include <variant>


    void yyerror(const char* msg);


    // Define a macro named 'yylex' that redirects calls to the lexer function.
    // This allows the Bison parser to use the 'yylex' method from the 'scanner' member
    // of the 'driver' object, providing a cleaner interface for accessing the lexer.
    // #undef yylex
    #define yylex driver.scanner->yylex
    #define SETLINE(node) node->setLineNumber(driver.cursor->begin.line);
%}

%code requires {
   #include <iostream>
    #include <numeric>
    #include <string>
    #include <variant>
    #include <llvm/ADT/ArrayRef.h>

    #include <constants/LValueType.hpp>
    #include <constants/DataType.hpp>
    #include <constants/Parameter.hpp>
    #include <constants/Argument.hpp>
    #include <constants/GenericArgumentType.hpp>
    #include <constants/runtimetypes/DateDataType.hpp>
    #include <constants/runtimetypes/DateTimeDataType.hpp>
    #include <constants/runtime/QualifiedPrefixType.hpp>

    #include <constants/VariableType.hpp>
    #include <constants/ImportType.hpp>
    #include <constants/NodeType.hpp>

    #include <constants/OperatorType.hpp>
    #include <constants/AccessModifierType.hpp>
    #include <constants/ClassInheritanceType.hpp>


    #include <constants/expressions/ExpressionType.hpp>
    #include <constants/expressions/PrimaryExpressionType.hpp>
    #include <constants/expressions/UnaryExpressionType.hpp>
    #include <constants/expressions/AssignExpressionType.hpp>


    #include <constants/DereferenceAccessType.hpp>
    #include <constants/SwitchBlockType.hpp>

    #include <ast/Node.hpp>
    #include <ast/IdentifierNode.hpp>
    #include <ast/QualifiedIdentifierNode.hpp>
    #include <ast/QualifiedFunctionCallNode.hpp>
    #include <ast/StaticMethodCallNode.hpp>

    #include <ast/LiteralNode.hpp>
    #include <ast/helper/LiteralPairNode.hpp>
    #include <ast/helper/LiteralListNode.hpp>
    #include <ast/helper/LiteralMapNode.hpp>
    #include <ast/CollectionInitializerNode.hpp>

    #include <ast/FunctionNode.hpp>
    #include <ast/helper/ImportSymbol.hpp>
    #include <ast/ImportStatementNode.hpp>

    // class
    #include <ast/ClazzConstructorNode.hpp>
    #include <ast/ObjectAccessNode.hpp>
    #include <ast/MethodCallNode.hpp>
    #include <ast/ObjectCreationNode.hpp>
    #include <ast/MixinDeclarationNode.hpp>
    #include <ast/ClazzDeclarationNode.hpp>
    #include <ast/InterfaceDeclarationNode.hpp>
    #include <ast/SuperConstructorCallNode.hpp>
    #include <ast/ChainedDereferenceNode.hpp>
    #include <ast/ClazzConstructorExpressionNode.hpp>

    // enum
    #include <ast/EnumDeclarationNode.hpp>
    #include <ast/EnumValueExpressionNode.hpp>

    #include <ast/StatementNode.hpp>
    #include <ast/ExpressionStatementNode.hpp>

   // arrays 
    #include <ast/ArrayInitializerNode.hpp>
    #include <ast/ArrayAccessNode.hpp>


    #include <ast/ExpressionNode.hpp>
    #include <ast/PrimaryExpressionNode.hpp>

    // siwtch statment
    #include <ast/SwitchStatementNode.hpp>
    #include <ast/helper/SwitchCaseBlockNode.hpp>
    #include <ast/helper/SwitchLevelExpressionNode.hpp>
    #include <ast/helper/SwitchCaseExpressionNode.hpp>

  
    #include <ast/AssignmentExpressionNode.hpp>
    #include <ast/LoggerPrinterNode.hpp>
    #include <ast/FunctionCallNode.hpp>
    #include <ast/StatementListNode.hpp>
    #include <ast/IfStatementNode.hpp>
    #include <ast/BinaryExpressionNode.hpp>
    #include <ast/UnaryExpressionNode.hpp>
    #include <ast/TernaryExpressionNode.hpp>
    #include <ast/ComparisonExpressionNode.hpp>
    #include <ast/ForLoopStatementNode.hpp>
    #include <ast/ExceptionHandlerNode.hpp>
    #include <ast/VariableDeclarationNode.hpp>
    #include <ast/VariableDereferenceNode.hpp>

    // Namespaces
    using namespace LynxAst;
    using namespace LynxConstants;

    // Basic Types
    using byteValue = uint8_t;
    using shortValue = short;
    using integerValue = int;
    using longValue = long;
    using floatValue = float;
    using doubleValue = double;
    using booleanValue = bool;
    using charValue = char;
    using date = Date;
    using datetime = DateTime;
    using identifier = std::string;
    using nullptr_t = std::nullptr_t;


    using root_ast_node                       = std::unique_ptr<Node>;
    using identifier_ast_node                 = std::unique_ptr<IdentifierNode>;

    using literal_ast_node                    = std::unique_ptr<LiteralNode>;
    using literal_list_ast_node               = std::unique_ptr<LiteralListNode>;
    using literal_pair_ast_node               = std::unique_ptr<LiteralPairNode>;
    using literal_map_ast_node                = std::unique_ptr<LiteralMapNode>;

    using literal_list_ast_nodes              = std::vector<std::unique_ptr<Node>>;
    using literal_pair_ast_nodes              = std::vector<std::unique_ptr<LiteralPairNode>>;
    using collection_init_ast_node            = std::unique_ptr<CollectionInitializerNode>;


    using function_ast_node                    = std::unique_ptr<FunctionNode>;
    using statement_ast_node                   = std::unique_ptr<StatementNode>;
    using function_call_ast_node               = std::unique_ptr<FunctionCallNode>;
    using static_method_call_ast_node          = std::unique_ptr<StaticMethodCallNode>;


    using expression_ast_node                  = std::unique_ptr<ExpressionNode>;
    using primary_expression_ast_node          = std::unique_ptr<PrimaryExpressionNode>;
    using import_statment_ast_node             = std::unique_ptr<ImportStatementNode>;

    using binary_expression_ast_node           = std::unique_ptr<BinaryExpressionNode>;
    using unary_expression_ast_node            = std::unique_ptr<UnaryExpressionNode>;
    using comparsion_expression_ast_node       = std::unique_ptr<ComparisonExpressionNode>;

    using array_initializer_ast_node           = std::unique_ptr<ArrayInitializerNode>;
    using statment_list_ast_node               = std::unique_ptr<StatementListNode>;
    using if_statement_ast_node                = std::unique_ptr<IfStatementNode>;
    using for_loop_statement_ast_node          = std::unique_ptr<ForLoopStatementNode>;
    using expression_statement_ast_node        = std::unique_ptr<ExpressionStatementNode>;
    using assignment_expression_ast_node       = std::unique_ptr<AssignmentExpressionNode>;

    using ternary_expression_ast_node          = std::unique_ptr<TernaryExpressionNode>;

    // switch case
    using switch_statment_ast_node             = std::unique_ptr<SwitchStatementNode>;
    using switch_lvl_expression_ast_node       = std::unique_ptr<SwitchLevelExpressionNode>;
    using switch_case_expression_ast_node      = std::unique_ptr<SwitchCaseExpressionNode>;
    using switch_case_block_ast_node           = std::unique_ptr<SwitchCaseBlockNode>;
    using list_of_case_block_ast_node          = std::unique_ptr<std::vector<std::unique_ptr<SwitchCaseBlockNode>>>;

    using logger_printer_ast_node              = std::unique_ptr<LoggerPrinterNode>;
    using exception_handle_ast_node            = std::unique_ptr<ExceptionHandlerNode>;
    using chained_dereference_ast_node         = std::unique_ptr<ChainedDereferenceNode>;
    using variable_declaration_ast_node        = std::unique_ptr<VariableDeclarationNode>;
    using variable_dereference_ast_node        = std::unique_ptr<VariableDereferenceNode>;

    using object_creation_ast_node             = std::unique_ptr<ObjectCreationNode>;
    using method_call_ast_node                 = std::unique_ptr<MethodCallNode>;

    using clazz_declaration_ast_node           = std::unique_ptr<ClazzDeclarationNode>;
    using clazz_constructor_ast_node           = std::unique_ptr<ClazzConstructorNode>;
    using super_constructor_call_ast_node      = std::unique_ptr<SuperConstructorCallNode>;

    using interface_declaration_ast_node       = std::unique_ptr<InterfaceDeclarationNode>;
    using mixin_declaration_ast_node           = std::unique_ptr<MixinDeclarationNode>;

    using list_of_clazz_names                  = std::vector<std::string>;
    using clazz_inheritance_type               = std::unique_ptr<ClassInheritanceType>;


    using array_access_ast_node                = std::unique_ptr<ArrayAccessNode>;
    using object_access_ast_node               = std::unique_ptr<ObjectAccessNode>;
    using list_of_ast_node                     = std::unique_ptr<std::vector<std::unique_ptr<Node>>>;

    using qualified_prefix_type                = std::unique_ptr<QualifiedPrefixType>;
    using parameter                            = std::shared_ptr<Parameter>;

    using var_types                            = std::shared_ptr<VariableType>;
    using generic_arg_type                     = std::shared_ptr<GenericArgumentType>;
    using list_of_generic_args                 = std::shared_ptr<std::vector<std::shared_ptr<GenericArgumentType>>>;
    

    using access_mode_type                     = AccessModifierType;

    // Enums
    using enum_declaration_ast_node            = std::unique_ptr<EnumDeclarationNode>;
    using enum_value_expression_ast_node       = std::unique_ptr<EnumValueExpressionNode>;
    using enum_member                          = std::pair<std::string, std::variant<int, char, std::string>>*; 
    using enum_members                         = std::vector<std::pair<std::string, std::variant<int, char, std::string>>>*;
    using qualified_identifier_ast_node        = std::unique_ptr<QualifiedIdentifierNode>;
    using qualified_func_call_ast_node         = std::unique_ptr<QualifiedFunctionCallNode>;

    using import_symbol                        = std::unique_ptr<ImportSymbol>;
    using import_list                          = std::vector<std::unique_ptr<ImportSymbol>>;

    using list_of_parameters                   = std::shared_ptr<std::vector<std::shared_ptr<Parameter>>>;
    using list_of_arguments                    = std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>>;
    using primary_exp_arguments                = std::unique_ptr<std::vector<std::unique_ptr<PrimaryExpressionNode>>>;
}


%parse-param {class LynxDriver &driver}
%lex-param { LynxDriver &driver }


%token <byteValue> TOK_BYTE
%token <shortValue> TOK_SHORT
%token <integerValue> TOK_INTEGER
%token <longValue> TOK_LONG
%token <floatValue> TOK_FLOAT
%token <doubleValue> TOK_DOUBLE
%token <booleanValue> TOK_BOOLEAN
%token <charValue> TOK_CHAR
%token <identifier> TOK_STRING

%token <identifier> TOK_IDENTIFIER

%token <date> TOK_DATE
%token <datetime> TOK_DATE_TIME


%token TOK_EOF 0
%token KW_BYTE KW_SHORT KW_INT KW_LONG KW_FLOAT KW_DOUBLE KW_BOOLEAN KW_CHAR KW_STRING KW_AUTO
%token KW_CLASS KW_INTERFACE KW_MIXIN KW_ABSTRACT 
%token KW_STATIC KW_CONST KW_VIRTUAL KW_INLINE KW_THIS KW_VOID 
%token KW_IMPLEMENTS KW_EXTENDS KW_WITH KW_ON KW_SUPER KW_OVERRIDE KW_PRIVATE KW_PUBLIC KW_PROTECTED
%token KW_LOG KW_CONSOLE KW_TRY KW_CATCH KW_FINALLY KW_RETURN KW_THROW
%token KW_IF KW_ELSE KW_WHILE KW_FOR KW_SWITCH KW_CASE KW_BREAK KW_DEFAULT
%token TOK_NEW
%token KW_ENUM


%token TOK_IMPORT TOK_FROM TOK_AS TOK_SIZE_OF TOK_ALIGN_OF TOK_ALIGN_AS TOK_NULL_PTR
%token TOK_PLUS TOK_MINUS TOK_MULTIPLY TOK_DIVIDE TOK_MODULO
%token TOK_GREATER_THAN TOK_LESS_THAN TOK_GTE TOK_LTE TOK_EQUALITY TOK_NEQUALITY TOK_L_AND TOK_L_OR
%token TOK_QUESTION_MARK TOK_INCREMENT TOK_DECREMENT TOK_LOGICAL_NOT TOK_BITWISE_XOR TOK_NULL_COALESCE
%token TOK_PLUS_ASSIGN TOK_MINUS_ASSIGN TOK_MULTIPLY_ASSIGN TOK_DIVIDE_ASSIGN TOK_MODULO_ASSIGN
%token TOK_LSHIFT_ASSIGN TOK_RSHIFT_ASSIGN TOK_AND_ASSIGN TOK_XOR_ASSIGN
%token TOK_BIT_AND TOK_BIT_OR
%token TOK_COMMA TOK_SEMICOLON TOK_DOUBLE_COLON TOK_COLON TOK_DOT
%token TOK_ASSIGN TOK_LBRACKET TOK_RBRACKET TOK_RBRACE TOK_LPAREN TOK_RPAREN
%token TOK_LBRACE_COMPOUND TOK_LBRACE_LITERAL 


%start program

%type <root_ast_node> program statement loop_expression clazz_body interface_body mixin_body variable_access constructor_assignment literal

%type <literal_ast_node> literal_value
%type <literal_list_ast_node> literal_list
%type <literal_list_ast_nodes> literal_list_items

%type <literal_pair_ast_node> literal_pair
%type <literal_pair_ast_nodes> literal_pairs
%type <literal_map_ast_node> literal_map
%type <collection_init_ast_node> literal_initializer

%type <import_list> import_list
%type <import_symbol>  import_specifier

%type <list_of_arguments> arguments arguments_opt 
%type <primary_exp_arguments> super_arguments super_arguments_opt
%type <parameter> parameter
%type <list_of_parameters> parameters parameters_opt

%type <var_types> type_specifier simple_type generic_type
%type <list_of_generic_args> type_arguments
%type <generic_arg_type> type_argument
%type <access_mode_type> access_modifier


// ENUM
%type <enum_member> enum_member
%type <enum_members> enum_members
%type <enum_declaration_ast_node> enum_definition 
%type <enum_value_expression_ast_node> enum_value_expression

// CLAZZ
%type <clazz_declaration_ast_node> clazz_declaration
%type <booleanValue> clazz_modifiers
%type <list_of_clazz_names> list_of_inheritance_clazz
%type <clazz_inheritance_type> clazz_inheritance
%type <interface_declaration_ast_node> interface_declaration 
%type <mixin_declaration_ast_node> mixin_decleration mixin_class_decleration
%type <method_call_ast_node> method_call_expression
%type <object_creation_ast_node> object_creation_expression
%type <list_of_ast_node> exp_list exp_list_opt clazz_contents clazz_contents_opt interface_contents interface_contents_opt mixin_contents_opt mixin_contents

%type <clazz_constructor_ast_node> clazz_constructor clazz_constructor_with_super
// %type <super_constructor_call_ast_node> super_call_expression

%type<array_access_ast_node> array_access
%type <identifier_ast_node> identifier_expr

%type <qualified_prefix_type> qualified_prefix
%type <qualified_identifier_ast_node> qualified_entity
%type <qualified_func_call_ast_node> qualified_function_call

// SWITCH
%type <switch_statment_ast_node> switch_statment 
%type <list_of_case_block_ast_node> case_blocks
%type <switch_case_block_ast_node> case_block default_block
%type <switch_lvl_expression_ast_node> switch_level_expression
%type <switch_case_expression_ast_node> switch_case_const_expression

%type <for_loop_statement_ast_node> loop_statement
%type <if_statement_ast_node> if_else_statement if_statement

%type <static_method_call_ast_node> static_method_call_expression
%type <expression_ast_node> expression
%type <primary_expression_ast_node> primary_expression
%type <binary_expression_ast_node> binary_expression
%type <comparsion_expression_ast_node> compare_expression
%type <unary_expression_ast_node> unary_expression
%type <assignment_expression_ast_node> assignment_expression 
%type <ternary_expression_ast_node> ternary_expression
%type <expression_statement_ast_node> expression_statement

%type <import_statment_ast_node> import_statement
%type <logger_printer_ast_node> log_statement
%type <statment_list_ast_node> compound_statement statements 
%type <statement_ast_node> return_statement throw_smt

%type <variable_dereference_ast_node> single_dereference
%type <chained_dereference_ast_node> chained_dereference
%type <variable_declaration_ast_node> variable_declaration clazz_variable

%type <function_call_ast_node> function_call_expression
%type <exception_handle_ast_node> try_catch try_catch_finally
%type <function_ast_node> function_declaration function_definition clazz_function_declaration clazz_function


//// /////////////////////////////////////
// Operator Precedence & Associativity  //
//// /////////////////////////////////////

%left TOK_L_OR TOK_L_AND
%left TOK_EQUALITY TOK_NEQUALITY
%left TOK_LTE TOK_GTE
%left TOK_PLUS TOK_MINUS
%left TOK_MULTIPLY TOK_DIVIDE TOK_MODULO
%left TOK_BITWISE_XOR
%left TOK_LSHIFT TOK_RSHIFT

%left TOK_DOT
%left TOK_NULL_COALESCE
%left TOK_DOUBLE_COLON
%left TOK_COMMA 
%left TOK_QUESTION_MARK
%left LOWER_THAN_CALL

%right TOK_COLON
%right TOK_LPAREN TOK_RPAREN

%nonassoc TOK_LESS_THAN TOK_GREATER_THAN
%nonassoc TOK_LBRACKET
%nonassoc TOK_ASSIGN TOK_PLUS_ASSIGN TOK_MINUS_ASSIGN TOK_MULTIPLY_ASSIGN TOK_DIVIDE_ASSIGN TOK_MODULO_ASSIGN TOK_LSHIFT_ASSIGN TOK_RSHIFT_ASSIGN TOK_AND_ASSIGN TOK_XOR_ASSIGN
%nonassoc TOK_INCREMENT TOK_DECREMENT
%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE
%nonassoc LOWER_THAN_GENERIC


%%

program: statements  { driver.setAstRoot(std::move($1)); $$ = nullptr; };

statements:
  statements statement { $1->pushStatement(std::move($2)); $$ = std::move($1); }
  | %empty { $$ = std::make_unique<StatementListNode>(); }   
;

statement:
  variable_declaration  TOK_SEMICOLON         { $$ = std::move($1); SETLINE($$); }
  | expression_statement                      { $$ = std::move($1); SETLINE($$); }
  | log_statement TOK_SEMICOLON               { $$ = std::move($1); SETLINE($$); }
  | import_statement TOK_SEMICOLON            { $$ = std::move($1); SETLINE($$); }
  | if_else_statement                         { $$ = std::move($1); SETLINE($$); }
  | switch_statment                           { $$ = std::move($1); SETLINE($$); }
  | loop_statement                            { $$ = std::move($1); SETLINE($$); }
  | enum_definition                           { $$ = std::move($1); SETLINE($$); }
  | clazz_declaration                         { $$ = std::move($1); SETLINE($$); }
  | interface_declaration                     { $$ = std::move($1); SETLINE($$); }
  | mixin_decleration                         { $$ = std::move($1); SETLINE($$); }
  | mixin_class_decleration                   { $$ = std::move($1); SETLINE($$); }
  | function_definition                       { $$ = std::move($1); SETLINE($$); }
  | compound_statement                        { $$ = std::move($1); SETLINE($$); }
  | return_statement  TOK_SEMICOLON           { $$ = std::move($1); SETLINE($$); }
  | try_catch_finally                         { $$ = std::move($1); SETLINE($$); }
  | throw_smt                                 { $$ = std::move($1); SETLINE($$); }
;

expression_statement:
  expression TOK_SEMICOLON      { $$ = std::make_unique<ExpressionStatementNode>(std::move($1)); }
;

expression:
  primary_expression                                { $$ = std::make_unique<ExpressionNode>(ExpressionType::PRIMARY_EXPR, std::move($1)); }
  | static_method_call_expression                   { $$ = std::make_unique<ExpressionNode>(ExpressionType::STATIC_METHOD_CALL_EXPR, std::move($1)); }
  | method_call_expression                          { $$ = std::make_unique<ExpressionNode>(ExpressionType::METHOD_CALL_EXPR, std::move($1)); }
  | assignment_expression                           { $$ = std::make_unique<ExpressionNode>(ExpressionType::ASSIGNMENT_EXPR, std::move($1)); }
  | ternary_expression                              { $$ = std::make_unique<ExpressionNode>(ExpressionType::TERNARY_EXPR, std::move($1)); }
  | unary_expression                                { $$ = std::make_unique<ExpressionNode>(ExpressionType::UNARY_EXPR, std::move($1)); }
  | binary_expression                               { $$ = std::make_unique<ExpressionNode>(ExpressionType::BINARY_EXPR, std::move($1)); }
  | compare_expression                              { $$ = std::make_unique<ExpressionNode>(ExpressionType::COMPARE_EXPR, std::move($1)); }
;


primary_expression:
  function_call_expression                          { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::FUNCTION_CALL_EXPR, std::move($1)); }
  | literal                                         { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::SCALAR_EXPR, std::move($1)); }
  | qualified_entity                                { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::ENUM_ACCESS_EXPR, std::move($1)); }
  | qualified_function_call                         { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::ENUM_ACCESS_EXPR, std::move($1)); }
  | object_creation_expression                      { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::OBJECT_CREATION_EXPR, std::move($1)); }
  | TOK_IDENTIFIER %prec LOWER_THAN_CALL            { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::IDENTIFIER_EXPR, std::make_unique<IdentifierNode>(std::move($1))); }
  | TOK_LPAREN expression TOK_RPAREN                { $$ = std::make_unique<PrimaryExpressionNode>(PrimaryExpressionType::PARENTHESIS_EXPR, std::move($2));  }
;

unary_expression:
  // TOK_PLUS primary_expression                         { $$ = new UnaryExpressionNode(OperatorType::PLUS, UnaryExpressionType::PREFIX, $2); }
  // | TOK_MINUS primary_expression                      { $$ = new UnaryExpressionNode(OperatorType::MINUS, UnaryExpressionType::PREFIX, $2); }
  TOK_LOGICAL_NOT primary_expression                  { $$ = std::make_unique<UnaryExpressionNode>(OperatorType::LOGICAL_NOT, UnaryExpressionType::PREFIX, std::move($2)); }
  | TOK_INCREMENT TOK_IDENTIFIER                      { $$ = std::make_unique<UnaryExpressionNode>(OperatorType::INCREMENT, UnaryExpressionType::PREFIX, std::move($2)); }
  | TOK_DECREMENT TOK_IDENTIFIER                      { $$ = std::make_unique<UnaryExpressionNode>(OperatorType::DECREMENT, UnaryExpressionType::PREFIX, std::move($2)); }
  | TOK_IDENTIFIER TOK_INCREMENT %prec TOK_INCREMENT  { $$ = std::make_unique<UnaryExpressionNode>(OperatorType::INCREMENT, UnaryExpressionType::POSTFIX, std::move($1)); }
  | TOK_IDENTIFIER TOK_DECREMENT %prec TOK_DECREMENT  { $$ = std::make_unique<UnaryExpressionNode>(OperatorType::DECREMENT, UnaryExpressionType::POSTFIX, std::move($1)); }
;

binary_expression:
  primary_expression TOK_PLUS primary_expression                  { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::PLUS, std::move($1), std::move($3)); }
  | primary_expression TOK_MINUS primary_expression               { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::MINUS, std::move($1), std::move($3)); }
  | primary_expression TOK_MULTIPLY primary_expression            { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::MUL, std::move($1), std::move($3)); }
  | primary_expression TOK_DIVIDE primary_expression              { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::DIV, std::move($1), std::move($3)); }
  | primary_expression TOK_BITWISE_XOR primary_expression         { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::BITWISE_XOR, std::move($1), std::move($3)); }
  | primary_expression TOK_L_AND primary_expression               { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::LOGICAL_AND, std::move($1), std::move($3)); }
  | primary_expression TOK_L_OR primary_expression                { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::LOGICAL_OR, std::move($1), std::move($3)); }
  | primary_expression TOK_NULL_COALESCE primary_expression       { $$ = std::make_unique<BinaryExpressionNode>(OperatorType::NULL_COALESCE, std::move($1), std::move($3)); }
;

compare_expression:
  primary_expression TOK_MODULO primary_expression                { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::MODULO_DIV, std::move($1), std::move($3)); }
  | primary_expression TOK_LESS_THAN primary_expression           { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::LESS_THAN, std::move($1), std::move($3)); }
  | primary_expression TOK_GREATER_THAN primary_expression        { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::GREATER_THAN, std::move($1), std::move($3)); }
  | primary_expression TOK_LTE primary_expression                 { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::LESS_THAN_EQUAL, std::move($1), std::move($3)); }
  | primary_expression TOK_GTE primary_expression                 { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::GREATER_THAN_EQUAL, std::move($1), std::move($3)); }
  | primary_expression TOK_EQUALITY primary_expression            { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::EQUALITY, std::move($1), std::move($3)); }
  | primary_expression TOK_NEQUALITY primary_expression           { $$ = std::make_unique<ComparisonExpressionNode>(OperatorType::NOT_EQUALITY, std::move($1), std::move($3)); }
;

assignment_expression:
  variable_access TOK_ASSIGN expression             { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::NONE, std::move($3), AssignExpressionType::SIMPLE_ASSIGN); }
  | variable_access TOK_PLUS_ASSIGN expression      { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::PLUS_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_MINUS_ASSIGN expression     { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::MINUS_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_MULTIPLY_ASSIGN expression  { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::MULTIPLY_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_DIVIDE_ASSIGN expression    { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::DIVIDE_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_MODULO_ASSIGN expression    { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::MODULO_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_LSHIFT_ASSIGN expression    { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::LSHIFT_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_RSHIFT_ASSIGN expression    { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::RSHIFT_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_AND_ASSIGN expression       { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::AND_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
  | variable_access TOK_XOR_ASSIGN expression       { $$ = std::make_unique<AssignmentExpressionNode>(std::move($1), OperatorType::XOR_ASSIGN, std::move($3), AssignExpressionType::COMPLEX_ASSIGN); }
;

// INTERFACE GROUP
interface_declaration:
  KW_INTERFACE TOK_IDENTIFIER TOK_LBRACE_COMPOUND interface_contents_opt TOK_RBRACE { 
    auto interfaceNode = std::make_unique<InterfaceDeclarationNode>($2, std::move($4));
    driver.registerInterface(interfaceNode.get());
    $$ = std::move(interfaceNode);
  }
  | KW_INTERFACE TOK_IDENTIFIER KW_EXTENDS list_of_inheritance_clazz TOK_LBRACE_COMPOUND interface_contents_opt TOK_RBRACE { 
    auto interfaceNode = std::make_unique<InterfaceDeclarationNode>($2, std::move($6));
    driver.registerInterface(interfaceNode.get());
    $$ = std::move(interfaceNode);
  }
;

interface_contents_opt:
  interface_contents                        { $$ = std::move($1); }
  | %empty                                  { $$ = std::unique_ptr<std::vector<std::unique_ptr<Node>>>(); }
;

interface_contents:
  interface_body  { 
    auto vec = std::make_unique<std::vector<std::unique_ptr<Node>>>(); 
    vec->push_back(std::move($1)); 
    $$ = std::move(vec);
  }
  | interface_contents interface_body {
    $1->push_back(std::move($2)); 
    $$ = std::move($1); 
  }
;

interface_body:
  clazz_function_declaration TOK_SEMICOLON  { $$ = std::move($1); }
  | clazz_variable TOK_SEMICOLON            { $$ = std::move($1); }
;

mixin_decleration:
  KW_MIXIN TOK_IDENTIFIER TOK_LBRACE_COMPOUND mixin_contents_opt TOK_RBRACE {
    auto mixinNode = std::make_unique<MixinDeclarationNode>($2, std::move($4));
    driver.addAnalyzerNode(mixinNode.get());
    $$ = std::move(mixinNode);
  }
  | KW_MIXIN TOK_IDENTIFIER KW_ON list_of_inheritance_clazz TOK_LBRACE_COMPOUND mixin_contents_opt TOK_RBRACE {
    auto mixinNode = std::make_unique<MixinDeclarationNode>($2, std::move($4), std::move($6));
    driver.addAnalyzerNode(mixinNode.get());
    $$ = std::move(mixinNode);
  }
;

mixin_class_decleration:
  KW_MIXIN KW_CLASS TOK_IDENTIFIER TOK_LBRACE_COMPOUND mixin_contents_opt TOK_RBRACE {
    auto mixinNode = std::make_unique<MixinDeclarationNode>($3, std::move($5));
    driver.addAnalyzerNode(mixinNode.get());
    $$ = std::move(mixinNode);
  }
;

mixin_contents_opt:
  mixin_contents                        { $$ = std::move($1); }
  | %empty                              { $$ = std::unique_ptr<std::vector<std::unique_ptr<Node>>>(); }
;

mixin_contents:
  mixin_body  { 
    auto vec = std::make_unique<std::vector<std::unique_ptr<Node>>>(); 
    vec->push_back(std::move($1)); 
    $$ = std::move(vec);
  }
  | mixin_contents mixin_body {
    $1->push_back(std::move($2)); 
    $$ = std::move($1); 
  }
;

mixin_body:
  clazz_function                              { $$ = std::move($1); }
  | clazz_function_declaration                { $$ = std::move($1); }
  | clazz_function_declaration TOK_SEMICOLON  { $$ = std::move($1); }
  | clazz_variable TOK_SEMICOLON              { $$ = std::move($1); }
;


clazz_declaration:
  clazz_modifiers KW_CLASS TOK_IDENTIFIER clazz_inheritance TOK_LBRACE_COMPOUND clazz_contents_opt TOK_RBRACE {
    auto clazzNode = std::make_unique<ClazzDeclarationNode>($3, std::move($4), std::move($6));
    if ($1) clazzNode->isAbstract = true;
    driver.registerClazz(clazzNode.get());
    $$ = std::move(clazzNode);
  }
;

clazz_modifiers:
  KW_ABSTRACT   { $$ = true; }
  | %empty      { $$ = false; }
;

clazz_inheritance:
  %empty { $$ = std::make_unique<ClassInheritanceType>(); }
  | KW_EXTENDS TOK_IDENTIFIER {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->baseClass = std::string($2);
    $$ = std::move(ci);
  }
  | KW_WITH list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->mixins = std::move($2);
    $$ = std::move(ci);
  }
  | KW_IMPLEMENTS list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->interfaces = std::move($2);
    $$ = std::move(ci);
  }
  | KW_IMPLEMENTS list_of_inheritance_clazz KW_WITH list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->interfaces = std::move($2);
    ci->mixins = std::move($4);
    $$ = std::move(ci);
  }
  | KW_WITH list_of_inheritance_clazz KW_IMPLEMENTS list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->mixins = std::move($2);
    ci->interfaces = std::move($4);
    $$ = std::move(ci);
  }
  | KW_EXTENDS TOK_IDENTIFIER KW_WITH list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->baseClass = std::string($2);
    ci->mixins = std::move($4);
    $$ = std::move(ci);
  }
  | KW_EXTENDS TOK_IDENTIFIER KW_IMPLEMENTS list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->baseClass = std::string($2);
    ci->interfaces = std::move($4);
    $$ = std::move(ci);
  }
  | KW_EXTENDS TOK_IDENTIFIER KW_WITH list_of_inheritance_clazz KW_IMPLEMENTS list_of_inheritance_clazz {
    auto ci = std::make_unique<ClassInheritanceType>();
    ci->baseClass = std::string($2);
    ci->mixins = std::move($4);
    ci->interfaces = std::move($6);
    $$ = std::move(ci);
  }
;

list_of_inheritance_clazz: 
  TOK_IDENTIFIER                                  { 
    std::vector<std::string> vec;
    vec.push_back($1);
    $$ = std::move(vec);
  }
  | list_of_inheritance_clazz TOK_COMMA TOK_IDENTIFIER  { 
    $1.push_back($3);
    $$ = std::move($1);
  }
;

clazz_contents_opt:
  clazz_contents                        { $$ = std::move($1); }
  | %empty                              { $$ = std::unique_ptr<std::vector<std::unique_ptr<Node>>>(); }
;

clazz_contents:
  clazz_body  { 
    auto vec = std::make_unique<std::vector<std::unique_ptr<Node>>>(); 
    vec->push_back(std::move($1)); 
    $$ = std::move(vec);
  }
  | clazz_contents clazz_body {
    $1->push_back(std::move($2)); 
    $$ = std::move($1); 
  }
;

clazz_body:
  clazz_function                              { $$ = std::move($1); }
  | clazz_function_declaration                { $$ = std::move($1); }
  | clazz_function_declaration TOK_SEMICOLON  { $$ = std::move($1); }
  | clazz_constructor                         { $$ = std::move($1); }
  | clazz_constructor_with_super              { $$ = std::move($1); }
  | clazz_variable TOK_SEMICOLON              { $$ = std::move($1); }
;

clazz_constructor: 
  access_modifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN TOK_LBRACE_COMPOUND exp_list_opt TOK_RBRACE {
    $$ = std::make_unique<ClazzConstructorNode>(
      $1, 
      $2, 
      std::shared_ptr<std::vector<std::shared_ptr<Parameter>>>($4),
      std::move($7)
    );
  }
;

clazz_constructor_with_super:
  access_modifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN TOK_LBRACE_COMPOUND KW_SUPER TOK_LPAREN super_arguments_opt TOK_RPAREN TOK_SEMICOLON exp_list_opt TOK_RBRACE {
    auto superArgs = std::make_unique<std::vector<std::unique_ptr<PrimaryExpressionNode>>>(std::move(*$9));
    auto superCall = std::make_optional(std::make_unique<SuperConstructorCallNode>($2, std::move(superArgs)));
    $$ = std::make_unique<ClazzConstructorNode>($1, $2, $4, std::move($12), std::move(superCall));
  }
;

clazz_variable:
  type_specifier TOK_IDENTIFIER { $$ = std::make_unique<VariableDeclarationNode>(std::move($1), $2); }
  | access_modifier type_specifier TOK_IDENTIFIER { 
    $$ = std::make_unique<VariableDeclarationNode>(std::move($2), $3); 
    $$->setAccessModifier($1); 
  }
  | type_specifier TOK_IDENTIFIER TOK_ASSIGN expression  { 
    $$ = std::make_unique<VariableDeclarationNode>(std::move($1), $2, std::unique_ptr<Node>(static_cast<Node*>($4.release()))); 
  }
  | access_modifier type_specifier TOK_IDENTIFIER TOK_ASSIGN expression   { 
    $$ = std::make_unique<VariableDeclarationNode>(std::move($2), $3, std::unique_ptr<Node>(static_cast<Node*>($5.release()))); 
    $$->setAccessModifier($1); 
  }
;

clazz_function_declaration:
  type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN  %prec LOWER_THAN_CALL { 
    $$ = std::make_unique<FunctionNode>($1, $2, $4); 
  }
  | access_modifier type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN %prec LOWER_THAN_CALL  { 
    $$ = std::make_unique<FunctionNode>($2, $3, $5); 
    $$->setAccessModifier($1);
  }
  | access_modifier KW_VIRTUAL type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN %prec LOWER_THAN_CALL { 
    $$ = std::make_unique<FunctionNode>($3, $4, $6); 
    $$->setVirtual(true);
    $$->setAccessModifier($1);
  }
  | KW_VIRTUAL type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN %prec LOWER_THAN_CALL  { 
    $$ = std::make_unique<FunctionNode>($2, $3, $5); 
    $$->setVirtual(true);
  }
  | type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN KW_OVERRIDE %prec LOWER_THAN_CALL  { 
    $$ = std::make_unique<FunctionNode>($1, $2, $4); 
   $$->setOverride(true);
  }
  | access_modifier type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN KW_OVERRIDE %prec LOWER_THAN_CALL  { 
    $$ = std::make_unique<FunctionNode>($2, $3, $5); 
    $$->setOverride(true);
    $$->setAccessModifier($1);
  }
;
  

clazz_function:
  clazz_function_declaration compound_statement { $$ = std::move($1); $$->setFunctionBody(std::move($2)); }
;

enum_definition:
  KW_ENUM TOK_IDENTIFIER TOK_LBRACE_COMPOUND enum_members TOK_RBRACE   { 
    $$ = std::make_unique<EnumDeclarationNode>($2);  // ✅ Initialize EnumDeclarationNode
    for (const auto& member : *$4) {  // ✅ Add members properly
      $$->addMember(member.first, member.second);
    }
    delete $4;  // ✅ Safe cleanup of temporary vector
  }
;

enum_members:
  enum_member { 
    $$ = new std::vector<std::pair<std::string, std::variant<int, char, std::string>>>();  
    $$->push_back(*$1); delete $1; 
  }
  | enum_members TOK_COMMA enum_member { 
    $$ = $1; 
    $$->push_back(*$3); 
    delete $3; 
  }
;

enum_member:
  TOK_IDENTIFIER                                 { $$ = new std::pair<std::string, std::variant<int, char, std::string>>($1, -1); }
  | TOK_IDENTIFIER TOK_ASSIGN TOK_CHAR           { $$ = new std::pair<std::string, std::variant<int, char, std::string>>($1, $3); }
  | TOK_IDENTIFIER TOK_ASSIGN TOK_STRING         { $$ = new std::pair<std::string, std::variant<int, char, std::string>>($1, std::move($3)); }
  | TOK_IDENTIFIER TOK_ASSIGN enum_value_expression { 
    int val = $3->evaluate(); // evaluate expression at parse time
    std::cout << "[ENUM] member value evaluate at parse time ===========>: " << val << std::endl;
    $$ = new std::pair<std::string, std::variant<int, char, std::string>>($1, val);
    $3.reset();
  }
;

enum_value_expression:
  TOK_INTEGER                                                 { $$ = std::make_unique<EnumValueExpressionNode>($1); }
  | enum_value_expression TOK_PLUS enum_value_expression      { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::PLUS, std::move($1), std::move($3)); }
  | enum_value_expression TOK_MINUS enum_value_expression     { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::MINUS, std::move($1), std::move($3)); }
  | enum_value_expression TOK_MULTIPLY enum_value_expression  { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::MUL, std::move($1), std::move($3)); }
  | enum_value_expression TOK_DIVIDE enum_value_expression    { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::DIV, std::move($1), std::move($3)); }
  | enum_value_expression TOK_LSHIFT enum_value_expression    { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::LSHIFT, std::move($1), std::move($3)); }
  | enum_value_expression TOK_RSHIFT enum_value_expression    { $$ = std::make_unique<EnumValueExpressionNode>(OperatorType::RSHIFT, std::move($1), std::move($3)); }   
;

function_declaration: type_specifier TOK_IDENTIFIER TOK_LPAREN parameters_opt TOK_RPAREN { 
  auto fnNode = std::make_unique<FunctionNode>($1, $2, $4);
  driver.registerFunction(fnNode.get());
  driver.setLexerState(LexerState::STATE_COMPOUND);
  $$ = std::move(fnNode);  // or store temporarily to set the body later
} ;

function_definition: function_declaration compound_statement { $$ = std::move($1); $$->setFunctionBody(std::move($2)); } ;
  
function_call_expression: TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN %prec LOWER_THAN_CALL { $$ = std::make_unique<FunctionCallNode>($1, std::move($3)); };

static_method_call_expression: 
  KW_BYTE TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("byte", $3, std::move($5));
  }
  | KW_SHORT TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("short", $3, std::move($5));
  }
  | KW_INT TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("int", $3, std::move($5));
  }
  | KW_LONG TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("long", $3, std::move($5));
  }
  | KW_FLOAT TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("float", $3, std::move($5));
  }
  | KW_DOUBLE TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("double", $3, std::move($5));
  }
  | KW_CHAR TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("char", $3, std::move($5));
  }
  | KW_BOOLEAN TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("boolean", $3, std::move($5));
  }
  | KW_STRING TOK_DOT TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN {
    $$ = std::make_unique<StaticMethodCallNode>("string", $3, std::move($5));
  }
;

object_creation_expression: TOK_NEW type_specifier TOK_LPAREN arguments_opt TOK_RPAREN { $$ = std::make_unique<ObjectCreationNode>($2, std::move($4)); };

log_statement: KW_CONSOLE TOK_DOT KW_LOG TOK_LPAREN expression TOK_RPAREN { $$ = std::make_unique<LoggerPrinterNode>(std::move($5)); };

try_catch:
  KW_TRY compound_statement KW_CATCH TOK_LPAREN TOK_IDENTIFIER TOK_COLON type_specifier TOK_RPAREN compound_statement {
    $$ = std::make_unique<ExceptionHandlerNode>(std::move($2));
    $$->addCatchBlock(std::move($9), $7, $5);
  }
  | try_catch KW_CATCH TOK_LPAREN TOK_IDENTIFIER TOK_COLON type_specifier TOK_RPAREN compound_statement {
    $1->addCatchBlock(std::move($8), $6, $4);
    $$ = std::move($1);
  }
;

try_catch_finally:
  try_catch                                       { $$ = std::move($1); }
  | try_catch KW_FINALLY compound_statement       {
    $$ = std::move($1);
    $$->setFinallyBlock(std::move($3));
  }
;

ternary_expression: expression TOK_QUESTION_MARK expression TOK_COLON expression { $$ = std::make_unique<TernaryExpressionNode>(std::move($1), std::move($3), std::move($5)); };

import_statement:
  TOK_IMPORT TOK_LBRACE_COMPOUND import_list TOK_RBRACE TOK_FROM TOK_STRING {
    driver.addImport($6); 
    $$ = std::make_unique<ImportStatementNode>($6, ImportType::Named, std::move($3));
  }
  | TOK_IMPORT TOK_IDENTIFIER TOK_AS TOK_IDENTIFIER TOK_FROM TOK_STRING {
    driver.addImport($6); 
    std::vector<std::unique_ptr<ImportSymbol>> vec;
    vec.push_back(std::make_unique<ImportSymbol>($2, $4));
    $$ = std::make_unique<ImportStatementNode>($6, ImportType::Named, std::move(vec));
  }
  | TOK_IMPORT TOK_IDENTIFIER TOK_FROM TOK_STRING {
    driver.addImport($4); 
    $$ = std::make_unique<ImportStatementNode>($4, ImportType::Default, $2);
  }
  | TOK_IMPORT TOK_IDENTIFIER TOK_COMMA TOK_LBRACE_COMPOUND import_list TOK_RBRACE TOK_FROM TOK_STRING {
    driver.addImport($8); 
    $$ = std::make_unique<ImportStatementNode>($8, ImportType::Named, std::move($5), $2);
  }
  | TOK_IMPORT TOK_STRING {
    driver.addImport($2); 
    $$ = std::make_unique<ImportStatementNode>($2, ImportType::SideEffect);
  }
;

import_list:
  import_specifier { $$ = import_list{}; $$.push_back(std::move($1)); }
  | import_list TOK_COMMA import_specifier { $1.push_back(std::move($3)); $$ = std::move($1); }
;

import_specifier:
  TOK_IDENTIFIER { $$ = std::make_unique<ImportSymbol>($1); }
  | TOK_IDENTIFIER TOK_AS TOK_IDENTIFIER { $$ = std::make_unique<ImportSymbol>($1, $3); }
;

if_else_statement:
  if_statement  %prec LOWER_THAN_ELSE         { $$ = std::move($1); }
  | if_statement KW_ELSE statement            { $$ = std::move($1); $$->addElseBranch(std::move($3)); }
;

if_statement:
  KW_IF TOK_LPAREN expression TOK_RPAREN statement { $$ = std::make_unique<IfStatementNode>(std::move($3), std::move($5)); }
;

switch_statment:
  KW_SWITCH TOK_LPAREN switch_level_expression TOK_RPAREN TOK_LBRACE_COMPOUND case_blocks default_block TOK_RBRACE { 
    $$ = std::make_unique<SwitchStatementNode>(std::move($3));
    if ($6) {
      for (auto& block : *$6) {
        $$->pushBasicBlock(std::move(block));
      }
    }
    if ($7) {
      $$->pushBasicBlock(std::move($7));
    }
  }
;

case_blocks:
  case_block case_blocks  { $$ = std::move($2);  $$->push_back(std::move($1)); }
  | %empty                { $$ = std::make_unique<std::vector<std::unique_ptr<SwitchCaseBlockNode>>>(); }
;

case_block:
  KW_CASE switch_case_const_expression TOK_COLON statement KW_BREAK TOK_SEMICOLON { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::CASE, std::move($2), std::move($4), false, false); }
  | KW_CASE switch_case_const_expression TOK_COLON statement { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::CASE, std::move($2), std::move($4), false, true); }
  | KW_CASE switch_case_const_expression TOK_COLON KW_BREAK TOK_SEMICOLON { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::CASE, std::move($2), nullptr, true, false); }
  | KW_CASE switch_case_const_expression TOK_COLON  { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::CASE, std::move($2), nullptr, true, false); }
;

default_block:
  KW_DEFAULT TOK_COLON statement KW_BREAK TOK_SEMICOLON { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::DEFAULT, nullptr, std::move($3), false, false); }
  | KW_DEFAULT TOK_COLON statement  { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::DEFAULT, nullptr, std::move($3), false, false); }
  | KW_DEFAULT TOK_COLON KW_BREAK TOK_SEMICOLON { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::DEFAULT, nullptr, nullptr, false, false); }
  | KW_DEFAULT TOK_COLON  { $$ = std::make_unique<SwitchCaseBlockNode>(SwitchBlockType::DEFAULT, nullptr, nullptr, false, false); }
  | %empty  { $$ = nullptr; }
;

switch_level_expression:
  switch_case_const_expression { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::NONE, std::move($1), nullptr); }          
  | switch_case_const_expression TOK_PLUS switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::PLUS, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_MINUS switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::MINUS, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_MULTIPLY switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::MUL, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_BITWISE_XOR switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::BITWISE_XOR, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_BIT_AND switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::BIT_AND, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_BIT_OR switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::BIT_OR, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_LSHIFT switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::LSHIFT, std::move($1), std::move($3)); }
  | switch_case_const_expression TOK_RSHIFT switch_case_const_expression  { $$ = std::make_unique<SwitchLevelExpressionNode>(OperatorType::RSHIFT, std::move($1), std::move($3)); }
;

switch_case_const_expression: 
  TOK_INTEGER { $$ = std::make_unique<SwitchCaseExpressionNode>($1); }
  | TOK_CHAR { $$ = std::make_unique<SwitchCaseExpressionNode>($1); }
  | TOK_STRING { $$ = std::make_unique<SwitchCaseExpressionNode>($1); }
  | TOK_IDENTIFIER { $$ = std::make_unique<SwitchCaseExpressionNode>($1); }
  | TOK_IDENTIFIER TOK_DOUBLE_COLON TOK_IDENTIFIER { $$ = std::make_unique<SwitchCaseExpressionNode>(std::make_pair($1, $3)); }
;


loop_statement:
  KW_WHILE TOK_LPAREN expression TOK_RPAREN compound_statement {
    $$ = std::make_unique<ForLoopStatementNode>(
      std::unique_ptr<ExpressionNode>(static_cast<ExpressionNode*>($3.release())),
      std::unique_ptr<StatementListNode>(static_cast<StatementListNode*>($5.release()))
    );
  }
  | KW_FOR TOK_LPAREN loop_expression TOK_SEMICOLON loop_expression TOK_SEMICOLON loop_expression TOK_RPAREN compound_statement {
    $$ = std::make_unique<ForLoopStatementNode>(
      std::unique_ptr<ExpressionNode>(static_cast<ExpressionNode*>($5.release())),
      std::unique_ptr<StatementListNode>(static_cast<StatementListNode*>($9.release()))
    );
    $$->preLoopNode = std::move($3);
    $$->afterNode = std::move($7);
  }
;

loop_expression:
  %empty                      { $$ = nullptr; }
  | expression                { $$ = std::move($1); }
  | variable_declaration      { $$ = std::move($1); }
;

variable_declaration:
  type_specifier TOK_IDENTIFIER                              { $$ = std::make_unique<VariableDeclarationNode>($1, $2); }
  | type_specifier TOK_IDENTIFIER TOK_ASSIGN expression      { $$ = std::make_unique<VariableDeclarationNode>($1, $2, std::move($4)); }
;

return_statement: KW_RETURN expression { $$ = std::make_unique<StatementNode>(StatementType::RETURN_STATEMENTS, std::move($2)); };

throw_smt: KW_THROW expression { $$ = std::make_unique<StatementNode>(StatementType::THROW_STATEMENTS, std::move($2)); };

compound_statement: TOK_LBRACE_COMPOUND statements TOK_RBRACE { driver.setLexerState(LexerState::STATE_COMPOUND); $$ = std::move($2); };

method_call_expression:
  chained_dereference TOK_DOT function_call_expression             { $$ = std::make_unique<MethodCallNode>(std::move($3), std::move($1)); }
  | method_call_expression TOK_DOT function_call_expression        { $$ = std::make_unique<MethodCallNode>(std::move($3), std::move($1)); } 
;

variable_access: chained_dereference { $$ = std::move($1); };

chained_dereference:
  single_dereference { $$ = std::make_unique<ChainedDereferenceNode>(std::move($1)); }
  | chained_dereference TOK_DOT single_dereference { $1->addDereference(std::move($3)); $$ = std::move($1); }
;

single_dereference:
  identifier_expr { $$ = VariableDereferenceNode::createFromIdentifier(std::move($1)); }
  | array_access { $$ = VariableDereferenceNode::createFromArray(std::move($1)); }
;

array_access: TOK_IDENTIFIER TOK_LBRACKET expression TOK_RBRACKET { $$ = std::make_unique<ArrayAccessNode>($1, std::move($3)); } ;

qualified_prefix: TOK_IDENTIFIER TOK_DOUBLE_COLON { $$ = std::make_unique<QualifiedPrefixType>($1); };

qualified_entity: qualified_prefix TOK_IDENTIFIER  %prec TOK_DOUBLE_COLON { $$ = std::make_unique<QualifiedIdentifierNode>($2, std::move($1)); };

qualified_function_call: qualified_prefix TOK_IDENTIFIER TOK_LPAREN arguments_opt TOK_RPAREN { $$ = std::make_unique<QualifiedFunctionCallNode>($2, std::move($1), std::move($4)); };

identifier_expr:
  TOK_IDENTIFIER { $$ = std::make_unique<IdentifierNode>($1); }
  | KW_THIS     { $$ = std::make_unique<IdentifierNode>("this"); }
  | KW_SUPER    { $$ = std::make_unique<IdentifierNode>("super"); }
;

exp_list_opt: 
  exp_list { $$ = std::move($1); } 
  | %empty { $$ = std::make_unique<std::vector<std::unique_ptr<Node>>>(); } 
;

exp_list:
  constructor_assignment { 
    auto vec = std::make_unique<std::vector<std::unique_ptr<Node>>>(); 
    vec->push_back(std::move($1)); 
    $$ = std::move(vec);
  }
  | exp_list constructor_assignment {
    $1->push_back(std::move($2)); 
    $$ = std::move($1);
  }
;

constructor_assignment:
  KW_THIS TOK_DOT TOK_IDENTIFIER TOK_ASSIGN TOK_IDENTIFIER TOK_SEMICOLON {
    $$ = std::make_unique<ClazzConstructorExpressionNode>($3, $5);
  }
  | KW_THIS TOK_DOT TOK_IDENTIFIER TOK_ASSIGN object_creation_expression TOK_SEMICOLON {
    $$ = std::make_unique<ClazzConstructorExpressionNode>($3, std::move($5));
  }
;

parameters_opt:
  parameters { $$ = std::move($1); }
  | %empty { $$ = std::make_shared<std::vector<std::shared_ptr<Parameter>>>(); }
;

parameters:
  parameter {
    auto vec = std::make_shared<std::vector<std::shared_ptr<Parameter>>>();
    vec->push_back(std::move($1));
    $$ = std::move(vec);
  }
  | parameters TOK_COMMA parameter {
    $1->push_back(std::move($3));
    $$ = std::move($1);
  }
;

parameter: type_specifier TOK_IDENTIFIER { $$ = std::make_shared<Parameter>($1, $2); };

super_arguments_opt:
  super_arguments { $$ = std::move($1); }
  | %empty          { $$ = std::make_unique<std::vector<std::unique_ptr<PrimaryExpressionNode>>>(); } // or nullptr
;

super_arguments:
  primary_expression {
    auto vec = std::make_unique<std::vector<std::unique_ptr<PrimaryExpressionNode>>>();
    vec->push_back(std::move($1));
    $$ = std::move(vec);
  }
  | super_arguments TOK_COMMA primary_expression {
    $1->push_back(std::move($3));
    $$ = std::move($1);
  }
;

arguments_opt:
  arguments { $$ = std::move($1); }
  | %empty    { $$ = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>(); }
;

arguments:
  expression {
    auto args = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>();
    args->push_back(std::move($1));
    $$ = std::move(args);
  }
  | arguments TOK_COMMA expression {
    $1->push_back(std::move($3));
    $$ = std::move($1);
  }
;

access_modifier:
  KW_PUBLIC         { $$ = AccessModifierType::PUBLIC; }
  | KW_PRIVATE      { $$ = AccessModifierType::PRIVATE; }
  | KW_PROTECTED    { $$ = AccessModifierType::PROTECTED; }
  | KW_DEFAULT      { $$ = AccessModifierType::DEFAULT; }
;

type_arguments:
  type_argument   {  auto vec = std::make_shared<std::vector<std::shared_ptr<GenericArgumentType>>>(); vec->push_back(std::move($1)); $$ = vec; }
  | type_arguments TOK_COMMA type_argument  { $1->push_back(std::move($3)); $$ = $1; }
;

type_argument:
  type_specifier      { $$ = std::make_shared<GenericArgumentType>($1); }
  | TOK_INTEGER       { $$ = std::make_shared<GenericArgumentType>($1); }
;

type_specifier:
  simple_type     { $$ = $1; }
  | generic_type  { $$ = $1; }
;

generic_type:
  TOK_IDENTIFIER TOK_LESS_THAN type_arguments TOK_GREATER_THAN %prec LOWER_THAN_GENERIC {
    try {
      TypeNestingLimiter limiter(driver.type_nesting_depth, driver.MAX_NESTING_DEPTH);
      $$ = createGenericVariableType($1, std::move($3));
    } catch (const std::exception& e) {
      yyerror(e.what());
      YYABORT;
    }
  }
;

simple_type:
    KW_BYTE                  { $$ = createVariableType(DataType::BYTE, "byte"); }
  | KW_SHORT                 { $$ = createVariableType(DataType::SHORT, "short"); }
  | KW_INT                   { $$ = createVariableType(DataType::INT, "int"); }
  | KW_LONG                  { $$ = createVariableType(DataType::LONG, "long"); }
  | KW_FLOAT                 { $$ = createVariableType(DataType::FLOAT, "float"); }
  | KW_DOUBLE                { $$ = createVariableType(DataType::DOUBLE, "double"); }
  | KW_BOOLEAN               { $$ = createVariableType(DataType::BOOLEAN, "boolean"); }
  | KW_CHAR                  { $$ = createVariableType(DataType::CHAR, "char"); }
  | KW_STRING                { $$ = createVariableType(DataType::STRING, "string"); }
  | KW_VOID                  { $$ = createVariableType(DataType::VOID, "void"); }
  | KW_AUTO                  { $$ = createVariableType(DataType::AUTO, "auto"); } 
  | TOK_IDENTIFIER           { $$ = createVariableType(DataType::OTHER, $1); }
;

literal:
  literal_value                        { $$ = std::move($1); }
  | literal_initializer                { $$ = std::move($1); }
;

literal_initializer:
  literal_list                       { $$ = std::make_unique<CollectionInitializerNode>(std::move($1)); }
  | literal_map                      { $$ = std::make_unique<CollectionInitializerNode>(std::move($1)); }
;

literal_value:
  TOK_BYTE            { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_SHORT         { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_INTEGER       { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_LONG          { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_FLOAT         { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_DOUBLE        { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_BOOLEAN       { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_CHAR          { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_STRING        { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_DATE          { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_DATE_TIME     { $$ = std::make_unique<LiteralNode>($1); }
  | TOK_NULL_PTR      { $$ = std::make_unique<LiteralNode>(nullptr); }
;

literal_list:
  TOK_LBRACE_LITERAL literal_list_items TOK_RBRACE {
    try {
      LiteralNestingLimiter limiter(driver.literal_nesting_depth, driver.MAX_NESTING_DEPTH);
      driver.setLexerState(LexerState::STATE_EXPR);
      auto listNode = std::make_unique<LiteralListNode>();
      for (auto& item : $2) listNode->addValue(std::move(item));
      $$ = std::move(listNode);
    } catch (const std::exception& e) {
      yyerror(e.what());
      YYABORT;
    }
  }
;

literal_list_items:
  literal { $$ = literal_list_ast_nodes{}; $$.push_back(std::move($1));  }
  | literal_list_items TOK_COMMA literal { $1.push_back(std::move($3)); $$ = std::move($1); }
;

literal_map:
  TOK_LBRACE_LITERAL literal_pairs TOK_RBRACE {
    driver.setLexerState(LexerState::STATE_EXPR);
    auto mapNode = std::make_unique<LiteralMapNode>();
    for (auto& pair : $2) { mapNode->addPair(std::move(pair)); }
    $$ = std::move(mapNode);
  }
;

literal_pairs:
  literal_pair { $$ = literal_pair_ast_nodes{}; $$.push_back(std::move($1));}
  | literal_pairs TOK_COMMA literal_pair { $1.push_back(std::move($3)); $$ = std::move($1); }
;

literal_pair:
  literal_value TOK_COLON literal { $$ = std::make_unique<LiteralPairNode>(std::move($1), std::move($3)); }
;

%%

void LynxLang::Parser::error(const LynxLang::location& loc, const std::string& message) {
  std::cerr << "Error at location " << loc << ": " << message << std::endl;
}




