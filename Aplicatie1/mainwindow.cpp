#include "database.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QFormLayout>
// #include <QDebug>
// #include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Username", "Email"});

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Database::initialize();
    loadUsers();

    connect(ui->addUserButton, &QPushButton::clicked, this, &MainWindow::onAddUserClicked);

    // if (!ui->addUserButton) {
    //     qWarning() << "addUserButton is null!";
    // } else {
    //     qDebug() << "addUserButton found, connecting signal";
    //     connect(ui->addUserButton, &QPushButton::clicked, this, &MainWindow::onAddUserClicked);
    // }
}

void MainWindow::loadUsers() {
    QList<User> users = Database::getAllUsers();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(users.size());

    int row = 0;
    ui->tableWidget->setRowCount(users.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "Username", "Email", "Actions"});

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);

    ui->tableWidget->setColumnWidth(3, 200);

    for (const User &user : users) {
        // Get users
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(user.id)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(user.username));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(user.email));

        // Create buttons
        QPushButton* viewBtn = new QPushButton("View");
        QPushButton* editBtn = new QPushButton("Edit");
        QPushButton* deleteBtn = new QPushButton("Delete");

        // Optional: Set button width for consistent layout
        viewBtn->setFixedWidth(50);
        editBtn->setFixedWidth(50);
        deleteBtn->setFixedWidth(60);

        // Create layout and container widget
        QWidget* buttonWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(buttonWidget);
        layout->addWidget(viewBtn);
        layout->addWidget(editBtn);
        layout->addWidget(deleteBtn);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);

        // Set the layout to the container
        buttonWidget->setLayout(layout);

        // Put all buttons into one cell (column 3)
        ui->tableWidget->setCellWidget(row, 3, buttonWidget);

        // Connect buttons with lambdas or slots, passing user info
        connect(viewBtn, &QPushButton::clicked, [this, user]() {
            // Open view dialog for this user
            QDialog *viewDialog = new QDialog();
            viewDialog->setAttribute(Qt::WA_DeleteOnClose);
            viewDialog->setWindowTitle("View");
            viewDialog->setFixedSize(600, 400);
            viewDialog->show();

            // Create layout
            QVBoxLayout *dialogLayout = new QVBoxLayout(viewDialog);

            // Create table widget
            QTableWidget *table = new QTableWidget(viewDialog);
            table->setRowCount(8); // One row per User field
            table->setColumnCount(2); // Label and Value columns
            table->setHorizontalHeaderLabels({"Label", "Info"});
            table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only

            // Populate table with user data
            table->setItem(0, 0, new QTableWidgetItem("ID"));
            table->setItem(0, 1, new QTableWidgetItem(QString::number(user.id)));
            table->setItem(1, 0, new QTableWidgetItem("Username"));
            table->setItem(1, 1, new QTableWidgetItem(user.username));
            table->setItem(2, 0, new QTableWidgetItem("Login"));
            table->setItem(2, 1, new QTableWidgetItem(user.login.isEmpty() ? "N/A" : user.login));
            table->setItem(3, 0, new QTableWidgetItem("Real Name"));
            table->setItem(3, 1, new QTableWidgetItem(user.realname.isEmpty() ? "N/A" : user.realname));
            table->setItem(4, 0, new QTableWidgetItem("Email"));
            table->setItem(4, 1, new QTableWidgetItem(user.email));
            table->setItem(5, 0, new QTableWidgetItem("Date of Birth"));
            table->setItem(5, 1, new QTableWidgetItem(user.dob.isEmpty() ? "N/A" : user.dob));
            table->setItem(6, 0, new QTableWidgetItem("Country"));
            table->setItem(6, 1, new QTableWidgetItem(user.country.isEmpty() ? "N/A" : user.country));
            table->setItem(7, 0, new QTableWidgetItem("Created At"));
            table->setItem(7, 1, new QTableWidgetItem(user.createdAt.isEmpty() ? "N/A" : user.createdAt));

            // Add table to layout
            dialogLayout->addWidget(table);

            // Add OK button
            QPushButton *okButton = new QPushButton("OK", viewDialog);
            connect(okButton, &QPushButton::clicked, viewDialog, &QDialog::accept);
            dialogLayout->addWidget(okButton);

            viewDialog->setLayout(dialogLayout);
            viewDialog->show();
        });

        // Connect edit button
        connect(editBtn, &QPushButton::clicked, [this, user]() {
            // Create dialog
            QDialog *editDialog = new QDialog(this);
            editDialog->setAttribute(Qt::WA_DeleteOnClose);
            editDialog->setWindowTitle("Edit User Details");
            editDialog->setFixedSize(600, 400);

            // Create layout
            QVBoxLayout *dialogLayout = new QVBoxLayout(editDialog);

            // Create table widget
            QTableWidget *table = new QTableWidget(editDialog);
            table->setRowCount(8); // One row per User field
            table->setColumnCount(2); // Label and Value columns
            table->setHorizontalHeaderLabels({"Field", "Value"});
            table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            table->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only except for QLineEdit

            // Populate table with labels and editable fields
            table->setItem(0, 0, new QTableWidgetItem("ID"));
            table->setItem(0, 1, new QTableWidgetItem(QString::number(user.id))); // ID is read-only
            table->setItem(1, 0, new QTableWidgetItem("Username"));
            table->setItem(2, 0, new QTableWidgetItem("Login"));
            table->setItem(3, 0, new QTableWidgetItem("Real Name"));
            table->setItem(4, 0, new QTableWidgetItem("Email"));
            table->setItem(5, 0, new QTableWidgetItem("Date of Birth"));
            table->setItem(6, 0, new QTableWidgetItem("Country"));
            table->setItem(7, 0, new QTableWidgetItem("Created At"));

            // Create QLineEdit widgets for editable fields
            QLineEdit *usernameEdit = new QLineEdit(user.username, table);
            QLineEdit *loginEdit = new QLineEdit(user.login.isEmpty() ? "N/A" : user.login, table);
            QLineEdit *realnameEdit = new QLineEdit(user.realname.isEmpty() ? "N/A" : user.realname, table);
            QLineEdit *emailEdit = new QLineEdit(user.email, table);
            QLineEdit *dobEdit = new QLineEdit(user.dob.isEmpty() ? "N/A" : user.dob, table);
            QLineEdit *countryEdit = new QLineEdit(user.country.isEmpty() ? "N/A" : user.country, table);
            QLineEdit *createdAtEdit = new QLineEdit(user.createdAt.isEmpty() ? "N/A" : user.createdAt, table);

            // Set QLineEdit widgets in the Value column
            table->setCellWidget(1, 1, usernameEdit);
            table->setCellWidget(2, 1, loginEdit);
            table->setCellWidget(3, 1, realnameEdit);
            table->setCellWidget(4, 1, emailEdit);
            table->setCellWidget(5, 1, dobEdit);
            table->setCellWidget(6, 1, countryEdit);
            table->setCellWidget(7, 1, createdAtEdit);

            // Add table to layout
            dialogLayout->addWidget(table);

            // Add OK button to save changes
            QPushButton *okButton = new QPushButton("OK", editDialog);
            connect(okButton, &QPushButton::clicked, [this, editDialog, user, usernameEdit, loginEdit, realnameEdit, emailEdit, dobEdit, countryEdit, createdAtEdit]() {
                // Create updated User object
                User updatedUser = user;
                updatedUser.username = usernameEdit->text();
                updatedUser.login = loginEdit->text() == "N/A" ? "" : loginEdit->text();
                updatedUser.realname = realnameEdit->text() == "N/A" ? "" : realnameEdit->text();
                updatedUser.email = emailEdit->text();
                updatedUser.dob = dobEdit->text() == "N/A" ? "" : dobEdit->text();
                updatedUser.country = countryEdit->text() == "N/A" ? "" : countryEdit->text();
                updatedUser.createdAt = createdAtEdit->text() == "N/A" ? "" : createdAtEdit->text();

                // Update database
                if (Database::updateUser(updatedUser)) {
                    loadUsers(); // Refresh table
                    editDialog->accept();
                } else {
                    QMessageBox::warning(editDialog, "Error", "Failed to update user in database.");
                }
            });

            // Add Cancel button
            QPushButton *cancelButton = new QPushButton("Cancel", editDialog);
            connect(cancelButton, &QPushButton::clicked, editDialog, &QDialog::reject);
            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(okButton);
            buttonLayout->addWidget(cancelButton);
            dialogLayout->addLayout(buttonLayout);

            editDialog->setLayout(dialogLayout);
            editDialog->show();
        });

        // Connect delete button
        connect(deleteBtn, &QPushButton::clicked, [this, user]() {
            // Create dialog
            QDialog *deleteDialog = new QDialog(this);
            deleteDialog->setAttribute(Qt::WA_DeleteOnClose);
            deleteDialog->setWindowTitle("Confirm Deletion");
            deleteDialog->setFixedSize(400, 200);

            // Create layout
            QVBoxLayout *dialogLayout = new QVBoxLayout(deleteDialog);

            // Add confirmation message
            QLabel *message = new QLabel(QString("Are you sure you want to delete user '%1'?").arg(user.username), deleteDialog);
            message->setAlignment(Qt::AlignCenter);
            dialogLayout->addWidget(message);

            // Add Yes/No buttons
            QPushButton *yesButton = new QPushButton("Yes", deleteDialog);
            QPushButton *noButton = new QPushButton("No", deleteDialog);
            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(yesButton);
            buttonLayout->addWidget(noButton);
            dialogLayout->addLayout(buttonLayout);

            // Connect Yes button
            connect(yesButton, &QPushButton::clicked, [this, deleteDialog, user]() {
                if (Database::deleteUser(user.id)) {
                    qDebug() << "Deletion successful, refreshing table";
                    loadUsers(); // Refresh table
                    deleteDialog->accept();
                } else {
                    QMessageBox::warning(deleteDialog, "Error", "Failed to delete user from database.");
                }
            });

            // Connect No button
            connect(noButton, &QPushButton::clicked, deleteDialog, &QDialog::reject);

            deleteDialog->setLayout(dialogLayout);
            deleteDialog->show();
        });

        ++row;
    }
}

void MainWindow::onAddUserClicked() {
    // Create dialog
    QDialog *addDialog = new QDialog(this);
    addDialog->setAttribute(Qt::WA_DeleteOnClose);
    addDialog->setWindowTitle("Add New User");
    addDialog->setFixedSize(400, 400);

    // Create layout
    QVBoxLayout *dialogLayout = new QVBoxLayout(addDialog);
    QFormLayout *formLayout = new QFormLayout();

    // Create input fields
    QLineEdit *usernameEdit = new QLineEdit(addDialog);
    QLineEdit *loginEdit = new QLineEdit(addDialog);
    QLineEdit *realnameEdit = new QLineEdit(addDialog);
    QLineEdit *emailEdit = new QLineEdit(addDialog);
    QLineEdit *dobEdit = new QLineEdit(addDialog);
    QLineEdit *countryEdit = new QLineEdit(addDialog);
    QLineEdit *createdAtEdit = new QLineEdit(addDialog);

    // Add fields to form layout
    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Login:", loginEdit);
    formLayout->addRow("Real Name:", realnameEdit);
    formLayout->addRow("Email:", emailEdit);
    formLayout->addRow("Date of Birth:", dobEdit);
    formLayout->addRow("Country:", countryEdit);
    formLayout->addRow("Created At:", createdAtEdit);

    dialogLayout->addLayout(formLayout);

    // Add OK and Cancel buttons
    QPushButton *okButton = new QPushButton("OK", addDialog);
    QPushButton *cancelButton = new QPushButton("Cancel", addDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    dialogLayout->addLayout(buttonLayout);

    // Connect OK button
    connect(okButton, &QPushButton::clicked, [this, addDialog, usernameEdit, loginEdit, realnameEdit, emailEdit, dobEdit, countryEdit, createdAtEdit]() {
        // Validate required fields
        // qDebug() << "Add User button clicked at" << QDateTime::currentDateTime().toString();
        if (usernameEdit->text().isEmpty() || emailEdit->text().isEmpty()) {
            QMessageBox::warning(addDialog, "Error", "Username and Email are required.");
            return;
        }

        // Create new User object
        User newUser;
        newUser.id = 0; // Will be auto-generated by SQLite
        newUser.username = usernameEdit->text();
        newUser.login = loginEdit->text();
        newUser.realname = realnameEdit->text();
        newUser.email = emailEdit->text();
        newUser.dob = dobEdit->text();
        newUser.country = countryEdit->text();
        newUser.createdAt = createdAtEdit->text();

        // Add to database
        if (Database::addUser(newUser)) {
            loadUsers(); // Refresh table
            addDialog->accept();
        } else {
            QMessageBox::warning(addDialog, "Error", "Failed to add user to database.");
        }
    });

    // Connect Cancel button
    connect(cancelButton, &QPushButton::clicked, addDialog, &QDialog::reject);

    addDialog->setLayout(dialogLayout);
    addDialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}



