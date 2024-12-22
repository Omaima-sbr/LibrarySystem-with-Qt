#include "LibrarySystem.h"
#include "User.h"
#include "Book.h"
#include "Borrowing.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>


LibrarySystem::LibrarySystem(QObject* parent)
    : QObject(parent)
{
    // Connexion à la base de données SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("file.db");  // Nom de votre fichier SQLite (peut être un chemin complet)

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
    } else {
        qDebug() << "Base de données connectée avec succès.";
    }
}


void LibrarySystem::registerUser(const User& user)
{
    // Insérer l'utilisateur dans la base de données sans spécifier l'ID
    QSqlQuery query;
    query.prepare("INSERT INTO users (name, surname, email, password, role) "
                  "VALUES (:name, :surname, :email, :password, :role)");

    query.bindValue(":name", user.getName());
    query.bindValue(":surname", user.getSurname());
    query.bindValue(":email", user.getEmail());
    query.bindValue(":password", user.getPassword());
    query.bindValue(":role", user.getRole());

    if (!query.exec()) {
        qDebug() << "Erreur d'insertion dans la base de données : " << query.lastError();
        return;
    }

    // Obtenir l'ID généré automatiquement par la base de données
    int generatedId = query.lastInsertId().toInt();

    // Créer un pointeur partagé avec l'ID généré automatiquement
    QSharedPointer<User> userPtr = QSharedPointer<User>::create(generatedId, user.getName(), user.getSurname(), user.getEmail(), user.getPassword(), user.getRole());

    // Ajouter l'utilisateur à la liste en mémoire
    users.append(userPtr);

    // Émettre le signal avec l'utilisateur déréférencé
    emit userRegistered(*userPtr);
}




void LibrarySystem::deleteUser(int userId)
{
    // Créer une requête SQL pour supprimer l'utilisateur de la base de données
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec()) {
        // L'utilisateur a été supprimé de la base de données avec succès

        // Maintenant, supprimer l'utilisateur de la liste en mémoire
        for (int i = 0; i < users.size(); ++i) {
            if (users[i]->getId() == userId) {
                users.removeAt(i);  // Supprimer l'utilisateur de la liste
                emit userDeleted(userId);  // Émettre le signal
                break;
            }
        }
    } else {
        // Si la requête échoue, afficher un message d'erreur
        qDebug() << "Error deleting user from database:" << query.lastError().text();
    }
}



void LibrarySystem::addBook(const Book& book)
{
    // Créer une requête SQL pour insérer le livre dans la base de données (sans l'ID)
    QSqlQuery query;
    query.prepare("INSERT INTO books (title, author, copiesAvailable) "
                  "VALUES (:title, :author, :copiesAvailable)");

    // Lier les valeurs à la requête (sans l'ID)
    query.bindValue(":title", book.getTitle());
    query.bindValue(":author", book.getAuthor());
    query.bindValue(":copiesAvailable", book.getCopiesAvailable());

    // Exécuter la requête d'insertion
    if (query.exec()) {
        // L'insertion a réussi dans la base de données

        // Créer un pointeur partagé pour le livre avec les informations de la requête
        // L'ID sera auto-généré par la base de données
        int insertedId = query.lastInsertId().toInt(); // Récupérer l'ID généré par la base de données

        // Créer un pointeur partagé pour le livre avec l'ID auto-généré
        QSharedPointer<Book> bookPtr = QSharedPointer<Book>::create(insertedId, book.getTitle(), book.getAuthor(), book.getCopiesAvailable());

        // Ajouter le pointeur partagé à la liste en mémoire
        books.append(bookPtr);

        // Émettre le signal avec le livre déréférencé
        emit bookAdded(*bookPtr);
    } else {
        // Si l'insertion échoue, afficher un message d'erreur
        qDebug() << "Error adding book to database:" << query.lastError().text();
    }
}


QList<QSharedPointer<Book>> LibrarySystem::searchBooks(const QString& title, const QString& author, const QString& category) const {
    // Initialisation de la liste pour stocker les résultats de la recherche
    QList<QSharedPointer<Book>> searchResults;
    QSqlQuery query;

    // Création de la requête SQL de base pour sélectionner les livres
    QString queryString = "SELECT books.id, books.title, books.author, books.copiesAvailable "
                          "FROM books "
                          "LEFT JOIN book_categories ON books.id = book_categories.bookId "
                          "LEFT JOIN categories ON book_categories.categoryId = categories.id "
                          "WHERE 1"; // 'WHERE 1' permet d'ajouter facilement des conditions supplémentaires

    // Ajout dynamique de conditions de recherche en fonction des paramètres fournis
    if (!title.isEmpty()) {
        queryString += " AND books.title LIKE :title"; // Condition pour le titre
    }
    if (!author.isEmpty()) {
        queryString += " AND books.author LIKE :author"; // Condition pour l'auteur
    }
    if (!category.isEmpty()) {
        queryString += " AND categories.name LIKE :category"; // Condition pour la catégorie
    }

    // Préparation de la requête SQL
    query.prepare(queryString);

    // Liaison des valeurs de recherche si elles sont spécifiées
    if (!title.isEmpty()) {
        query.bindValue(":title", "%" + title + "%"); // Recherche partielle pour le titre
    }
    if (!author.isEmpty()) {
        query.bindValue(":author", "%" + author + "%"); // Recherche partielle pour l'auteur
    }
    if (!category.isEmpty()) {
        query.bindValue(":category", "%" + category + "%"); // Recherche partielle pour la catégorie
    }

    // Exécution de la requête
    if (query.exec()) {
        // Parcours des résultats de la requête
        while (query.next()) {
            // Récupération des valeurs des colonnes
            int id = query.value("id").toInt();
            QString bookTitle = query.value("title").toString();
            QString bookAuthor = query.value("author").toString();
            int copiesAvailable = query.value("copiesAvailable").toInt();

            // Création d'un objet Book et ajout à la liste des résultats
            QSharedPointer<Book> book = QSharedPointer<Book>::create(id, bookTitle, bookAuthor, copiesAvailable);
            searchResults.append(book);
        }
    } else {
        // Gestion des erreurs en cas d'échec de l'exécution de la requête
        qDebug() << "Error retrieving search results:" << query.lastError().text();
    }

    // Retourne la liste des livres trouvés
    return searchResults;
}





void LibrarySystem::deleteBook(int bookId)
{
    // Créer une requête SQL pour supprimer le livre de la base de données
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", bookId);

    // Exécuter la requête de suppression
    if (query.exec()) {
        // Si la suppression a réussi dans la base de données

        // Supprimer le livre de la liste en mémoire
        for (int i = 0; i < books.size(); ++i) {
            if (books[i]->getId() == bookId) {  // Accéder au livre via le pointeur partagé
                books.removeAt(i);  // Supprimer le livre de la liste en mémoire
                emit bookDeleted(bookId);  // Émettre le signal
                break;
            }
        }
    } else {
        // Si la suppression échoue dans la base de données, afficher un message d'erreur
        qDebug() << "Error deleting book from database:" << query.lastError().text();
    }
}


void LibrarySystem::borrowBook(int userId, int bookId, const QDate& returnDate)
{
    // Vérifier si le livre est disponible avant de procéder à l'emprunt
    if (!isBookAvailable(bookId)) {
        qDebug() << "The book is not available for borrowing.";
        return;  // Sortir de la fonction si le livre n'est pas disponible
    }

    // Créer un emprunt avec un pointeur partagé, la date de retour est passée comme argument
    QSharedPointer<Borrowing> newBorrowing = QSharedPointer<Borrowing>::create(userId, bookId, QDate::currentDate(), returnDate);

    // Créer une requête SQL pour insérer l'emprunt dans la base de données
    QSqlQuery query;
    query.prepare("INSERT INTO borrowings (userId, bookId, borrowDate, returnDate) "
                  "VALUES (:userId, :bookId, :borrowDate, :returnDate)");
    query.bindValue(":userId", userId);
    query.bindValue(":bookId", bookId);
    query.bindValue(":borrowDate", newBorrowing->getBorrowDate());
    query.bindValue(":returnDate", returnDate.isValid() ? returnDate : QVariant());  // Si la date de retour n'est pas définie, utiliser NULL

    // Exécuter la requête d'insertion dans la base de données
    if (query.exec()) {
        // Si l'emprunt a été ajouté à la base de données, ajouter également à la mémoire
        borrowings.append(newBorrowing);  // Ajouter l'emprunt à la liste en mémoire

        // Émettre le signal pour notifier que le livre a été emprunté
        emit bookBorrowed(userId, bookId);
    } else {
        // Si l'insertion échoue, afficher un message d'erreur
        qDebug() << "Error borrowing book:" << query.lastError().text();
    }
}



bool LibrarySystem::isBookAvailable(int bookId) {
    QSqlQuery query;
    query.prepare("SELECT copiesAvailable FROM books WHERE id = :bookId");
    query.bindValue(":bookId", bookId);

    if (query.exec() && query.next()) {
        int copiesAvailable = query.value(0).toInt();
        return copiesAvailable > 0;
    }
    return false;
}


void LibrarySystem::returnBook(int userId, int bookId)
{
    // Créer une requête SQL pour mettre à jour la date de retour dans la base de données
    QSqlQuery query;
    query.prepare("UPDATE borrowings SET returnDate = :returnDate WHERE userId = :userId AND bookId = :bookId AND returnDate IS NULL");
    query.bindValue(":returnDate", QDate::currentDate());  // Utiliser la date du jour comme date de retour
    query.bindValue(":userId", userId);
    query.bindValue(":bookId", bookId);

    // Exécuter la requête de mise à jour
    if (query.exec()) {
        // Si la mise à jour est réussie, on met à jour la disponibilité du livre
        // Créer une requête pour augmenter les copies disponibles du livre
        QSqlQuery updateCopiesQuery;
        updateCopiesQuery.prepare("UPDATE books SET copiesAvailable = copiesAvailable + 1 WHERE id = :bookId");
        updateCopiesQuery.bindValue(":bookId", bookId);

        if (updateCopiesQuery.exec()) {
            // Si l'update des copies est réussi, on peut supprimer l'emprunt de la base de données
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM borrowings WHERE userId = :userId AND bookId = :bookId AND returnDate IS NOT NULL");
            deleteQuery.bindValue(":userId", userId);
            deleteQuery.bindValue(":bookId", bookId);

            if (deleteQuery.exec()) {
                // Si la suppression est réussie, on émet le signal pour notifier le retour du livre
                emit bookReturned(userId, bookId);
            } else {
                // Si la suppression échoue, afficher un message d'erreur
                qDebug() << "Error deleting borrowing record:" << deleteQuery.lastError().text();
            }
        } else {
            // Si la mise à jour des copies échoue, afficher un message d'erreur
            qDebug() << "Error updating book availability:" << updateCopiesQuery.lastError().text();
        }
    } else {
        // Si l'insertion échoue, afficher un message d'erreur
        qDebug() << "Error returning book:" << query.lastError().text();
    }

    // Mettre à jour la mémoire pour synchroniser avec la base de données
    for (int i = 0; i < borrowings.size(); ++i) {
        if (borrowings[i]->getUserId() == userId && borrowings[i]->getBookId() == bookId) {
            borrowings.removeAt(i);  // Supprimer l'emprunt de la liste en mémoire
            break;
        }
    }
}


QList<QSharedPointer<User>> LibrarySystem::getUsers() const
{
    QList<QSharedPointer<User>> users;
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer tous les utilisateurs
    query.prepare("SELECT id, name, surname, email, password, role FROM users");

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            int id = query.value("id").toInt();
            QString name = query.value("name").toString();
            QString surname = query.value("surname").toString();
            QString email = query.value("email").toString();
            QString password = query.value("password").toString();
            QString role = query.value("role").toString();

            // Créer un objet User avec les données récupérées
            QSharedPointer<User> user = QSharedPointer<User>::create(id, name, surname, email, password, role);

            // Ajouter l'utilisateur à la liste
            users.append(user);
        }
    } else {
        qDebug() << "Error retrieving users:" << query.lastError().text();
    }

    return users;
}


QList<QSharedPointer<Book>> LibrarySystem::getBooks() const
{
    QList<QSharedPointer<Book>> books;
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer tous les livres
    query.prepare("SELECT id, title, author, copiesAvailable FROM books");

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            int id = query.value("id").toInt();
            QString title = query.value("title").toString();
            QString author = query.value("author").toString();
            int copiesAvailable = query.value("copiesAvailable").toInt();

            // Créer un objet Book avec les données récupérées
            QSharedPointer<Book> book = QSharedPointer<Book>::create(id, title, author, copiesAvailable);

            // Ajouter le livre à la liste
            books.append(book);
        }
    } else {
        qDebug() << "Error retrieving books:" << query.lastError().text();
    }

    return books;
}


QList<QSharedPointer<Borrowing>> LibrarySystem::getBorrowings() const
{
    QList<QSharedPointer<Borrowing>> borrowings;
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer tous les emprunts
    query.prepare("SELECT userId, bookId, borrowDate, returnDate FROM borrowings");

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            int userId = query.value("userId").toInt();
            int bookId = query.value("bookId").toInt();
            QDate borrowDate = query.value("borrowDate").toDate();
            QDate returnDate = query.value("returnDate").toDate();

            // Créer un objet Borrowing avec les données récupérées
            QSharedPointer<Borrowing> borrowing = QSharedPointer<Borrowing>::create(userId, bookId, borrowDate, returnDate);

            // Ajouter l'emprunt à la liste
            borrowings.append(borrowing);
        }
    } else {
        // Si la requête échoue, afficher un message d'erreur
        qDebug() << "Error retrieving borrowings:" << query.lastError().text();
    }

    return borrowings;
}




QList<QSharedPointer<Borrowing>> LibrarySystem::getBorrowingHistoryForUser(int userId)
{
    QList<QSharedPointer<Borrowing>> history;
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer l'historique des emprunts de l'utilisateur
    query.prepare("SELECT id, userId, bookId, borrowDate, returnDate FROM borrowings WHERE userId = :userId");
    query.bindValue(":userId", userId);

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            int bookId = query.value("bookId").toInt();
            QDate borrowDate = query.value("borrowDate").toDate();
            QDate returnDate = query.value("returnDate").toDate();

            // Créer un objet Borrowing à partir des données récupérées
            QSharedPointer<Borrowing> borrowing = QSharedPointer<Borrowing>::create(userId, bookId, borrowDate, returnDate);

            // Ajouter l'emprunt à l'historique
            history.append(borrowing);
        }
        emit borrowingHistoryUpdated(userId);  // Émettre le signal une fois que l'historique est mis à jour
    } else {
        // Si la requête échoue, afficher l'erreur
        qDebug() << "Error retrieving borrowing history:" << query.lastError().text();
    }

    return history;
}



QList<QSharedPointer<Borrowing>> LibrarySystem::getBorrowingHistoryForBook(int bookId)
{
    QList<QSharedPointer<Borrowing>> history;
    QSqlQuery query;

    // Préparer la requête SQL pour récupérer l'historique des emprunts du livre
    query.prepare("SELECT userId, bookId, borrowDate, returnDate FROM borrowings WHERE bookId = :bookId");
    query.bindValue(":bookId", bookId);

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            int userId = query.value("userId").toInt();
            QDate borrowDate = query.value("borrowDate").toDate();
            QDate returnDate = query.value("returnDate").toDate();

            // Créer un objet Borrowing à partir des données récupérées
            QSharedPointer<Borrowing> borrowing = QSharedPointer<Borrowing>::create(userId, bookId, borrowDate, returnDate);

            // Ajouter l'emprunt à l'historique
            history.append(borrowing);
        }
        emit borrowingHistoryForBookUpdated(bookId);  // Émettre le signal une fois que l'historique est mis à jour
    } else {
        // Si la requête échoue, afficher l'erreur
        qDebug() << "Error retrieving borrowing history for book:" << query.lastError().text();
    }

    return history;
}




// Retourner les livres les plus empruntés
QMap<int, int> LibrarySystem::getMostBorrowedBooks() const
{
    // Crée un QMap pour stocker le nombre d'emprunts pour chaque livre. La clé est l'ID du livre, et la valeur est le nombre d'emprunts.
    QMap<int, int> borrowCount;

    // Crée une instance de QSqlQuery pour exécuter une requête SQL.
    QSqlQuery query;

    // Prépare la requête SQL pour récupérer le nombre d'emprunts pour chaque livre.
    // La requête sélectionne l'ID du livre (bookId) et le nombre d'emprunts (COUNT(*) AS borrowCount).
    // La clause GROUP BY regroupe les résultats par bookId.
    query.prepare("SELECT bookId, COUNT(*) AS borrowCount FROM borrowings GROUP BY bookId");

    // Exécute la requête.
    if (query.exec()) {
        // Si la requête réussit, parcourt les résultats ligne par ligne.
        while (query.next()) {
            // Récupère l'ID du livre à partir du résultat de la requête.
            int bookId = query.value("bookId").toInt();

            // Récupère le nombre d'emprunts du livre à partir du résultat de la requête.
            int count = query.value("borrowCount").toInt();

            // Insère l'ID du livre et son nombre d'emprunts dans le QMap.
            // Si le livre est déjà présent dans le QMap, sa valeur (le nombre d'emprunts) est mise à jour.
            borrowCount.insert(bookId, count);
        }
    } else {
        // Si la requête échoue, affiche l'erreur dans la console pour le débogage.
        qDebug() << "Error retrieving most borrowed books:" << query.lastError().text();
    }

    // Retourne le QMap contenant l'ID des livres et leur nombre d'emprunts.
    return borrowCount;
}


