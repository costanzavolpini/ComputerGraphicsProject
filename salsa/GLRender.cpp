/************************************************************************/
/* simple image viewer using qt                                         */
/************************************************************************/

#include <QtGui>
#include "GLRender.h"
#include <iostream>

/************************************************************************/
/* setup the UI                                                         */
/************************************************************************/
GLRender::GLRender(QWidget *parent,Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setupUi(this);

    setupConnections();

    knownFileTypes = QString("Image Files ( *.jpg *.png *.bmp *.tiff )");
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
GLRender::~GLRender()
{ 
    // if (canvas) delete canvas;
}

/************************************************************************/
/* setup connections                                                    */
/************************************************************************/
void GLRender::setupConnections() 
{
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(loadImage()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveImage()));
}

/************************************************************************/
/* load an Image                                                        */
/************************************************************************/
void GLRender::loadImage()
{
    QString fname = QFileDialog::getOpenFileName(this, "Open File", ".", knownFileTypes);

    if(!fname.isEmpty()) { /* canvas->load(fname); */ }
    else QMessageBox::critical(this, "Error Message", "error loading file");
}

/************************************************************************/
/* save an Image                                                        */
/************************************************************************/
void GLRender::saveImage()
{  
    QString fname = QFileDialog::getSaveFileName(this, "Save File", ".", knownFileTypes);

    if(!fname.isEmpty()) { /* canvas->save(fname); */ }
    else QMessageBox::critical(this, "Error Message", "error saving mesh");
}
