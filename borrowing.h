#ifndef BORROWING_H
#define BORROWING_H

#include <QObject>
#include <QDate>

class Borrowing : public QObject
{
    Q_OBJECT

public:
    // Constructeur avec date de retour
    Borrowing(int userId, int bookId, const QDate& borrowDate, const QDate& returnDate = QDate(), QObject* parent = nullptr);

    int getId() const;
    int getUserId() const;
    int getBookId() const;
    QDate getBorrowDate() const;
    QDate getReturnDate() const;  // Accéder à la date de retour

    static int nextId;  // Compteur pour générer les ID automatiquement

signals:
    void borrowingCreated(int userId, int bookId);

private:
    int id;
    int userId;
    int bookId;
    QDate borrowDate;
    QDate returnDate;  // Ajouter la date de retour
};

#endif // BORROWING_H
