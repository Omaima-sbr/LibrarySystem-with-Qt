#include "Borrowing.h"

// Initialisation du compteur d'ID
int Borrowing::nextId = 1;

Borrowing::Borrowing(int userId, int bookId, const QDate& borrowDate, const QDate& returnDate, QObject* parent)
    : QObject(parent), userId(userId), bookId(bookId), borrowDate(borrowDate), returnDate(returnDate)
{
    // Générer l'ID automatiquement
    this->id = nextId++;
    emit borrowingCreated(userId, bookId);
}

int Borrowing::getId() const { return id; }
int Borrowing::getUserId() const { return userId; }
int Borrowing::getBookId() const { return bookId; }
QDate Borrowing::getBorrowDate() const { return borrowDate; }
QDate Borrowing::getReturnDate() const { return returnDate; }  // Retourner la date de retour
