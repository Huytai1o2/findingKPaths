#ifndef PRE_PROCESSING_H
#define PRE_PROCESSING_H

#include "main.h"

// void createMatrixAdj(float **matrixAda)
// {
//     matrixAda = new float* [numberOfVertex];
//     for (int i = 0; i < numberOfVertex; i++) {
//         matrixAda[i] = new float[numberOfVertex];
//     }

//     //insert value to matrix
//     for (int row = 0; row < numberOfVertex; row++) {
//         for (int col = 0; col < numberOfVertex; col++) {
//             if (row == col) matrixAda[row][col] = 0;
//             else matrixAda[row][col] = inf;
//         }
//     }
// }

void doReadDistance(QString fileName, float **matrixAda)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open the file:" << fileName;
        return;
    }

    QTextStream in(&file);
    int ini_vertex = -1, des_vertex = -1;
    float distance = inf;

    while (!in.atEnd()) {
        in >> ini_vertex >> des_vertex >> distance;
        if (in.status() == QTextStream::Ok) {
            if (ini_vertex >= 0 && ini_vertex < numberOfVertex && des_vertex >= 0 && des_vertex < numberOfVertex) {
                matrixAda[ini_vertex][des_vertex] = distance;
            } else {
                qWarning() << "The nummeric of vertex is not valid" << ini_vertex << "or" << des_vertex;
            }
        } else {
            qWarning() << "There is a wrong in reading file";
            break;
        }
    }

    file.close();
}

void doReadCoordinates(QString fileName, QPointF *coordinates, int numberOfVertices)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open the file:" << fileName;
        return;
    }

    QTextStream in(&file);
    int vertexIndex = -1;
    qreal x = 0.0, y = 0.0;

    while (!in.atEnd()) {
        in >> vertexIndex >> x >> y;
        if (in.status() == QTextStream::Ok) {
            if (vertexIndex >= 0 && vertexIndex < numberOfVertices) {
                coordinates[vertexIndex] = QPointF(x, y);
            } else {
                qWarning() << "The vertex index is not valid" << vertexIndex;
            }
        } else {
            qWarning() << "There was an error reading the file";
            break;
        }
    }

    file.close();
}

void calculateDistance(QString fileNameIni_ters, float **matrixAda, QPointF* coordinates)
{
    QFile file(fileNameIni_ters);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open the file:" << fileNameIni_ters;
        return;
    }

    QTextStream in(&file);
    // get line
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList part = line.split(" ");
        // there are many ters in each line


        if (!part.isEmpty())
        {
            int ini_vertex = part[0].toInt();
            QList<int> ter_vertex;
            for (int i = 1; i < part.size(); i++) {
                ter_vertex.push_back(part[i].toInt());
            }

            for (int i = 0; i < ter_vertex.size(); i++) {
                matrixAda[ini_vertex][ter_vertex[i]] = 2.2634 / 1000 * sqrt(pow(coordinates[ini_vertex].x() - coordinates[ter_vertex[i]].x(), 2) +
                                                            pow(coordinates[ini_vertex].y() - coordinates[ter_vertex[i]].y(), 2));
            }
        }
    }
}

#endif // PRE_PROCESSING_H
