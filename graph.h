#ifndef GRAPH_H
#define GRAPH_H
#include <QPoint>
#include <QtMath>

class graph
{
	explicit graph(int size);
public:
    graph();
	graph(const graph &other);
	graph& operator= (const graph &other);
	~graph();
	QPoint* getArray() const;
	void add(const QPoint &punto);
	void swap(graph &other);
	int getSize() const;
	int getAdjMatrix(int i, int j) const;
	void setAdjMatrix(int i, int j, int val);
	int randomNumber(int min, int max);
	int maxInteractionNode(unsigned int n) const;
	void printAdjMatrix() const;
	const unsigned int maxNodi;
	unsigned int getTotalInteractions() const;
	void setTotalInteractions(unsigned int totalInteractions);

private:
	QPoint *_punti;
	unsigned int _size;									//numero di nodi salvati nel grafo
	unsigned int _adjMatrix[30][30];					//matrice di adiacenza
	unsigned int _totalInteractions;					//conteggio delle interazioni totali del grafo
};

#endif // GRAPH_H
