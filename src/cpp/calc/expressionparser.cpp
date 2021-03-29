#include "calc/expressionparser.hpp"

ExpressionParser::ExpressionParser() = default;

ExpressionParser::~ExpressionParser() = default;

/**
 * Evaluate the arithmetic expression using the defined symbol table.
 *
 * @param expr The mathematical expression which may contain symbols defined in the table.
 * @param symbolTable The symbol table to use when evaluating the expression.
 *
 * @return The value of the expression.
 */
ArithmeticType ExpressionParser::evaluate(const std::string &expr, SymbolTable &symbolTable) {
    exprtk::function_compositor<ArithmeticType> compositor;
    exprtk::symbol_table<ArithmeticType> symbols = compositor.symbol_table();

    int varArgScriptCount = 0;
    int scriptCount = 0;
    for (auto &v : symbolTable.getScripts()) {
        if (v.second.enableArguments)
            varArgScriptCount++;
        else
            scriptCount++;
    }

    //Use vectors with fixed size to store the function objects as the symbol table itself only stores references.
    int varArgScriptIndex = 0;
    std::vector<ScriptVarArgFunction<ArithmeticType>> varArgScriptFunctions;
    varArgScriptFunctions.resize(varArgScriptCount);

    int scriptIndex = 0;
    std::vector<ScriptFunction<ArithmeticType>> scriptFunctions;
    scriptFunctions.resize(scriptCount);

    for (auto &v : symbolTable.getScripts()) {
        if (v.second.enableArguments) {
            int index = varArgScriptIndex++;
            assert(index < varArgScriptCount);
            varArgScriptFunctions.at(index) = ScriptVarArgFunction<ArithmeticType>(pythonParser, v.second.expression);
            symbols.add_function(v.first, varArgScriptFunctions.at(index));
        } else {
            int index = scriptIndex++;
            assert(index < scriptCount);
            scriptFunctions.at(index) = ScriptFunction<ArithmeticType>(pythonParser, v.second.expression);
            symbols.add_function(v.first, scriptFunctions.at(index));
        }
    }

    assert(varArgScriptIndex == varArgScriptCount);
    assert(scriptIndex == scriptCount);

    for (auto &v : symbolTable.getFunctions()) {
        switch (v.second.argumentNames.size()) {
            case 0:
                compositor.add(
                        typename exprtk::function_compositor<ArithmeticType>::function(v.first, v.second.expression));
                break;
            case 1:
                compositor.add(typename exprtk::function_compositor<ArithmeticType>::function(v.first,
                                                                                              v.second.expression,
                                                                                              v.second.argumentNames[0]));
                break;
            case 2:
                compositor.add(
                        typename exprtk::function_compositor<ArithmeticType>::function(v.first,
                                                                                       v.second.expression,
                                                                                       v.second.argumentNames[0],
                                                                                       v.second.argumentNames[1]));
                break;
            case 3:
                compositor.add(
                        typename exprtk::function_compositor<ArithmeticType>::function(v.first,
                                                                                       v.second.expression,
                                                                                       v.second.argumentNames[0],
                                                                                       v.second.argumentNames[1],
                                                                                       v.second.argumentNames[2]));
                break;
            case 4:
                compositor.add(
                        typename exprtk::function_compositor<ArithmeticType>::function(v.first,
                                                                                       v.second.expression,
                                                                                       v.second.argumentNames[0],
                                                                                       v.second.argumentNames[1],
                                                                                       v.second.argumentNames[2],
                                                                                       v.second.argumentNames[3]));
                break;
            case 5:
                compositor.add(
                        typename exprtk::function_compositor<ArithmeticType>::function(v.first,
                                                                                       v.second.expression,
                                                                                       v.second.argumentNames[0],
                                                                                       v.second.argumentNames[1],
                                                                                       v.second.argumentNames[2],
                                                                                       v.second.argumentNames[3],
                                                                                       v.second.argumentNames[4]));
                break;
            default:
                throw std::runtime_error("Too many function argumentNames");
        }
    }

    for (auto &constant : symbolTable.getConstants()) {
        symbols.add_constant(constant.first, constant.second);
    }

    std::map<std::string, ArithmeticType> variables = symbolTable.getVariables();
    for (auto &variable : variables) {
        symbols.add_variable(variable.first, variable.second);
    }

    exprtk::expression<ArithmeticType> expression;
    expression.register_symbol_table(symbols);

    if (parser.compile(expr, expression)) {
        ArithmeticType ret = expression.value();
        for (auto &v : variables) {
            symbolTable.setVariable(v.first, v.second);
        }
        return ret;
    } else {
        throw std::runtime_error(parser.error());
    }
}

ArithmeticType ExpressionParser::evaluate(const std::string &expr) {
    exprtk::expression<ArithmeticType> expression;

    if (parser.compile(expr, expression)) {
        return expression.value();
    } else {
        throw std::runtime_error("Failed to compile expression");
    }
}