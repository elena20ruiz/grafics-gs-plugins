#include "draw-smooth.h"
#include "glwidget.h"

void DrawSmooth::onPluginLoad() {
    if (scene()->objects().size() > 0) onObjectAdd();
}

void DrawSmooth::onObjectAdd() {
    Object& object = scene()->objects()[scene()->objects().size()-1];

    vector<GLuint> indices(3*object.faces().size());
    vector<float> vertices(3*object.vertices().size());
    vector<float> normals(3*object.vertices().size());
    vector<float> colors(3*object.vertices().size());
    //vector<list<GLuint>> VF(object.vertices().size());
    vector<Vector> avgNormals(object.vertices().size());
    vector<unsigned int> numIncidence(object.vertices().size());

    for (unsigned int i = 0, k = 0; i < object.faces().size(); ++i) {
        const Face& face = object.faces()[i];
        if (face.numVertices() != 3) {
            cout << "Object has one (or more) face that is not a triangle" << endl;
            return;
        }
        Vector normal = face.normal();
        for (int j = 0; j < face.numVertices(); ++k, ++j) {
            int index = face.vertexIndex(j);
            indices[k] = index;
            //VF[index].push_back(i);
            avgNormals[index] += normal;
            ++numIncidence[index];
        }
    }
    unsigned int i = 0;
    for (Vertex& vertex : object.vertices()) {
        Point p = vertex.coord();
        vertices[i] = p.x();
        vertices[i+1] = p.y();
        vertices[i+2] = p.z();
        i += 3;
    }
    for (i = 0; i < object.vertices().size(); ++i) {
        Vector average = avgNormals[i]/numIncidence[i];
        Vector normal = average.normalized();

        normals[3*i] = average.x();
        normals[3*i+1] = average.y();
        normals[3*i+2] = average.z();

        colors[3*i] = abs(normal.x());
        colors[3*i+1] = abs(normal.y());
        colors[3*i+2] = abs(normal.z());
    }
    /*for (unsigned int v = 0; v < VF.size(); ++v) {
        list<GLuint>& faces = VF[v];
        QVector3D average;
        for (GLuint faceIndex : faces) {
            average += object.faces()[faceIndex].normal();
        }
        average /= faces.size();
        QVector3D normal = average.normalized();

        normals[3*v] = average.x();
        normals[3*v+1] = average.y();
        normals[3*v+2] = average.z();

        colors[3*v] = abs(normal.x());
        colors[3*v+1] = abs(normal.y());
        colors[3*v+2] = abs(normal.z());
    }*/
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

    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    g.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

    g.glBindVertexArray(0);
    g.glBindBuffer(GL_ARRAY_BUFFER,0);
    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    VAOs.push_back(VAO);
    numIndices.push_back(indices.size());
    vertexBuffer.push_back(vertexVBO);
    normalBuffer.push_back(normalVBO);
    colorBuffer.push_back(colorVBO);
    indexBuffer.push_back(indexBufferID);
}

void DrawSmooth::cleanUp() {
    GLWidget &g = *glwidget();
    g.glDeleteBuffers(vertexBuffer.size(), &vertexBuffer[0]);
    g.glDeleteBuffers(normalBuffer.size(), &normalBuffer[0]);
    g.glDeleteBuffers(colorBuffer.size(), &colorBuffer[0]);
    g.glDeleteBuffers(indexBuffer.size(), &indexBuffer[0]);

    g.glDeleteVertexArrays(VAOs.size(), &VAOs[0]);

    vertexBuffer.clear();
    normalBuffer.clear();
    colorBuffer.clear();
    indexBuffer.clear();
    numIndices.clear();
}

DrawSmooth::~DrawSmooth() {
    cleanUp();
}

void DrawSmooth::onSceneClear() {
    cleanUp();
}

bool DrawSmooth::drawObject(int i) {
    GLWidget &g = *glwidget();
    g.glBindVertexArray(VAOs[i]);
    g.glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);

    g.glBindVertexArray(0);
    return true;
}

bool DrawSmooth::drawScene() {
    GLWidget &g = *glwidget();
    g.makeCurrent();

    for(unsigned int i=0; i < VAOs.size(); ++i) {
        g.glBindVertexArray(VAOs[i]);
        g.glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, 0);
    }
    g.glBindVertexArray(0);
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(draw-mooth, DrawSmooth)   // plugin name, plugin class
#endif



