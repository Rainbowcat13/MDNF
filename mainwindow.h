#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTableWidget* mainTable;
    QVBoxLayout* mainLayout;
    QLineEdit* numberIn2;

public slots:
    void binaryStringChanged(const QString &);
    void numberBitsChanged(const QString &);
    void functionUpdated(int, int);
    QTableWidget* createTable();
    void updateTable();
};

#endif // MAINWINDOW_H
