#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
//

#include "datapoint.h"

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
public slots:
private slots:
	void openClicked();
	void saveAsClicked();
	void closeClicked();
	void aboutClicked();

private:
    QLabel *statisticsStatusLabel_;
};
#endif




