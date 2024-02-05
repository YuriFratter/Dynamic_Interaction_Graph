#include "graph.h"
#include <QDebug>
#include <iostream>


//attenzione all'ultima riga: la matrice è DA SUBITO grande 30x30, anche con 0 nodi.
graph::graph() : maxNodi(30), _punti(nullptr), _size(0), _totalInteractions(0)
	{
		for(int i=0; i<30; ++i)
			for(int j=0; j<30; ++j)
				_adjMatrix[i][j] = 0;
	}


//costruttore del grafo con parametro size, è un metodo privato usato solo all'interno della classe
//inizializza un grafo ad una certa grandezza, con tutti i relativi parametri in uno stato coerente.
graph::graph(int size) : maxNodi(30), _punti(nullptr), _size(0), _totalInteractions(0)
{
	if(_size < maxNodi)
		_punti = new QPoint[size];
	_size = size;
	for(int i=0; i<30; ++i)
		for(int j=0; j<30; ++j)
			_adjMatrix[i][j] = 0;
}


//Copy constructor del grafo, lavora su un grafo temporaneo per impedire che eventuali errori diano uno stato
//non coerente, quando ha fatto tutte le modifiche necessarie viene chiamata la swap che aggiorna il grafo corrente.
graph::graph(const graph &other) : maxNodi(30), _punti(nullptr), _size(0), _totalInteractions(0)
{
	graph tmp(other._size);
	for(unsigned int i=0; i<other._size; ++i)
	{
		tmp._punti[i] = other._punti[i];
	}
	for(int i=0; i<30; ++i)
		for(int j=0; j<30; ++j)
			tmp._adjMatrix[i][j] = other._adjMatrix[i][j];
	tmp._totalInteractions = _totalInteractions;
	swap(tmp);
}


//Operatore di assegnamento del grafo, copia le informazioni di un altro grafo in una fariabile temporanea e se non ci sono
//errori usa la swap per aggiornarle con il grafo corrente.
graph& graph::operator= (const graph &other)
{
	graph tmp(other);
	swap(tmp);
	return *this;
}


//Distruttore del grafo. Si occupa di liberare la memoria che è stata allocata dal grafo.
graph::~graph()
{
	delete [] _punti;
}


//Metodo che restituisce un puntatore all'array dei punti, è utile per ottenere le informazioni
//su di essi come le loro coordinate.
QPoint* graph::getArray() const
{
	return _punti;
}


/*
Metodo per l'aggiunta di nodi al grafo; se viene raggiunto il massimo di 30 nodi
non fa nulla, altrimenti copia tutti i nodi correnti in un nuovo grafo temporaneo ricavato
con il costruttore secondario che riceve un int come parametro in ingresso (tmp) in modo
da ricavare lo spazio aggiuntivo per un nuovo nodo per poi alla fine sovrascrivere il nodo nuovo
con i valori fittizi che aveva usato il costruttore per inizializzare l'array.
Viene anche copiata la matrice di adiacenza per mantenere i dati quando alla fine si andrà ad
eseguire una swap con l'oggetto corrente e scambiare i dati aggiornati
*/
void graph::add(const QPoint &punto)
{
	graph tmp(_size + 1);
	if(_size <maxNodi)
	{
		for(unsigned int i=0; i<_size; ++i)
		{
			tmp._punti[i] = _punti[i];
		}
		tmp._punti[_size] = punto;
		for(int i=0; i<30; ++i)
			for(int j=0; j<30; ++j)
				tmp._adjMatrix[i][j] = _adjMatrix[i][j];
		swap(tmp);
		//qDebug() << "Aggiunto nodo: " << "(" << punto.x() << ", " << punto.y() << ")";
	}
}


/*
Funzione swap per scambiare i dati membro tra il grafo corrente ed un altro grafo.
*/
void graph::swap(graph &other)
{
	std::swap(_punti, other._punti);
	std::swap(_size, other._size);
	std::swap(_adjMatrix, other._adjMatrix);
	std::swap(_totalInteractions, other._totalInteractions);
}


//Funzione che restituisce la _size privata del grafo corrente.
int graph::getSize() const
{
		return _size;
}


//Funzione che restituisce il valore contenuto nella cella [i][j] indicata dai parametri in ingresso
int graph::getAdjMatrix(int i, int j) const
{
	return _adjMatrix[i][j];
}


//Funzione che altera il valore contenuto nella cella [i][j] indicata dai parametri in ingresso
//sostituendoli con un nuovo dato, quello passato come parametro "val"
void graph::setAdjMatrix(int i, int j, int val)
{
	_adjMatrix[i][j] = val;
}


//Funzione che genera un numero casuale entro un range dato dall'utente.
//Usata per la generazione di numeri randomici di nodi e archi.
int graph::randomNumber(int min, int max)
{
	return rand()%(max-min+1)+min;
}


//Funzione che calcola il numero massimo di interazioni che un nodo ha tra tutte le interazioni
//attualmente salvate e restituisce questo numero al chiamante, è usato dal chiamante per aggiornare
//i valori della diagonale.
int graph::maxInteractionNode(unsigned int n) const
	{
		unsigned int maxRiga = 0;
		unsigned int maxColonna = 0;
		for(unsigned int i=0; i<n; ++i)
			if(_adjMatrix[n][i] > maxRiga)
				maxRiga = _adjMatrix[n][i];

		for(unsigned int j=n+1; j>n && j<_size; ++j)
			if(_adjMatrix[j][n] > maxColonna)
				maxColonna = _adjMatrix[j][n];
		if(maxRiga >= maxColonna)
			return maxRiga;
		else
			return maxColonna;
	}


//Metodo utile in fase di debug, stampa la matrice di adiacenza. Per comodità ed utilità si ferma
//in corrispondenza della size (se il grafo ha n nodi dove n<30 la stampa darà una matrice n x n
//perché anche se la matrice è da subito inizializzata a 30 x 30 caselle quelle fuori dal riquadro
//size x size sono tutti zeri e non ci interessano.
void graph::printAdjMatrix() const
	{
		QString s = "";
		for(unsigned int i=0; i<_size; ++i)
		{
			for(unsigned int j=0; j<_size; ++j)
			{
				s += QString::number(_adjMatrix[i][j]) + ", ";
			}
			qDebug() << s;
			s = "";
		}

	}


//Metodo che restituisce il numero totale di interazioni che il grafo ha salvate: utile per non
//rischiare di creare più archi di quanti possano essercene tra i nodi esistenti, va sottratto alle
//combinazioni possibili di archi.
//Spesso il risultato è molto grande e quindi viene comunque ridotto per non creare confusione visivamente.
unsigned int graph::getTotalInteractions() const
	{
		return _totalInteractions;
	}


//Metodo che serve ad aggiornare la variabile privata che rappresenta il numero totale di interazioni nel grafo.
void graph::setTotalInteractions(unsigned int totalInteractions)
	{
		_totalInteractions = totalInteractions;
	}

