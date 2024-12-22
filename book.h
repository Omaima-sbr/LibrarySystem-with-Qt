#ifndef BOOK_H
#define BOOK_H

#include <QObject>
#include <QString>

class Book : public QObject
{
    Q_OBJECT

    // Déclaration des propriétés dynamiques
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString author READ getAuthor WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(int copiesAvailable READ getCopiesAvailable WRITE setCopiesAvailable NOTIFY copiesAvailableChanged)

public:
    explicit Book(QObject *parent = nullptr);  // Constructeur par défaut
    Book(int id, const QString &title, const QString &author, int copiesAvailable, QObject *parent = nullptr);  // Constructeur avec paramètres

    // Getters et setters
    int getId() const;
    void setId(int newId);

    QString getTitle() const;
    void setTitle(const QString &newTitle);

    QString getAuthor() const;
    void setAuthor(const QString &newAuthor);

    int getCopiesAvailable() const;
    void setCopiesAvailable(int newCopiesAvailable);

    // Méthodes pour gérer l'emprunt et le retour
    void borrowBook();
    void returnBook();

signals:
    // Signaux pour notifier les changements de propriété
    void idChanged(int id);
    void titleChanged(const QString &title);
    void authorChanged(const QString &author);
    void copiesAvailableChanged(int copiesAvailable);
    void bookBorrowed();
    void bookReturned();

private:
    int id;
    QString title;
    QString author;
    int copiesAvailable;
};

#endif // BOOK_H
