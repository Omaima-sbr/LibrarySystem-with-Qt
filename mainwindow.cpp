#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Initialisation de l'interface

    // Configurer la table des résultats
    setupTable();

    // Connecter le bouton de recherche à la fonction de gestion du clic
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchBooksClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction pour configurer la table des résultats
void MainWindow::setupTable()
{
    // Définir le nombre de colonnes
    ui->resultsTable->setColumnCount(4);

    // Définir les en-têtes des colonnes
    QStringList headers;
    headers << "ID" << "Title" << "Author" << "Copies Available";
    ui->resultsTable->setHorizontalHeaderLabels(headers);

    // Empêcher l'édition directe des cellules
    ui->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Activer la sélection d'une ligne entière
    ui->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Ajuster la taille des colonnes en fonction du contenu
    ui->resultsTable->horizontalHeader()->setStretchLastSection(true);
}

// Fonction pour gérer la recherche de livres
void MainWindow::onSearchBooksClicked()
{
    // Récupérer les critères de recherche saisis par l'utilisateur
    QString title = ui->titleLineEdit->text();
    QString author = ui->authorLineEdit->text();  // Corrigez le nom du widget ici si nécessaire
    QString category = ui->categoryLineEdit->text();

    // Appeler la fonction de recherche dans le système de bibliothèque
    QList<QSharedPointer<Book>> searchResults = librarySystem.searchBooks(title, author, category);

    // Vider la table avant d'afficher les nouveaux résultats
    ui->resultsTable->clearContents();
    ui->resultsTable->setRowCount(0);

    // Afficher les résultats dans la table
    for (const auto& book : searchResults) {
        int row = ui->resultsTable->rowCount();
        ui->resultsTable->insertRow(row);

        // Remplir les colonnes avec les données des livres
        ui->resultsTable->setItem(row, 0, new QTableWidgetItem(QString::number(book->getId())));               // Utiliser getId()
        ui->resultsTable->setItem(row, 1, new QTableWidgetItem(book->getTitle()));                             // Utiliser getTitle()
        ui->resultsTable->setItem(row, 2, new QTableWidgetItem(book->getAuthor()));                            // Utiliser getAuthor()
        ui->resultsTable->setItem(row, 3, new QTableWidgetItem(QString::number(book->getCopiesAvailable()))); // Utiliser getCopiesAvailable()
    }
}
