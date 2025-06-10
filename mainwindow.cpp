#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gitProcess = new QProcess(this);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    fileModel->setRootPath("");

    ui->treeView->setModel(fileModel);
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
    ui->treeView->setColumnHidden(3, true);

    connect(ui->switchButton, &QPushButton::clicked, this, &MainWindow::on_switchButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_path_pushButton_clicked()
{
    projectPath = ui->path_lineEdit->text();
    if (!QDir(projectPath).exists()) {
        QMessageBox::warning(this, "Warning!", "The specified path does not exist.");
        projectPath.clear();
        return;
    }
    gitProcess->setWorkingDirectory(projectPath);

    ui->treeView->setRootIndex(fileModel->setRootPath(projectPath));

    loadBranchesIntoListWidget();

    QMessageBox::information(this, "Info", "Working directory set to:\n" + projectPath);
}

void MainWindow::loadBranchesIntoListWidget()
{
    if (projectPath.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Project path is not set");
        return;
    }

    gitProcess->start("git", QStringList() << "branch");
    if (!gitProcess->waitForFinished(5000)) {
        QMessageBox::critical(this, "Error", "Git command timed out.");
        return;
    }

    QString output = gitProcess->readAllStandardOutput();
    QString error = gitProcess->readAllStandardError();

    if (!error.isEmpty()) {
        QMessageBox::critical(this, "Git Error", error);
        return;
    }

    ui->branchListWidget->clear();

    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QString branchName = line.trimmed();
        QListWidgetItem *item = nullptr;

        if (branchName.startsWith('*')) {
            branchName = branchName.mid(1).trimmed();
            item = new QListWidgetItem(branchName);
            item->setSelected(true);

            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        } else {
            item = new QListWidgetItem(branchName);
        }

        ui->branchListWidget->addItem(item);
    }
}

void MainWindow::on_command_pushButton_clicked()
{
    if (projectPath.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "No project path is given");
        return;
    }

    QString commandText = ui->command_lineEdit->text().trimmed();
    if (commandText.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "No git command entered");
        return;
    }

    QStringList arguments = commandText.split(' ', Qt::SkipEmptyParts);

    gitProcess->start("git", arguments);

    if (!gitProcess->waitForFinished(5000)) {
        QMessageBox::critical(this, "Error", "Git command timed out.");
        return;
    }

    QString output = gitProcess->readAllStandardOutput();
    QString error = gitProcess->readAllStandardError();

    if (!error.isEmpty()) {
        QMessageBox::critical(this, "Git Error", error);
        return;
    }

    if (!arguments.isEmpty() && arguments[0] == "branch") {
        loadBranchesIntoListWidget();

        // Show branches info in a message box
        QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        QString displayText = "Branches:\n";

        for (QString line : lines) {
            line = line.trimmed();
            if (line.startsWith('*')) {
                displayText += QString("• %1 (current branch)\n").arg(line.mid(1).trimmed());
            } else {
                displayText += QString("• %1\n").arg(line);
            }
        }

        QMessageBox::information(this, "Git Branches", displayText);
    } else {
        QMessageBox::information(this, "Git Output", output.isEmpty() ? "Git command succeeded." : output);
    }
}


void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void MainWindow::on_switchButton_clicked()
{
    if (projectPath.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Project path is not set");
        return;
    }

    QListWidgetItem *selectedItem = ui->branchListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Warning", "No branch selected");
        return;
    }

    QString branchName = selectedItem->text();

    gitProcess->start("git", QStringList() << "checkout" << branchName);
    if (!gitProcess->waitForFinished(5000)) {
        QMessageBox::critical(this, "Error", "Git command timed out.");
        return;
    }

    QString error = gitProcess->readAllStandardError();
    QString output = gitProcess->readAllStandardOutput();

    if (!error.isEmpty()) {
        QMessageBox::critical(this, "Git Error", error);
        return;
    }

    QMessageBox::information(this, "Git Output", output.isEmpty() ? "Switched branch successfully." : output);

    loadBranchesIntoListWidget();
}
