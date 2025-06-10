#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_path_pushButton_clicked();
    void on_command_pushButton_clicked();
    void on_treeView_activated(const QModelIndex &index);

    void on_switchButton_clicked();

private:
    void loadBranchesIntoListWidget();

    Ui::MainWindow *ui;
    QProcess *gitProcess;
    QString projectPath;
    QFileSystemModel *fileModel;
};

#endif // MAINWINDOW_H
