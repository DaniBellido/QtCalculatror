#include "calculator.h"
#include "ui_calculator.h"
#include <QRegularExpression>

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));

     // Declare an array of QPushButton pointers to hold number buttons
    QPushButton *numButtons[10];

    // Loop to find and assign number buttons to the array
    for(int i = 0; i < 10; ++i)
    {
        // Generate the button name using the loop index
        QString butName = "Button" + QString::number(i);

        // Find the child widget with the generated button name and assign it to the array
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);

        // Connects each number button to the NumPressed slot when released
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Substract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));

    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));

    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearButtonPressed()));

}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed()
{
    // Get the sender button and its text
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    // Get the current text of the display
    QString displayVal = ui->Display->text();

    // Check if the display value is zero or not
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0))
    {
        // If display value is zero, set it to the button's text
        ui->Display->setText(butVal);
    }
    else
    {
        // If display value is not zero, concatenate button's text to the current display value
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();

         // Set the new concatenated value to the display with scientific notation ('g') and precision 16
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed()
{
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    if(QString::compare(butVal, "÷", Qt::CaseInsensitive) == 0)
    {
        divTrigger = true;
    }
    else if(QString::compare(butVal, "x", Qt::CaseInsensitive) == 0)
    {
        multTrigger = true;
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0)
    {
        addTrigger = true;
    }
    else
    {
        subTrigger = true;
    }

    ui->Display->setText("");
}

void Calculator::EqualButtonPressed()
{

    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if(addTrigger || subTrigger || multTrigger || divTrigger)
    {
        double solution = 0.0;

        if(addTrigger)
        {
            solution = calcVal + dblDisplayVal;
        }
        else if(subTrigger)
        {
            solution = calcVal - dblDisplayVal;
        }
        else if(multTrigger)
        {
            solution = calcVal * dblDisplayVal;
        }
        else
        {
            solution = calcVal / dblDisplayVal;
        }

        ui->Display->setText(QString::number(solution));
    }



    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
}

void Calculator::ChangeNumberSign()
{
    QString displayVal = ui->Display->text();
    QRegularExpression reg("[-]?[0-9.]*");

    QRegularExpressionMatch match = reg.match(displayVal);
    if(match.hasMatch())
    {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }

}

void Calculator::ClearButtonPressed()
{
    calcVal = 0.0;
    ui->Display->setText(QString::number(calcVal));

}
