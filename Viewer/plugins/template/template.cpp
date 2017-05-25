#include "template.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

void Template::onPluginLoad()
{
    // Link program
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    qDebug() << program->log() << endl;

    // Timers
    connect(&timer, SIGNAL(timeout()), glwidget(), SLOT(updateGL()));
    timer.start();
    elapsedTimer.start();
}


void Template::onObjectAdd()
{

}

void Template::preFrame()
{
}

void Template::postFrame()
{
    
}

bool Template::drawScene()
{
    // See example DrawImmediate and DrawVBO examples
    // If you draw the scene here, you should return true
    return false;
}
	
void Template::keyPressEvent (QKeyEvent *e)
{
    // if (e->key() == Qt::Key_C) ...
    qDebug() << "User pressed key " << e->key() << endl; 
    glwidget()->updateGL();
}

void Template::keyReleaseEvent (QKeyEvent *)
{}

void Template::mouseMoveEvent (QMouseEvent *) 
{}

void Template::mousePressEvent (QMouseEvent *e)
{   
    qDebug() << "Mouse (x,y) = " << e->x() << ", " << e->y() << endl;
}

void Template::mouseReleaseEvent (QMouseEvent *)
{}

void Template::wheelEvent (QWheelEvent *)
{}


bool Template::paintGL()
{
    // clear buffers
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    // setup matrices
    camera()->setOpenGLModelviewMatrix();
    camera()->setOpenGLProjectionMatrix();
    // draw axes
    glwidget()->drawAxes();
    // draw scene with shaders
    program->bind();
    program->setUniformValue("time", float(elapsedTimer.elapsed()/1000.));
    drawPlugin()->drawScene();
    program->release();

    return true;
}

