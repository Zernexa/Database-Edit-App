#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

QList<User> Database::getAllUsers()
{
    QList<User> users;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open!";
        return users;
    }

    QSqlQuery query("SELECT id, username, user_login, real_full_name, email, date_of_birth, country, account_creation_date FROM users");
    while (query.next()) {
        User user;
        user.id = query.value(0).toInt();
        user.username = query.value(1).toString();
        user.login = query.value(2).toString().isEmpty() ? "N/A" : query.value(2).toString();
        user.realname = query.value(3).toString().isEmpty() ? "N/A" : query.value(3).toString();
        user.email = query.value(4).toString();
        user.dob = query.value(5).toString().isEmpty() ? "N/A" : query.value(5).toString();
        user.country = query.value(6).toString().isEmpty() ? "N/A" : query.value(6).toString();
        user.createdAt = query.value(7).toString().isEmpty() ? "N/A" : query.value(7).toString();
        users.append(user);
    }

    qDebug() << "Users loaded:" << users.size();
    return users;
}

bool Database::initialize() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = QCoreApplication::applicationDirPath() + "/users.db";
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY, "
                    "username TEXT, "
                    "user_login TEXT, "
                    "real_full_name TEXT, "
                    "email TEXT, "
                    "date_of_birth TEXT, "
                    "country TEXT, "
                    "account_creation_date TEXT)")) {
        qWarning() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    qDebug() << "Database opened successfully.";
    qDebug() << "Database path used:" << dbPath;
    return true;
}

bool Database::updateUser(const User &user)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open!";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET username = :username, user_login = :user_login, real_full_name = :real_full_name, "
                  "email = :email, date_of_birth = :date_of_birth, country = :country, "
                  "account_creation_date = :account_creation_date WHERE id = :id");
    query.bindValue(":username", user.username);
    query.bindValue(":user_login", user.login);
    query.bindValue(":real_full_name", user.realname);
    query.bindValue(":email", user.email);
    query.bindValue(":date_of_birth", user.dob);
    query.bindValue(":country", user.country);
    query.bindValue(":account_creation_date", user.createdAt);
    query.bindValue(":id", user.id);

    if (!query.exec()) {
        qWarning() << "Failed to update user:" << query.lastError().text();
        return false;
    }

    qDebug() << "User updated: ID" << user.id;
    return true;
}

bool Database::deleteUser(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open in deleteUser!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to delete user ID" << id << ":" << query.lastError().text();
        return false;
    }

    int rowsAffected = query.numRowsAffected();
    qDebug() << "Delete user ID" << id << ": Rows affected:" << rowsAffected;
    if (rowsAffected == 0) {
        qWarning() << "No user found with ID" << id;
        return false;
    }

    return true;
}

bool Database::addUser(const User &user)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open in addUser!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, user_login, real_full_name, email, date_of_birth, country, account_creation_date) "
                  "VALUES (:username, :user_login, :real_full_name, :email, :date_of_birth, :country, :account_creation_date)");
    query.bindValue(":username", user.username);
    query.bindValue(":user_login", user.login);
    query.bindValue(":real_full_name", user.realname);
    query.bindValue(":email", user.email);
    query.bindValue(":date_of_birth", user.dob);
    query.bindValue(":country", user.country);
    query.bindValue(":account_creation_date", user.createdAt);

    if (!query.exec()) {
        qWarning() << "Failed to add user:" << query.lastError().text();
        return false;
    }

    int rowsAffected = query.numRowsAffected();
    qDebug() << "User added: Username" << user.username << ", Rows affected:" << rowsAffected;
    return rowsAffected > 0;
}
