#include "draw-bounding-box.h"
#include <cstdlib>

void DrawBox::onPluginLoad() {
    glwidget()->makeCurrent();
    // Carregar shader, compile & link
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/draw-bounding-box/template.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/draw-bounding-box/template.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();

    const float normals[] = {
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
    };

    const float vertex[] = {
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

void DrawBox::postFrame() {
    GLWidget &g = *glwidget();
    g.makeCurrent();
    program->bind();
    program->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    program->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix() * camera()->viewMatrix());
    for (Object& object : scene()->objects()) {
        program->setUniformValue("boundingBoxMax", object.boundingBox().max());
        program->setUniformValue("boundingBoxMin", object.boundingBox().min());
        g.glBindVertexArray(normalizedBoxVAO);
        g.glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    program->release();
    g.glBindVertexArray(0);
}


#if QT_VERSION < 0x040000
Q_EXPORT_PLUGIN2(draw-bounding-box, DrawBox)   // plugin name, plugin class
#endif

