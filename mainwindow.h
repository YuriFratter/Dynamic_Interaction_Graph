#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QtMath>
#include "graph.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	virtual void paintEvent(QPaintEvent *event);
	graph grafo;
	int getWindowX() const;
	int getWindowY() const;
	const int dimension;							//Dimensione in pixel dei nodi
	const int minDistance;							//distanza minima dal centro di un nodo al centro di un altro

public slots:
	void iterationUpdate();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
	const int _windowX;								//Larghezza della MainWindow
	const int _windowY;								//Altezza della MainWindow
	QColor from;									//Colore di partenza nella scala cromatica dei nodi
	QColor to;										//Colore massimo nella scala cromatica dei nodi
};
#endif // MAINWINDOW_H
