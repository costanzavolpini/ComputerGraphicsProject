/****************************************************************************
** simple image viewer using qt
****************************************************************************/

#ifndef GLRENDER_H
#define GLRENDER_H

#include "ui_GLRender.h"
#include "CCanvas.h"

class GLRender : public QMainWindow, private Ui::GLRender
{
    Q_OBJECT

public:
    GLRender(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~GLRender();

public slots:
    /************************************************************************/
    /* file operations                                                      */
    /************************************************************************/
    void loadImage();
    void saveImage();

private:
    void setupConnections();
    QString knownFileTypes;
};

#endif
