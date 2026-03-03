#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "main.h"

class Node {
public:
    vector<int> trace;
    float dist;
    Node() : dist(0) {}
};

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sourceInput READ getSourceInput WRITE setSourceInput NOTIFY sourceInputChanged)
    Q_PROPERTY(QString destinationInput READ getDestinationInput WRITE setDestinationInput NOTIFY destinationInputChanged)
    Q_PROPERTY(QString kInput READ getKInput WRITE setKInput NOTIFY kInputChanged)
    Q_PROPERTY(QString textOutput READ gettextOutput WRITE setTextOutput NOTIFY textOutputChanged)
    Q_PROPERTY(QVector<QVector<QPointF>> paths READ getPaths WRITE setPaths NOTIFY pathsChanged)

public:
    explicit Backend(QObject *parent = nullptr);

    // Getter for sourceInput
    QString getSourceInput() const;

    // Setter for sourceInput
    void setSourceInput(const QString &sourceInput);

    // Getter for destinationInput
    QString getDestinationInput() const;

    // Setter for destinationInput
    void setDestinationInput(const QString &destinationInput);

    // Getter for kInput
    QString getKInput() const;

    // Setter for kInput
    void setKInput(const QString &kInput);

    // Getter for textOutput
    QString gettextOutput() const;

    // Setter for textOutput
    void setTextOutput(const QString &textOutput);

    QString makeText(string text);

    QVector<QVector<QPointF>> getPaths() const;

    void setPaths(const QVector<QVector<QPointF>> &paths);

    void convertToPaths(QPointF *coordinate, Node **kShortestPaths, int numberOfPaths, int numberOfVertices, QVector<QVector<QPointF>> &paths);

signals:
    void sourceInputChanged();
    void destinationInputChanged();
    void kInputChanged();
    void textOutputChanged();
    void pathsChanged();


public slots:
    void do_something();

protected:
    int source;
    int destination;
    int k;

    float** matrixAda = nullptr;
    Node** kShortestPaths = nullptr;
    string text = "";
    QVector<QVector<QPointF>> kShortestPathsPaint;
    QPointF *coordinates = nullptr;
};

#endif // BACKEND_H
