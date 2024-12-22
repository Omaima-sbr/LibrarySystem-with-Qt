#include "User.h"

// Constructeur par défaut
User::User(QObject *parent)
    : QObject(parent), m_id(0), m_name(""), m_surname(""), m_email(""), m_password(""), m_role("")
{
}

// Constructeur paramétré
User::User(int id, const QString& name, const QString& surname, const QString& email, const QString& password, const QString& role, QObject *parent)
    : QObject(parent), m_id(id), m_name(name), m_surname(surname), m_email(email), m_password(password), m_role(role)
{
}

// Implémentation des getters et setters avec signaux

// Getter pour l'ID
int User::getId() const {
    return m_id;
}

// Setter pour l'ID
void User::setId(int newId) {
    if (m_id != newId) {
        m_id = newId;
        emit idChanged(m_id); // Emitting the signal
    }
}

// Getter pour le prénom
QString User::getName() const {
    return m_name;
}

// Setter pour le prénom
void User::setName(const QString &newName) {
    if (m_name != newName) {
        m_name = newName;
        emit nameChanged(m_name); // Emitting the signal
    }
}

// Getter pour le nom
QString User::getSurname() const {
    return m_surname;
}

// Setter pour le nom
void User::setSurname(const QString &newSurname) {
    if (m_surname != newSurname) {
        m_surname = newSurname;
        emit surnameChanged(m_surname); // Emitting the signal
    }
}

// Getter pour l'email
QString User::getEmail() const {
    return m_email;
}

// Setter pour l'email
void User::setEmail(const QString &newEmail) {
    if (m_email != newEmail) {
        m_email = newEmail;
        emit emailChanged(m_email); // Emitting the signal
    }
}

// Getter pour le mot de passe
QString User::getPassword() const {
    return m_password;
}

// Setter pour le mot de passe
void User::setPassword(const QString &newPassword) {
    if (m_password != newPassword) {
        m_password = newPassword;
        emit passwordChanged(m_password); // Emitting the signal
    }
}

// Getter pour le rôle
QString User::getRole() const {
    return m_role;
}

// Setter pour le rôle
void User::setRole(const QString &newRole) {
    if (m_role != newRole) {
        m_role = newRole;
        emit roleChanged(m_role); // Emitting the signal
    }
}
