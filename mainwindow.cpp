#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QSpinBox>
#include <QFont>
#include <QDebug>
#include <iostream>
#include <QPalette>
#include <map>
#include <QSize>
#include <set>

int numberVariables = 3;
int rows = (1 << numberVariables), columns = (1 << numberVariables);
QString function = "01000111";
const int minimumSizeHeight = 800, minimumSizeWidth = 1200;
const QString alph = "ABCDEF";
bool flag = 0;
QLineEdit* numberIn2;
QFont mainFont("Arial", 10, QFont::Bold);
std::vector<QString> ans;
std::map<int, int> lg2 = {{64, 6}, {32, 5}, {16, 4}, {8, 3}, {4, 2}, {2, 1}};
std::vector<std::vector<int> > used;


bool only10(QString s) {
    for (auto c: s) {
        if (c.cell() != '1' && c.cell() != '0') {
            return false;
        }
    }
    return true;
}


void gen(int gened, int all, QString cur, QStringList &t, int needed, int last) {
    if (gened == needed) {
        t.push_back(cur);
        return;
    }
    for (int i = last; i < all; i++) {
        gen(gened + 1, all, cur + alph[i], t, needed, i + 1);
    }
}

QStringList generateHorizontalHeaders(int x) {
    QStringList ans;
    for (int i = 1; i <= x; i++) {
        gen(0, x, QString(""), ans, i, 0);
    }
    return ans;
}

void gen1(int gened, int needed, QString cur) {
    if (gened == needed) {
        ans.push_back(cur);
        return;
    }
    for (QString c: {"0", "1"}) {
        gen1(gened + 1, needed, cur + c);
    }
}

void gen2(int gened, int all, QString cur, std::vector<QString> &t, int needed, int last, QString &alp) {
    if (gened == needed) {
        t.push_back(cur);
        return;
    }
    for (int i = last; i < all; i++) {
        gen2(gened + 1, all, cur + alp[i], t, needed, i + 1, alp);
    }
}

void MainWindow::updateTable() {
    QTableWidget* newMainTable = createTable();
    mainLayout->replaceWidget(mainTable, newMainTable);
    delete mainTable;
    mainTable = newMainTable;
}

std::vector<std::vector<QString> > getTableFilling() {
    ans = {};
    gen1(0, numberVariables, "");
    std::vector<std::vector<QString> > ft;
    for (QString elem: ans) {
        std::vector<QString> currow;
        for (int i = 1; i <= numberVariables; i++) {
            gen2(0, numberVariables, QString(""), currow, i, 0, elem);
        }
        ft.push_back(currow);
    }
    return ft;
}

QTableWidget* MainWindow::createTable() {
    rows = (1 << numberVariables);
    columns = (1 << numberVariables);
    QTableWidget* tn = new QTableWidget(rows, columns);
    QStringList verticalHeaders;
    for (int row = 0; row < rows; ++row) {
        verticalHeaders.push_back(QString(""));
    }
    tn->setVerticalHeaderLabels(verticalHeaders);
    QStringList horizontalHeaders = generateHorizontalHeaders(numberVariables);
    horizontalHeaders.push_front(QString("F"));
    tn->setHorizontalHeaderLabels(horizontalHeaders);
    for (int i = 0; i < columns; i++) {
        QTableWidgetItem* header = tn->horizontalHeaderItem(i);
        header->setFont(mainFont);
        header->setTextAlignment(Qt::AlignCenter);
        header->setSizeHint(QSize(100, 30));
    }
    for (int i = 0; i < rows; i++) {
        QTableWidgetItem* changeable = new QTableWidgetItem(QString(function[i]));
        changeable->setTextAlignment(Qt::AlignCenter);
        tn->setItem(i, 0, changeable);
        for (int j = 1; j < columns; j++) {
            QTableWidgetItem* current = new QTableWidgetItem("");
            current->setFlags(current->flags() ^ Qt::ItemIsEditable);
            current->setTextAlignment(Qt::AlignCenter);
            tn->setItem(i, j, current);
        }
    }
    tn->setFont(mainFont);
    tn->resizeRowsToContents();
    tn->resizeColumnsToContents();
    tn->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    tn->adjustSize();
    tn->setSelectionMode(QAbstractItemView::NoSelection);
    tn->setFocusPolicy(Qt::NoFocus);
    std::vector<std::vector<QString> > tb = getTableFilling();
    for (int i = 0; i < tb.size(); i++) {
        for (int j = 1; j <= tb[i].size(); j++) {
            tn->item(i, j)->setText(tb[i][j - 1]);
        }
    }
    connect(tn, SIGNAL(cellChanged(int, int)), this, SLOT(functionUpdated(int, int)));
    return tn;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(minimumSizeWidth, minimumSizeHeight);
    setWindowTitle(QString("MDNF"));
    mainTable = createTable();
    mainTable->setFont(mainFont);
    mainLayout = new QVBoxLayout(this);
    QGridLayout* up = new QGridLayout(this);
    QTextBrowser* answer = new QTextBrowser(this);
    answer->setText(QString("Your answer is: "));
    answer->setFont(mainFont);
    up->addWidget(mainTable);
    up->addWidget(answer);
    mainLayout->addLayout(up);
    centralWidget()->setLayout(mainLayout);

    numberIn2 = new QLineEdit(this);
    numberIn2->setMaxLength(64);
    numberIn2->setFixedSize(200, 30);
    numberIn2->setText(function);
    numberIn2->setStyleSheet("color: green");
    numberIn2->setFont(mainFont);
    connect(numberIn2, SIGNAL(textEdited(const QString &)), this, SLOT(binaryStringChanged(const QString &)));

    QLineEdit* numberBits = new QLineEdit(this);
    numberBits->setMaxLength(1);
    numberBits->setFixedSize(200, 30);
    numberBits->setText("3");
    numberBits->setFont(mainFont);
    connect(numberBits, SIGNAL(textEdited(const QString &)), this, SLOT(numberBitsChanged(const QString &)));

    QLabel* label = new QLabel(this);
    label->setText("Function in binary format and number of variables:");
    label->setFont(mainFont);
    label->setFixedSize(400, 30);
    QHBoxLayout* low = new QHBoxLayout(this);
    low->addWidget(label);
    low->addWidget(numberIn2);
    low->addWidget(numberBits);
    mainLayout->addLayout(low);
}

void MainWindow::binaryStringChanged(const QString &s) {
    if (s.length() != rows || !only10(s)) {
        numberIn2->setStyleSheet("color: red");
        return;
    }
    for (int i = 0; i < rows; i++) {
        auto itm = new QTableWidgetItem;
        itm->setFont(mainFont);
        itm->setTextAlignment(Qt::AlignCenter);
        itm->setText(QString(s[i]));
        mainTable->setItem(i, 0, itm);
    }
    numberIn2->setStyleSheet("color: green");
    function = s;
}

void MainWindow::numberBitsChanged(const QString &s) {
    if (s == "") return;
    int k = s[0].cell() - '0';
    if (k > 6 || k < 1) return;
    numberVariables = k;
    if (numberVariables != lg2[function.length()])
        numberIn2->setStyleSheet("color: red");
    else
        numberIn2->setStyleSheet("color: green");
    updateTable();
}

void MainWindow::functionUpdated(int row, int column) {
    QString s = mainTable->item(row, column)->text();
    if (s.length() > 1) return;
    int k = s[0].cell() - '0';
    if (k != 0 && k != 1) return;
    function[row] = s[0].cell();
    flag = 1;
    numberIn2->setText(function);
}



MainWindow::~MainWindow()
{
    delete ui;
}
