#include "draw-flat.h"
#include "glwidget.h"

void DrawFlat::onPluginLoad() {
    if (scene()->objects().size() > 0) onObjectAdd();
}

void DrawFlat::onObjectAdd() {
    Object& object = scene()->objects()[scene()->objects().size()-1];
    const vector<Vertex>& mVertices = object.vertices();

    vector<float> vertices(3*3*object.faces().size());
    vector<float> normals(3*3*object.faces().size());
    vector<float> colors(3*3*object.faces().size());

    for (unsigned int i = 0, k = 0; i < object.faces().size(); ++i) {
        const Face& face = object.faces()[i];
        Vector normal = face.normal();
        Vector normalNormalized = normal.normalized();
        if (face.numVertices() != 3) {
            cout << "Object has one (or more) face that is not a triangle" << endl;
            return;
        }
        for (int j = 0; j < face.numVertices(); k += 3, ++j) {
            int index = face.vertexIndex(j);
            Point coords = mVertices[index].coord();
            vertices[k] = coords.x();
            vertices[k+1] = coords.y();
            vertices[k+2] = coords.z();

            normals[k] = normal.x();
            normals[k+1] = normal.y();
            normals[k+2] = normal.z();

            colors[k] = abs(normalNormalized.x());
            colors[k+1] = abs(normalNormalized.y());
            colors[k+2] = abs(normalNormalized.z());
        }
    }
    GLuint VAO, vertexVBO, normalVBO, indexBufferID, colorVBO;

    GLWidget &g = *glwidget();

    g.makeCurrent();

    g.glGenVertexArrays(1, &VAO);
    g.glGenBuffers(1, &vertexVBO);
    g.glGenBuffers(1, &normalVBO);
    g.glGenBuffers(1, &colorVBO);
    g.glGenBuffers(1, &indexBufferID);

    g.glBindVertexArray(VAO);

    g.glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);

    g.glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(1);

    g.glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colors.size(), &colors[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(2);

    g.glBindVertexArray(0);
    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    VAOs.push_back(VAO);
    numIndices.push_back(3*object.faces().size());
    vertexBuffer.push_back(vertexVBO);
    normalBuffer.push_back(normalVBO);
    colorBuffer.push_back(colorVBO);
}

void DrawFlat::cleanUp() {
    GLWidget &g = *glwidget();
    g.glDeleteBuffers(vertexBuffer.size(), &vertexBuffer[0]);
    g.glDeleteBuffers(normalBuffer.size(), &normalBuffer[0]);
    g.glDeleteBuffers(colorBuffer.size(), &colorBuffer[0]);

    g.glDeleteVertexArrays(VAOs.size(), &VAOs[0]);

    vertexBuffer.clear();
    normalBuffer.clear();
    colorBuffer.clear();
    numIndices.clear();
}

DrawFlat::~DrawFlat() {
    cleanUp();
}

void DrawFlat::onSceneClear() {
    cleanUp();
}

bool DrawFlat::drawObject(int i) {
    GLWidget &g = *glwidget();
    g.glBindVertexArray(VAOs[i]);
    g.glDrawArrays(GL_TRIANGLES, 0, numIndices[i]);

    g.glBindVertexArray(0);
    return true;
}

bool DrawFlat::drawScene() {
    GLWidget &g = *glwidget();
    g.makeCurrent();

    for(unsigned int i=0; i < VAOs.size(); ++i) {
        g.glBindVertexArray(VAOs[i]);
        g.glDrawArrays(GL_TRIANGLES, 0, numIndices[i]);
    }
    g.glBindVertexArray(0);
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(draw-flat, DrawFlat)   // plugin name, plugin class
#endif



