#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


QT_BEGIN_NAMESPACE
namespace Ui { class Library; }
QT_END_NAMESPACE

class Library : public QMainWindow
{
    Q_OBJECT

public:
    Library(QWidget *parent = nullptr);
    ~Library();

private:
    Ui::Library *ui;
    QSqlDatabase mydb;

private slots:
    int Login();
    int Register();

};
#endif // LIBRARY_H
