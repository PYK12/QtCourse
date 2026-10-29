#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void btnNumClicked();
    void binaryOperatorClicked();
    void on_btnPeriod_clicked();
    void on_btnEqual_clicked();
    void on_btnClear_clicked();
    void on_btnDel_clicked();
    // 新增：所有功能键共用这一个槽
    void funcBtnClicked();

    void on_btnPercentage_clicked();

    void on_btnClearAll_clicked();

    void on_btnInverse_clicked();

    void on_btnSquare_clicked();

    void on_btnSqrt_clicked();

private:
    Ui::MainWindow *ui;
    QString operand;
    QQueue<QString> operands;
    QQueue<QString> opcodes;
    QString calculation(QString a, QString op, QString b, bool *ok);
    void refreshStatus();
};
#endif // MAINWINDOW_H
