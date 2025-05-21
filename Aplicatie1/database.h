#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QList>

struct User {
    int id;
    QString username;
    QString login;
    QString realname;
    QString email;
    QString dob;       // Date of birth, e.g. "1990-01-01"
    QString country;
    QString createdAt; // Account creation date
};


class Database
{
public:
    static bool initialize();
    static QList<User> getAllUsers();
    static bool updateUser(const User &user);
    static bool deleteUser(int id);
    static bool addUser(const User &user);
};

#endif // DATABASE_H
