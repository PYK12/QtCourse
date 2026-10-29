#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cmath>
#include <QPushButton>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    operand = "0";
    ui->display->setText(operand);

    this->setFocusPolicy(Qt::StrongFocus);

    this->setStyleSheet(R"(

    QMainWindow{
        background-color: #f0f0f0;
    }
    QLabel#display{
        background-color: #ffffff;
        color: #000000;
        font-size:36px;
        font-family:Microsoft YaHei;
        padding:24px;
        border-radius:12px;
        qproperty-alignment: AlignRight;
    }

    QPushButton#btnNum0,QPushButton#btnNum1,QPushButton#btnNum2,QPushButton#btnNum3,
    QPushButton#btnNum4,QPushButton#btnNum5,QPushButton#btnNum6,QPushButton#btnNum7,
    QPushButton#btnNum8,QPushButton#btnNum9,QPushButton#btnPeriod
    {
        background-color: #ffffff;
        color: #000000;
        border:1px solid #cccccc;
        border-radius:8px;
        font-size:20px;
        font-family:Microsoft YaHei;
        min-height:45px;
    }
    QPushButton#btnNum0:hover,QPushButton#btnNum1:hover,QPushButton#btnNum2:hover,QPushButton#btnNum3:hover,
    QPushButton#btnNum4:hover,QPushButton#btnNum5:hover,QPushButton#btnNum6:hover,QPushButton#btnNum7:hover,
    QPushButton#btnNum8:hover,QPushButton#btnNum9:hover,QPushButton#btnPeriod:hover
    {
        background-color:#e6e6e6;
    }

    QPushButton#btnPercentage,QPushButton#btnCE,QPushButton#btnClear,QPushButton#btnClearAll,QPushButton#btnDel,
    QPushButton#btnInverse,QPushButton#btnSquare,QPushButton#btnSqrt,
    QPushButton#btnDivide,QPushButton#btnMultiple,QPushButton#btnMinus,QPushButton#btnPlus
    {
        background-color:#e2e2e2;
        color:black;
        border:1px solid #cccccc;
        border-radius:8px;
        font-size:20px;
        font-family:Microsoft YaHei;
        min-height:45px;
    }
    QPushButton#btnPercentage:hover,QPushButton#btnCE:hover,QPushButton#btnClear:hover,QPushButton#btnClearAll:hover,QPushButton#btnDel:hover,
    QPushButton#btnInverse:hover,QPushButton#btnSquare:hover,QPushButton#btnSqrt:hover,
    QPushButton#btnDivide:hover,QPushButton#btnMultiple:hover,QPushButton#btnMinus:hover,QPushButton#btnPlus:hover
    {
        background-color:#d3d3d3;
    }

    QPushButton#btnEqual
    {
        background-color:#4285f4;
        color:white;
        border:1px solid #cccccc;
        border-radius:8px;
        font-size:20px;
        font-family:Microsoft YaHei;
        min-height:45px;
    }
    QPushButton#btnEqual:hover
    {
        background-color:#336edb;
    }
    )");

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

    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));

    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(binaryOperatorClicked()));
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString text = event->text();

    if (key >= Qt::Key_0 && key <= Qt::Key_9)
    {
        operand = (operand == "0") ? text : operand + text;
        ui->display->setText(operand);
        refreshStatus();
    }

    else if (key == Qt::Key_Period)
    {
        on_btnPeriod_clicked();
    }

    else if (key == Qt::Key_Plus)
    {
        ui->btnPlus->click();
    }
    else if (key == Qt::Key_Minus)
    {
        ui->btnMinus->click();
    }
    else if (key == Qt::Key_Asterisk)
    {
        ui->btnMultiple->click();
    }
    else if (key == Qt::Key_Slash)
    {
        ui->btnDivide->click();
    }

    else if (key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        on_btnEqual_clicked();
    }

    else if (key == Qt::Key_Backspace)
    {
        on_btnDel_clicked();
    }

    else if (key == Qt::Key_Escape)
    {
        ui->btnClear->click();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}
