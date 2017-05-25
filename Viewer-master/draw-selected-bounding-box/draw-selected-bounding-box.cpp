#include "draw-selected-bounding-box.h"
#include <cstdlib>

void DrawSelectedBox::onPluginLoad() {
    glwidget()->makeCurrent();
    // Carregar shader, compile & link
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/draw-selected-bounding-box/template.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/draw-selected-bounding-box/template.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();

    /*const float normals[] = {
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,

        0,0,-1,
        0,0,-1,
        0,0,-1,
        0,0,-1,

        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,

        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,

        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,

        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0
    };*/

    /*const float vertex[] = {
        -1, 1,-1,
         1, 1,-1,
        -1, 1, 1,
         1, 1, 1,

        -1, 1,-1,
         1, 1,-1,
        -1,-1,-1,
         1,-1,-1,

        -1, 1, 1,
         1, 1, 1,
        -1,-1, 1,
         1,-1, 1,

        -1, 1,-1,
        -1, 1, 1,
        -1,-1,-1,
        -1,-1, 1,

         1, 1,-1,
         1, 1, 1,
         1,-1,-1,
         1,-1, 1,

         1,-1,-1,
        -1,-1,-1,
         1,-1, 1,
        -1,-1, 1
    };*/

    const float normals[] = {
        0,-1,0,
        0,-1,0,
        0,-1,0,
        0,-1,0,

        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,

        -1,0,0,
        -1,0,0,
        -1,0,0,
        -1,0,0,

        0,0,-1,
        0,0,-1,
        0,0,-1,
        0,0,-1,

        1,0,0,
        1,0,0,
        1,0,0,
        1,0,0,

        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1
    };

    const float vertex[] = {
            //abajo
            0, 0, 0,
            1, 0, 0,
            0, 0, 1,
            1, 0, 1,

            //arriba
            0, 1, 0,
            1, 1, 0,
            0, 1, 1,
            1, 1, 1,

            //izquierda
            0, 0, 0,
            0, 1, 0,
            0, 0, 1,
            0, 1, 1,

            //delante
            0, 0, 0,
            0, 1, 0,
            1, 0, 0,
            1, 1, 0,

            //derecha
            1, 0, 0,
            1, 1, 0,
            1, 0, 1,
            1, 1, 1,

            //fondo
            0, 0, 1,
            0, 1, 1,
            1, 0, 1,
            1, 1, 1

     };

    const uint index[] = {
         0, 1, 2,
         1, 2, 3,

         4, 5, 6,
         5, 6, 7,

         8, 9,10,
         9,10,11,

        12,13,14,
        13,14,15,

        16,17,18,
        17,18,19,

        20,21,22,
        21,22,23
    };

    GLWidget &g = *glwidget();

    g.glGenVertexArrays(1, &normalizedBoxVAO);

    GLuint vertexVBO;
    g.glGenBuffers(1, &vertexVBO);
    GLuint normalVBO;
    g.glGenBuffers(1, &normalVBO);
    GLuint indexBufferID;
    g.glGenBuffers(1, &indexBufferID);

    g.glBindVertexArray(normalizedBoxVAO);

    g.glBindBuffer(GL_ARRAY_BUFFER, vertexVBO); // VAO
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);

    g.glBindBuffer(GL_ARRAY_BUFFER, normalVBO); // VAO
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(1);

    g.glBindBuffer(GL_ARRAY_BUFFER, vertexVBO); // VAO
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);

    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    g.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    g.glBindVertexArray(0);
}

void DrawSelectedBox::postFrame() {
    int selected = scene()->selectedObject();
    if (selected == -1) return;
    GLWidget &g = *glwidget();
    g.makeCurrent();
    program->bind();
    program->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    program->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix() * camera()->viewMatrix());
    const Object& object = scene()->objects()[selected];
    program->setUniformValue("boundingBoxMax", object.boundingBox().max());
    program->setUniformValue("boundingBoxMin", object.boundingBox().min());
    g.glBindVertexArray(normalizedBoxVAO);
    g.glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);
    program->release();
    g.glBindVertexArray(0);
}


#if QT_VERSION < 0x040000
Q_EXPORT_PLUGIN2(draw-selected-bounding-box, DrawSelectedBox)   // plugin name, plugin class
#endif

