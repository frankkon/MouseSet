#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_timer();
    void on_timerLcd();

private:
    Ui::MainWindow *ui;

    QPoint pos0;
    QTimer m_timer;
    QTimer m_timerLcd;
    bool m_bStartFlag;
};
#endif // MAINWINDOW_H
