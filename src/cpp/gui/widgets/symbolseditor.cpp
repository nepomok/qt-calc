#include "gui/widgets/symbolseditor.hpp"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QMessageBox>

#include "numberformat.hpp"

std::map<QString, QString> convertMap(std::map<std::string, ArithmeticType> map) {
    std::map<QString, QString> ret;
    for (auto &p : map) {
        ret[QString(p.first.c_str())] = NumberFormat::toDecimal(p.second).c_str();
    }
    return ret;
}

SymbolsEditor::SymbolsEditor(QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout(this));

    auto *tabs = new QTabWidget(this);

    variablesEditor = new NamedValueEditor(tabs);
    constantsEditor = new NamedValueEditor(tabs);
    functionsEditor = new FunctionsEditor(tabs);
    scriptsEditor = new ScriptsEditor(tabs);

    tabs->addTab(variablesEditor, "Variables");
    tabs->addTab(constantsEditor, "Constants");
    tabs->addTab(functionsEditor, "Functions");
    tabs->addTab(scriptsEditor, "Scripts");

    layout()->addWidget(tabs);

    connect(variablesEditor,
            SIGNAL(onNamedValueAdded(const QString &, const QString &)),
            this,
            SLOT(onVariableAdded(const QString &, const QString &)));
    connect(variablesEditor,
            SIGNAL(onNameChanged(const QString &, const QString &)),
            this,
            SLOT(onVariableNameChanged(const QString &, const QString &)));
    connect(variablesEditor,
            SIGNAL(onValueChanged(const QString &, const QString &)),
            this,
            SLOT(onVariableValueChanged(const QString &, const QString &)));

    connect(constantsEditor,
            SIGNAL(onNamedValueAdded(const QString &, const QString &)),
            this,
            SLOT(onConstantAdded(const QString &, const QString &)));
    connect(constantsEditor,
            SIGNAL(onNameChanged(const QString &, const QString &)),
            this,
            SLOT(onConstantNameChanged(const QString &, const QString &)));
    connect(constantsEditor,
            SIGNAL(onValueChanged(const QString &, const QString &)),
            this,
            SLOT(onConstantValueChanged(const QString &, const QString &)));
}

void SymbolsEditor::setSymbols(const SymbolTable &symtable) {
    symbolTable = symtable;

    variablesEditor->setValues(convertMap(symtable.getVariables()));
    constantsEditor->setValues(convertMap(symtable.getConstants()));
    functionsEditor->setFunctions(symtable.getFunctions());
    scriptsEditor->setScripts(symtable.getScripts());
}

void SymbolsEditor::onVariableAdded(const QString &name, const QString &value) {
    if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else {
        ArithmeticType valueConverted;
        try {
            valueConverted = NumberFormat::fromDecimal(value.toStdString());
        }
        catch (const std::exception &e) {
            valueConverted = 0;
            QMessageBox::warning(this, "Error", "Error");
        }
        symbolTable.setVariable(name.toStdString(), valueConverted);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onVariableNameChanged(const QString &originalName, const QString &name) {
    if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else {
        ArithmeticType value = symbolTable.getVariables().at(originalName.toStdString());
        symbolTable.remove(originalName.toStdString());
        symbolTable.setVariable(name.toStdString(), value);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onVariableValueChanged(const QString &name, const QString &value) {
    ArithmeticType originalValue = symbolTable.getVariables().at(name.toStdString());
    ArithmeticType newValue;
    try {
        newValue = NumberFormat::fromDecimal(value.toStdString());
    }
    catch (const std::exception &e) {
        newValue = originalValue;
        QMessageBox::warning(this, "Error", "Error");
    }
    symbolTable.setVariable(name.toStdString(), newValue);
    emit onSymbolsChanged(symbolTable);
}

void SymbolsEditor::onConstantAdded(const QString &name, const QString &value) {
    if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
    } else {
        ArithmeticType valueConverted;
        try {
            valueConverted = NumberFormat::fromDecimal(value.toStdString());
        }
        catch (const std::exception &e) {
            valueConverted = 0;
            QMessageBox::warning(this, "Error", "Error");
        }
        symbolTable.setConstant(name.toStdString(), valueConverted);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onConstantNameChanged(const QString &originalName, const QString &name) {
    if (symbolTable.hasVariable(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasConstant(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasFunction(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else if (symbolTable.hasScript(name.toStdString())) {
        QMessageBox::warning(this, "Error", "Error");
        variablesEditor->setValues(convertMap(symbolTable.getVariables()));
    } else {
        ArithmeticType value = symbolTable.getConstants().at(originalName.toStdString());
        symbolTable.remove(originalName.toStdString());
        symbolTable.setConstant(name.toStdString(), value);
        emit onSymbolsChanged(symbolTable);
    }
}

void SymbolsEditor::onConstantValueChanged(const QString &name, const QString &value) {
    ArithmeticType originalValue = symbolTable.getConstants().at(name.toStdString());
    ArithmeticType newValue;
    try {
        newValue = NumberFormat::fromDecimal(value.toStdString());
    }
    catch (const std::exception &e) {
        newValue = originalValue;
        QMessageBox::warning(this, "Error", "Error");
    }
    symbolTable.setConstant(name.toStdString(), newValue);
    emit onSymbolsChanged(symbolTable);
}
