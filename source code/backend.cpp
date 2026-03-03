
#include "pre_processing.h"
#include "yen_algorithm.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{}

QString Backend::getSourceInput() const
{
    QString newSource = QString::number(source);
    return newSource;
}

void Backend::setSourceInput(const QString &sourceInput)
{
    bool ok;
    int newSource = sourceInput.toInt(&ok);
    if (newSource != source)
    {
        source = newSource;
        emit sourceInputChanged();
    }
}

QString Backend::getDestinationInput() const
{
    QString newDestination = QString::number(destination);
    return newDestination;
}

void Backend::setDestinationInput(const QString &destinationInput)
{
    bool ok;
    int newDestination = destinationInput.toInt(&ok);
    if (newDestination != destination)
    {
        destination = newDestination;
        emit destinationInputChanged();
    }
}


QString Backend::getKInput() const
{
    QString newK = QString::number(k);
    return newK;
}

void Backend::setKInput(const QString &kInput)
{
    bool ok;
    int newK = kInput.toInt(&ok);
    if (newK != k) {
        k = newK;
        emit kInputChanged();
    }
}

QString Backend::gettextOutput() const
{
    QString newText = QString::fromStdString(text);
    return newText;
}

void Backend::setTextOutput(const QString &textOutput)
{
    string newText = textOutput.toStdString();
    if (newText != text)
    {
        text = newText;
        emit textOutputChanged();
    }
}

QString Backend::makeText(string text)
{
    int i;
    for (i = 0; (i < k); i++)
    {
        if (i+1==1)
        {
            text = text + "<b>The " + to_string(i + 1) + "st shortest path is </b>";
        }
        else if (i+1==2)
        {
            text = text + "<b>The " + to_string(i + 1) + "nd shortest path is </b>";
        }
        else if (i+1==3)
        {

            text = text + "<b>The " + to_string(i + 1) + "rd shortest path is </b>";
        }
        else
        {
            text = text + "<b>The " + to_string(i + 1) + "th shortest path is </b>";
        }
        for (int j = 0;j<(int)kShortestPaths[i]->trace.size();j++)
        {
            text += to_string(kShortestPaths[i]->trace[j]) + " ";
        }
        text = text + " which has the total length: " + to_string(kShortestPaths[i]->dist) + " km<br>";
    }
    text += "<br>";
    return QString::fromStdString(text);
}

QVector<QVector<QPointF> > Backend::getPaths() const
{
    return kShortestPathsPaint;
}

void Backend::setPaths(const QVector<QVector<QPointF> > &paths)
{
    emit pathsChanged();
}

void Backend::convertToPaths(QPointF *coordinate, Node **kShortestPaths, int numberOfPathsPaints, int numberOfVertices, QVector<QVector<QPointF> > &paths)
{
    paths.clear();

    // if (numberOfPathsPaints > 5)
    // {
    //     numberOfPathsPaints = 5;
    // }

    // make the constant lines (5 lines) should be drawn
    cout << numberOfVertices <<"aaa";
    paths.reserve(numberOfPathsPaints);

    for (int i = 0; i < numberOfPathsPaints; ++i) {
        QVector<QPointF> path;
        path.reserve(numberOfVertices);

        for (int j = 0; j <(int)kShortestPaths[i]->trace.size() ; ++j) {
            int vertexIndex = kShortestPaths[i]->trace[j];
            if (vertexIndex > 0 && vertexIndex < numberOfVertices) {
                path.append(coordinate[vertexIndex]);
            } else {
                qWarning() << "Vertex index" << vertexIndex << "is out of range.";
            }
        }
        if (!path.isEmpty())
        {
            paths.append(path);
        }
    }
}

void Backend::do_something()
{
    qDebug() << "Source:" << source;
    qDebug() << "Destination:" << destination;
    qDebug() << "K:" << k;

    //////////////////////////////////////////Preprocessing database of matrix of adajency//////////////////////////////////////////

    matrixAda = new float* [numberOfVertex];
    for (int i = 1; i < numberOfVertex; i++) {
        matrixAda[i] = new float[numberOfVertex];
    }

    //insert value to matrix
    for (int row = 1; row < numberOfVertex; row++) {
        for (int col = 1; col < numberOfVertex; col++) {
            matrixAda[row][col] = inf;
        }
    }

    // QString fileNameAdjacency = ":/adjacency_matrix.txt";

    // doReadDistance(fileNameAdjacency, matrixAda);

    /////////////////////////////////////////Processing database of matrix of coordiante//////////////////////////////////////////////

    coordinates = new QPointF[numberOfVertex];

    for (int i = 0; i < numberOfVertex; i++)
    {
        coordinates[i] = QPointF(0, 0); // hoặc bất kỳ giá trị mặc định nào bạn muốn
    }

    QString fileNameCoordinate = ":/list_coordinate_v2.txt";

    doReadCoordinates(fileNameCoordinate, coordinates, numberOfVertex);

    qDebug() << coordinates[1];

    QString fileNameIni_ter = ":/list_ini_ter.txt";
    calculateDistance(fileNameIni_ter, matrixAda, coordinates);


    int src = source;
    int des = destination;

    if (src < 1 || src > 69)
    {
        setTextOutput("The input of source is incorrect, please type again!");
        delete[] kShortestPaths;
        delete[] matrixAda;

        kShortestPaths = nullptr;
        matrixAda = nullptr;
        return;
    }

    if (des < 1 || des > 69)
    {
        setTextOutput("The input of destination is incorrect, please type again!");
        delete[] kShortestPaths;
        delete[] matrixAda;

        kShortestPaths = nullptr;
        matrixAda = nullptr;
        return;
    }

    if (k <= 0)
    {
        setTextOutput("The input of k is not typed, please type again!");
        delete[] kShortestPaths;
        delete[] matrixAda;

        kShortestPaths = nullptr;
        matrixAda = nullptr;
        return;
    }

    ///////////////////////////////////////////Handling the database///////////////////////////////////////////////////////////////////
    setTextOutput("");
    kShortestPathsPaint.clear();

    kShortestPaths = new Node*[k];
    for (int i=0;i<k;i++)
    {
        kShortestPaths[i] = new Node;

    }

    kShortestPaths = Yen(matrixAda, src, des, k);
    cout << k;
    if (k == 0)
    {
        setTextOutput("Cannot find the k shortest paths");
        QPointF point(0, 0);  // Create a QPointF with coordinates (0, 0)
        QVector<QPointF> path;
        path.append(point);  // Append the point to the first QVector<QPointF>
        kShortestPathsPaint.append(path);
        setPaths(kShortestPathsPaint);

        delete[] kShortestPaths;
        delete[] matrixAda;

        kShortestPaths = nullptr;
        matrixAda = nullptr;

        return;
    }

    convertToPaths(coordinates, kShortestPaths, k, numberOfVertex, kShortestPathsPaint);
    setPaths(kShortestPathsPaint);

    QString newText = makeText(text);
    setTextOutput(newText);

    delete[] kShortestPaths;
    delete[] matrixAda;
    delete[] coordinates;

    kShortestPaths = nullptr;
    matrixAda = nullptr;
    coordinates = nullptr;

    return;
}
