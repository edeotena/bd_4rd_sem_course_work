#include "createuser.h"
#include "ui_createuser.h"
#include <QMessageBox>
#include "mainwindow.h"

createUser::createUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createUser)
{
    ui->setupUi(this);
}

createUser::~createUser()
{
    delete ui;
}

void createUser::on_pushButton_clicked()
{
    QString login = ui->login->text();
    QString pass = ui->pass->text();
    QString id = ui->id->text();
    QSqlDatabase db = createUser::get_db();
    ui->login->clear();
    ui->pass->clear();
    ui->id->clear();
    if (!db.open()) {
        QMessageBox::warning(this, "Ошибка!", "Не удалось открыть базу данных!");
    } else {
        QSqlQuery query;
        query.prepare("SELECT MAX(id) FROM qt_user");
        if (query.exec()) {
           int max = 1;
           if (query.size() > 0) {
              query.first();
              max = query.value(0).toInt() + 1;
           }
           qDebug() << "INSERT INTO qt_user (id, id_worker, access, login, password) VALUES (" + QString::number(max) + ", " + id + ", 'no', '" + login + "', '" + pass + "')";
           query.prepare("INSERT INTO qt_user (id, id_worker, access, login, password) VALUES (" + QString::number(max) + ", " + id + ", 'no', '" + login + "', '" + pass + "')");
           if (query.exec()) {
               qDebug() << "Запись добавлена.";
               QMessageBox::information(this, "Успех!", "Пользователь создан!");
               this->close();
               emit loginWindow();
           } else {
               qDebug() << "Ошибка добавления записи.";
               QMessageBox::warning(this, "Ошибка!", "Выберите другой логин!");
           }
        } else {
            qDebug() << "Ошибка выполнения запроса.";
            QMessageBox::warning(this, "Ошибка!", "Сервер не смог выполнить поиск!");
        }
    }
    db.close();
}

