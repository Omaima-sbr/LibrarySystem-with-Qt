#ifndef USER_H
#define USER_H

#include <QObject>
#include <QString>

class User : public QObject
{
    Q_OBJECT // Macro nécessaire pour les fonctionnalités QObject

    // Propriétés pour une utilisation avec des outils Qt
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString surname READ getSurname WRITE setSurname NOTIFY surnameChanged)
    Q_PROPERTY(QString email READ getEmail WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString role READ getRole WRITE setRole NOTIFY roleChanged)

public:
    explicit User(QObject *parent = nullptr);
    User(int id, const QString& name, const QString& surname, const QString& email, const QString& password, const QString& role, QObject *parent = nullptr);

    // Supprimer le constructeur de copie et l'opérateur d'affectation
    User(const User&) = delete;  // Supprimer le constructeur de copie
    User& operator=(const User&) = delete;  // Supprimer l'opérateur d'affectation

    // Getters
    int getId() const;
    QString getName() const;
    QString getSurname() const;
    QString getEmail() const;
    QString getPassword() const;
    QString getRole() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setSurname(const QString& surname);
    void setEmail(const QString& email);
    void setPassword(const QString& password);
    void setRole(const QString& role);

signals:
    // Signaux pour notifier des changements
    void idChanged(int id);
    void nameChanged(const QString& name);
    void surnameChanged(const QString& surname);
    void emailChanged(const QString& email);
    void passwordChanged(const QString& password);
    void roleChanged(const QString& role);

private:
    int m_id;
    QString m_name;
    QString m_surname;
    QString m_email;
    QString m_password;
    QString m_role;
};

#endif // USER_H
