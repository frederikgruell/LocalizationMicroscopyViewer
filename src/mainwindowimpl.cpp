#include "mainwindowimpl.h"

#include "datapoint.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QProgressDialog>
#include <QTime>

#include <iostream>
#include <vector>

//
MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);

	connect(actionOpen, SIGNAL(triggered()), this, SLOT(openClicked()));
	connect(actionSave_as, SIGNAL(triggered()), this, SLOT(saveAsClicked()));
	connect(actionClose, SIGNAL(triggered()), this, SLOT(closeClicked()));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	
	connect(actionZoom_In, SIGNAL(triggered()), locmicview, SLOT(zoomIn()));
	connect(actionZoom_Out, SIGNAL(triggered()), locmicview, SLOT(zoomOut()));
	connect(actionActual_Size, SIGNAL(triggered()), locmicview, SLOT(actualSize()));
	connect(actionZoom_to_Fit, SIGNAL(triggered()), locmicview, SLOT(zoomToFit()));
	
	connect(locmicview, SIGNAL(zoomInEnabled(bool)), actionZoom_In, SLOT(setEnabled(bool)));
	connect(locmicview, SIGNAL(zoomOutEnabled(bool)), actionZoom_Out, SLOT(setEnabled(bool)));
	
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(aboutClicked()));
	
    statisticsStatusLabel_ = new QLabel();
    statusBar()->addPermanentWidget(statisticsStatusLabel_, 1);

//	std::cerr << "main window initialized" << std::endl;
}


void MainWindowImpl::openClicked()
{
	//static QBrush whiteBrush = QBrush(Qt::white);
	
//	std::cerr << "open was clicked" << std::endl;
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".",  "CSV/TSV (*.csv *.tsv);; All files (*.*)");
	if(fileName.isEmpty()) return;
	
	QCoreApplication::processEvents();
	
//	std::cerr << "fileName: " << fileName.toAscii().data() << std::endl;
	QFile inputFile(fileName);
	
	if(!inputFile.open(QIODevice::ReadOnly)) {
		QMessageBox msgBox;
		msgBox.setText(QString("Could not open file '") + fileName + "' for reading");
		msgBox.exec();
//		std::cerr << "exec msgBox" << std::endl;
		return;
	}

    statisticsStatusLabel_->setText("");

	QTextStream inputStream(&inputFile);
	QString line;
	QStringList list;

	std::vector<DataPoint> dataPoints;
	float minX = 0;
	float minY = 0;
	float maxX = 0;
	float maxY = 0;

	QProgressDialog progress("Reading data", QString(), 0, 0, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setValue(0);
	progress.open();
	
	int lineNr = 1;
	QTime timer;
	timer.start();
	while(!inputStream.atEnd()) {
		line = inputStream.readLine();
		//std::cerr << "line: " << line.toAscii().data() << std::endl;
		//std::cerr << "list: " << list.join("; ").toAscii().data() << std::endl;
		//list = line.split(QRegExp("(\\s+)|;|\\t"));
		list = line.split(QRegExp("[\\t,\\,]+"));		
		
		if(list.size() < 9) {
//			std::cerr << "invalid size " << list.size() << " in string list " << list.join("; ").toAscii().data() << std::endl;
//			std::cerr << "line: " << line.toAscii().data() << std::endl;
			//return;
			continue;
		}
		//
//		float maxQ     = list[0].toFloat();
		float y        = list[1].toFloat();
		float x        = list[2].toFloat();
		float delta_y  = list[3].toFloat();
		float delta_x  = list[4].toFloat();
//		float sigma_y  = list[5].toFloat();
//		float sigma_x  = list[6].toFloat();
//		float Q        = list[7].toFloat();
//		int   imgNr    = list[8].toInt();

		minX = std::min(minX, x);
		minY = std::min(minY, y);
		maxX = std::max(maxX, x);
		maxY = std::max(maxY, y);
		
		//dataPoints.insert(std::pair<int, DataPoint>(lineNr, DataPoint(x, y, delta_x, delta_y)));
		dataPoints.push_back(DataPoint(x, y, delta_x, delta_y));

		if(lineNr % 10000 == 0) {
			QCoreApplication::processEvents();
		}
		lineNr++;
	}
//	std::cerr << lineNr << " items added" << std::endl;
	
	inputFile.close();
	
	locmicview->newPixmap(QRectF(minX, minY, maxX - minX, maxY - minY));
	locmicview->addDataPoints(dataPoints);
	locmicview->showNewPixmap();
//	std::cerr << "time  : " << timer.elapsed() << std::endl;

    statisticsStatusLabel_->setText(QString("%1 signals drawn")
                           .arg(lineNr)
           );
}


void MainWindowImpl::saveAsClicked()
{
	//static QBrush whiteBrush = QBrush(Qt::white);
	
//	std::cerr << "save was clicked" << std::endl;
	
	QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".",  "PNG (*.png);; All files (*.*)");
//	std::cerr << "saving to " << fileName.toStdString() << std::endl;
	if(fileName.isEmpty()) return;
	
	locmicview->saveImage(fileName);
}

void MainWindowImpl::closeClicked()
{
//	std::cerr << "close was clicked" << std::endl;
	
    statisticsStatusLabel_->setText("");
	locmicview->closeImage();	
}


void MainWindowImpl::aboutClicked()
{
	QMessageBox::about(this,
		QString("About Localization Microscopy Viewer"),
		QString::fromUtf8("Version 0.1\n"
		"Frederik Grüll <Frederik.Gruell@kip.uni-heidelberg.de>\n"
		"Manfred Kirchgessner <mkirchge@kip.uni-heidelberg.de>")); 
}

//////////////////////////////////////////////////////////
