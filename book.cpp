#include "Book.h"

Book::Book(QObject *parent)
    : QObject(parent), id(0), title(""), author(""), copiesAvailable(0)
{
    // Initialisation des membres par défaut
}

Book::Book(int id, const QString& title, const QString& author, int copiesAvailable, QObject *parent)
    : QObject(parent), id(id), title(title), author(author), copiesAvailable(copiesAvailable)
{
}

int Book::getId() const { return id; }
void Book::setId(int newId) {
    if (id != newId) {
        id = newId;
        emit idChanged(id);
    }
}

QString Book::getTitle() const { return title; }
void Book::setTitle(const QString& newTitle) {
    if (title != newTitle) {
        title = newTitle;
        emit titleChanged(title);
    }
}

QString Book::getAuthor() const { return author; }
void Book::setAuthor(const QString& newAuthor) {
    if (author != newAuthor) {
        author = newAuthor;
        emit authorChanged(author);
    }
}

int Book::getCopiesAvailable() const { return copiesAvailable; }
void Book::setCopiesAvailable(int newCopiesAvailable) {
    if (copiesAvailable != newCopiesAvailable) {
        copiesAvailable = newCopiesAvailable;
        emit copiesAvailableChanged(copiesAvailable);
    }
}

void Book::borrowBook() {
    if (copiesAvailable > 0) {
        --copiesAvailable;
        emit copiesAvailableChanged(copiesAvailable);  // Émettre le signal de mise à jour de la disponibilité
        emit bookBorrowed();  // Signaler que le livre a été emprunté
    }
}

void Book::returnBook() {
    ++copiesAvailable;
    emit copiesAvailableChanged(copiesAvailable);  // Émettre le signal de mise à jour de la disponibilité
    emit bookReturned();  // Signaler que le livre a été retourné
}
