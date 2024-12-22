#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QSharedPointer>
#include "LibrarySystem.h"
#include "Book.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot pour gérer le clic sur le bouton de recherche
    void onSearchBooksClicked();

private:
    // Initialisation de l'interface utilisateur
    Ui::MainWindow *ui;

    // Instance du système de gestion de bibliothèque
    LibrarySystem librarySystem;

    // Fonction pour configurer la table des résultats
    void setupTable();
};

#endif // MAINWINDOW_H
