/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "imagewidget.h"

#include <qmessagebox.h>
#include <qpainter.h>
#include <qapplication.h>
#include <QFileDialog>
#include <QMouseEvent>

#include <iostream>


ImageWidget::ImageWidget( QWidget *parent)
    : QWidget( parent)
{
	scaleFactor_ = 1.0;
    setMouseTracking( TRUE );
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::setPixmap(const QPixmap& newPixmap)
{
    pmScaled_ = pm_ = newPixmap;

    if (pm_.size() == QSize( 0, 0))
		return;

    resize(pmScaled_.width(), pmScaled_.height());

}

QPixmap& ImageWidget::pixmap()
{
	return pm_;
}

void ImageWidget::scale(float factor)
{       
    QApplication::setOverrideCursor(Qt::WaitCursor);
	scaleFactor_ = factor;
	pmScaled_ = pm_.scaled(pm_.size() * factor);
	resize(pmScaled_.width(), pmScaled_.height());
	//repaint();

    QApplication::restoreOverrideCursor();
}



void ImageWidget::resizeEvent( QResizeEvent * )
{

}

void ImageWidget::paintEvent( QPaintEvent *e )
{
	QRect rect = e->rect();
//	std::cerr << "repaint event: (" << rect.x() << ", " << rect.y() << ") to ("
//									<< (rect.x() + rect.width()) << ", " << (rect.y() + rect.height()) << ")" << std::endl;
	
    if (pmScaled_.size() != QSize( 0, 0)) {		// is an image loaded?
	QPainter painter(this);
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled_);
    }
}





