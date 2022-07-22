#include "library.h"
#include "ui_library.h"
#include <iostream>

QString globusername;
int DB_NUM_NEPTUN_CODE = 0;
int DB_NUM_FIRST_NAME = 1;
int DB_NUM_LAST_NAME = 2;
int DB_NUM_USERNAME = 3;
int DB_NUM_PASSWORD = 4;


Library::Library(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Library)
{
    ui->setupUi(this);

    mydb = QSqlDatabase::addDatabase("QODBC");
    QString connectionString = "DRIVER={Oracle in OraDB19Home2};UID=admin;PWD=jq6G4MriRlJ879onnXGj;DBQ=db.chsluornur3n.eu-central-1.rds.amazonaws.com:1521/DATABASE_A;";
    std::cout << connectionString.toStdString() << std::endl;
    mydb.setDatabaseName(connectionString);

    bool dbcheck = false;
    if(mydb.open())
    {
        std::cout << "Connected to the Database" << std::endl;
        dbcheck = true;
    }
    else
        std::cout << "Failed to Connect to Database" << std::endl;
    std::cout << mydb.lastError().text().toStdString() << std::endl;

    if (!dbcheck)
        QMessageBox::warning(
            this,
            tr("Database"),
            tr("Warning: Unable to connect to database. Please restart application") );

    mydb.close();
    connect(ui->Login, SIGNAL(pressed()), this, SLOT(Login()));
    connect(ui->Register, SIGNAL(pressed()), this, SLOT(Register()));
}

Library::~Library()
{
    delete ui;
}

int Library::Login()
{
    std::cout << "In Login function!" << std::endl;
    QString username = ui->L_Username->text();
    QString password = ui->L_Password->text();
    QString dbusername;
    QString dbpassword;
    bool executed = false;

    bool dbcheck = false;
    if(mydb.open())
    {
        std::cout << "Connected to the Database" << std::endl;
        dbcheck = true;
    }
    else
        std::cout << "Failed to Connect to Database" << std::endl;

    if (!dbcheck)
    {
        QMessageBox::warning(
            this,
            tr("Database"),
            tr("Warning: Unable to connect to database. Please restart application") );
        return 1;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = ?");
    query.addBindValue(username);
    executed = query.exec();

    if (!query.next())
    {
        QMessageBox::warning(this, "Error: ", "No such username exists!");
    }

    dbusername = query.value(DB_NUM_USERNAME).toString();
    dbpassword = query.value(DB_NUM_PASSWORD).toString();

    std::cout << dbusername.toStdString() << ", " << dbpassword.toStdString() << std::endl;

    if (username == dbusername)
    {
        if (password == dbpassword)
        {
            QMessageBox::information(this, tr("Login"), tr("Login Successful! You are now logged in!"));
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Login"), tr("The username does not exist!"));
        return 1;
    }

    if (dbpassword == password)
        QMessageBox::warning(this, "Wrong Password", "The password is incorrect!");
    else
        globusername = dbusername;

    mydb.close();
    return 0;
}

int Library::Register()
{
    std::cout << "In Register Function!" << std::endl;
    QString username = ui->R_Username->text();
    QString password = ui->R_Password->text();
    QString neptun = ui->Neptun->text();
    QString firstName = ui->First_Name->text();
    QString lastName = ui->Last_Name->text();
    bool executed = false;

    mydb.open();

    bool dbcheck = false;
    if(mydb.open())
    {
        std::cout << "Connected to the Database" << std::endl;
        dbcheck = true;
    }
    else
    {
        std::cout << "Failed to Connect to Database" << std::endl;
        std::cout << mydb.lastError().text().toStdString() << std::endl;
    }

    if (!dbcheck)
    {
        QMessageBox::warning(
            this,
            tr("Database"),
            tr("Warning: Unable to connect to database. Please restart application") );

        return 1;
    }

    QSqlDatabase::database().transaction();
    QSqlQuery query;

    std::cout << "You can now execute a query!" << std::endl;
    query.prepare("SELECT * FROM users WHERE neptun_code = ?");
    query.addBindValue(neptun);
    executed = query.exec();

    //TODO: Add method of retreiving username from db when username is forgotten.
    if(query.next())
    {
        QMessageBox::warning(this, "Error: ", "There already exists a record with this neptun code! Please log in!");
        return 0;
    }

    query.prepare("INSERT INTO USERS (neptun_code, first_name, last_name, username, password) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(neptun);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(username);
    query.addBindValue(password);
    executed = query.exec();

    if(!executed)
    {
        std::cout << "Failed to execute query!" << std::endl;
        std::cout << query.lastError().text().toStdString() <<std::endl;
        QMessageBox::critical(this, tr("Registration"), tr("Unable to complete registration! Database Error! Please restart the application!"));
        return 1;
    }
    QSqlDatabase::database().commit();

    //Assuming Successful Registration
    QMessageBox::information(this, tr("Registration"), tr("Registration Successful! Please log in!"));


    mydb.close();
    return 0;
}
