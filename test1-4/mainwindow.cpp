#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cmath>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    operand = "0";
    ui->display->setText(operand);

    // ==========数字按键绑定==========
    connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    // ==========四则运算符==========
    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));

    // =========重点！！！=========
    // 你在Qt Designer里，选中下面这些按钮，右键【转到槽】，选clicked()
    // 或者你手动在这里写connect，**按钮名字以你ui里实际objectName为准**
    // 下面这几个按钮（%、CE、C、退格、1/x、x²、²√x、+/-）全部绑定到 funcBtnClicked
    // 举个例子，如果你的%按钮叫pushButton，就写 connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::funcBtnClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshStatus()
{
    int showOperandCount = operands.size();
    if(operand != "0"){
        showOperandCount += 1;
    }
    ui->statusbar->showMessage(QString("operands:%1,opcode:%2").arg(showOperandCount).arg(opcodes.size()));
}

QString MainWindow::calculation(QString a, QString op, QString b, bool *ok)
{
    *ok = true;
    double op1 = a.toDouble();
    double op2 = b.toDouble();
    double res = 0;
    if (op == "+")
    {
        res = op1 + op2;
    }
    else if (op == "-")
    {
        res = op1 - op2;
    }
    else if (op == "×")
    {
        res = op1 * op2;
    }
    else if (op == "÷")
    {
        if (qFuzzyCompare(op2, 0.0))
        {
            *ok = false;
            return "0";
        }
        res = op1 / op2;
    }
    return QString::number(res);
}

void MainWindow::btnNumClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString numText = btn->text();
    if (operand == "0")
    {
        operand = numText;
    }
    else
    {
        operand += numText;
    }
    ui->display->setText(operand);
    refreshStatus();
}

void MainWindow::binaryOperatorClicked()
{
    QString op = qobject_cast<QPushButton *>(sender())->text();
    operands.push_back(operand);
    opcodes.push_back(op);
    operand = "0";
    ui->display->setText(operand);
    refreshStatus();
}

void MainWindow::on_btnPeriod_clicked()
{
    if (!operand.contains("."))
    {
        operand += ".";
    }
    ui->display->setText(operand);
    refreshStatus();
}

void MainWindow::on_btnEqual_clicked()
{
    operands.push_back(operand);
    bool ok = true;
    QString res = operands.dequeue();
    while(!operands.isEmpty() && !opcodes.isEmpty())
    {
        QString op = opcodes.dequeue();
        QString b = operands.dequeue();
        res = calculation(res, op, b, &ok);
        if(!ok)
        {
            operand = "0";
            operands.clear();
            opcodes.clear();
            ui->display->setText(operand);
            ui->statusbar->showMessage("错误，除零");
            return;
        }
    }
    operand = res;
    ui->display->setText(operand);
    operands.clear();
    opcodes.clear();
    refreshStatus();
}

void MainWindow::on_btnClear_clicked()
{
    operand = "0";
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
    refreshStatus();
}

void MainWindow::on_btnDel_clicked()
{
    if(operand.length() <= 1)
    {
        operand = "0";
    }
    else
    {
        operand.chop(1);
    }
    ui->display->setText(operand);
    refreshStatus();
}

// ==========统一功能按键槽函数！==========
void MainWindow::funcBtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    QString text = btn->text();
    double val = operand.toDouble();

    if(text == "CE")
    {
        // CE：清空当前输入，保留运算队列
        operand = "0";
    }
    else if(text == "C")
    {
        // C：全部清空，和Clear一样
        operand = "0";
        operands.clear();
        opcodes.clear();
    }
    else if(text == "⌫") // 你的退格按钮文字，看你ui上的图案
    {
        if(operand.length() <= 1)
            operand = "0";
        else
            operand.chop(1);
    }
    else if(text == "1/x")
    {
        if(qFuzzyCompare(val,0.0))
        {
            ui->statusbar->showMessage("错误，除零");
            return;
        }
        val = 1.0 / val;
        operand = QString::number(val);
    }
    else if(text == "x²")
    {
        val = val * val;
        operand = QString::number(val);
    }
    else if(text == "²√x")
    {
        if(val < 0)
        {
            ui->statusbar->showMessage("错误，负数不能开方");
            return;
        }
        val = sqrt(val);
        operand = QString::number(val);
    }
    else if(text == "+/-")
    {
        val = -val;
        operand = QString::number(val);
    }
    else if(text == "%")
    {
        val = val / 100.0;
        operand = QString::number(val);
    }

    ui->display->setText(operand);
    refreshStatus();
}

void MainWindow::on_btnPercentage_clicked()
{
    funcBtnClicked();
}


void MainWindow::on_btnClearAll_clicked()
{
    funcBtnClicked();
}


void MainWindow::on_btnInverse_clicked()
{
    funcBtnClicked();
}


void MainWindow::on_btnSquare_clicked()
{
    funcBtnClicked();
}


void MainWindow::on_btnSqrt_clicked()
{
    funcBtnClicked();
}

