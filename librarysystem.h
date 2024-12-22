#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

// Inclusion des en-têtes des autres classes du système
#include "User.h"
#include "Book.h"
#include "Borrowing.h"
#include <QList>    // Pour gérer les listes d'objets
#include <QMap>     // Pour utiliser une structure de données de type Map
#include <QObject>  // Inclure QObject pour utiliser les signaux et slots
#include <QSharedPointer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

// La classe LibrarySystem gère l'ensemble des utilisateurs, des livres et des emprunts
class LibrarySystem : public QObject  // Hérite de QObject pour utiliser les signaux et slots
{
    Q_OBJECT  // Permet l'utilisation de signaux et de slots

public:
    // Constructeur
    // Permet de créer une instance de la classe LibrarySystem avec un parent optionnel
    LibrarySystem(QObject* parent = nullptr);

    // Méthodes publiques permettant la gestion des utilisateurs, des livres et des emprunts
    void registerUser(const User& user);  // Enregistre un nouvel utilisateur dans le système
    void deleteUser(int userId);          // Supprime un utilisateur en fonction de son ID
    void addBook(const Book& book);       // Ajoute un nouveau livre au système
    QList<QSharedPointer<Book>> searchBooks(const QString& title, const QString& author, const QString& category) const ;
    void deleteBook(int bookId);          // Supprime un livre du système en fonction de son ID
    void borrowBook(int userId, int bookId, const QDate& returnDate);  // Permet à un utilisateur d'emprunter un livre
    bool isBookAvailable(int bookId); // Déclaration de la méthode pour vérifier la disponibilité du livre
    void returnBook(int userId, int bookId);  // Permet à un utilisateur de retourner un livre
    QList<QSharedPointer<User>> getUsers() const;  // Récupère la liste de tous les utilisateurs enregistrés
    QList<QSharedPointer<Book>> getBooks() const; // Récupère la liste de tous les livres disponibles
    QList<QSharedPointer<Borrowing>> getBorrowings() const;  // Récupère la liste de tous les emprunts effectués

    // Méthodes supplémentaires pour l'historique des emprunts
    QList<QSharedPointer<Borrowing>> getBorrowingHistoryForUser(int userId);   // Affiche l'historique des emprunts pour un utilisateur donné
    QList<QSharedPointer<Borrowing>> getBorrowingHistoryForBook(int bookId);   // Affiche l'historique des emprunts pour un livre donné

    // Méthode pour obtenir les livres les plus empruntés
    QMap<int, int> getMostBorrowedBooks() const;  // Retourne une map associant chaque livre à son nombre d'emprunts
signals:
    // Signaux qui sont émis lors de certaines actions dans le système
    void userRegistered(const User& user);  // Émis lorsqu'un utilisateur est enregistré
    void userDeleted(int userId);           // Émis lorsqu'un utilisateur est supprimé
    void bookAdded(const Book& book);       // Émis lorsqu'un livre est ajouté
    void bookDeleted(int bookId);           // Émis lorsqu'un livre est supprimé
    void bookBorrowed(int userId, int bookId);  // Émis lorsqu'un livre est emprunté
    void bookReturned(int userId, int bookId);  // Émis lorsqu'un livre est retourné

    // Signaux pour l'historique des emprunts
    void borrowingHistoryUpdated(int userId);       // Émis lorsque l'historique des emprunts d'un utilisateur est mis à jour
    void borrowingHistoryForBookUpdated(int bookId); // Émis lorsque l'historique des emprunts d'un livre est mis à jour

private:
    QList<QSharedPointer<User>> users;  // Conteneur avec pointeur partagé
    QList<QSharedPointer<Book>> books;  // Conteneur avec pointeur partagé
    QList<QSharedPointer<Borrowing>> borrowings;  // Conteneur avec pointeur partagé

    // Base de données SQLite
    QSqlDatabase db;  // Base de données SQLite

    // Méthodes de gestion de la base de données
    void connectToDatabase();  // Se connecte à la base de données
};

#endif // LIBRARYSYSTEM_H
