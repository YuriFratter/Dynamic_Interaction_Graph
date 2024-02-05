#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "graph.h"


//costruttore della MainWindow, inizializza tutti i parametri privati e avvia il timer
//che serve ad aggiornare ogni secondo il grafo e la sua rappresentazione grafica
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, dimension(15)
	, minDistance(dimension/2+35)
	, ui(new Ui::MainWindow)
	, _windowX(800)
	, _windowY(600)
	, from(248, 181, 20)
	, to(64, 150, 214)

{
    ui->setupUi(this);
    timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(iterationUpdate()));
	timer->start(1000);
}


//Distruttore della MainWindow
MainWindow::~MainWindow()
{
	delete timer;
    delete ui;
}


/*
Paintevent, questo metodo si usa per aggiornare graficamente il grafo.
L'oggetto grafo ha già le informazioni aggiornate dal metodo "iterationUpdate()"
ma non sono ancora visibili a schermo.
Questo metodo si occupa di ridisegnare ogni volta tutto il grafo aggiornandolo con le
nuove informazioni.
*/
void MainWindow::paintEvent(QPaintEvent *)
{
	/*
	Nella prima sezione si disegnano i cerchi con i rispettivi colori (scelti ogni volta
	dalla diagonale della matrice contenente il massimo delle interazioni di un nodo con tutti
	gli altri nodi collegati ad esso.
	*/
	QPainter painter(this);
	QPen pen;
	QColor c;
	int d = 10;
	qreal rv;
	pen.setWidth(2);
	painter.setPen(pen);
	for(int i=0; i<grafo.getSize(); ++i)
	{
		rv = qreal(grafo.getAdjMatrix(i,i)) / d;
		c = QColor::fromHsl(205 - (205 - 42) * rv, 200, 135);
		painter.setBrush(QBrush(c));
		painter.drawEllipse(grafo.getArray()[i], dimension/2, dimension/2);
	}

	/*
	Nella seconda parte vengono disegnati gli archi i quali sono stati abbassati di trasparenza
	quando si crea un colore con il comando "pen2.setColor(QColor(0, 0, 0, 140));" l'ultimo parametro
	del costruttore QColor è il canale Alpha dove 0 è trasparente e 255 è visibile

	All'interno dei cicli si cerca qualsiasi arco (cella nella parte bassa della matrice con un numero
	maggiore di 0.
	Ad ogni modo i valori sono stati scritti solamente nella parte inferiore quindi anche se si cerca nella
	parte superiore non ci sono problemi.

	Viene anche calcolato il punto medio di ogni arco per stampare un etichetta con scritto il numero
	di interazioni (preso dalla cella che ci fa capire che c'è un arco)
	*/
	QPen pen2;
	qreal width = 0.1;
	pen2.setWidth(width);
	pen2.setColor(QColor(0, 0, 0, 140));
	painter.setPen(pen2);
	QString tmp = "";
	QPoint puntoMedio(0,0);
	for(int i=0; i<grafo.getSize(); ++i)
	{
		for(int j=0; j<grafo.getSize(); ++j)
		{
			if(grafo.getAdjMatrix(i,j) > 0 && i!=j )
			{
				painter.drawLine(grafo.getArray()[i], grafo.getArray()[j]);
				puntoMedio = (grafo.getArray()[i] + grafo.getArray()[j]) / 2;
				tmp = "";
				tmp += "+" + QString::number(grafo.getAdjMatrix(i,j));
				painter.drawText(puntoMedio, tmp);

			}
		}
	}
}


//metodo che resituisce la variabile privata che determina la larghezza della MainWindow in pixel
int MainWindow::getWindowX() const
{
	return _windowX;
}


//metodo che resituisce la variabile privata che determina l'altezza della MainWindow in pixel
int MainWindow::getWindowY() const
{
		return _windowY;
}


/*
Questo è il metodo che aggiorna il grafo: contiene sia le istruzioni per i passo T(0) sia
quelle per tutti i passi T(n) successivi.
Si è voluto impedire che il grafo si riempisse già nel passo T(0) nonostante non fossero specificati
particolari limiti: è una scelta puramente di comodità, per garantire un paio di iterazioni minime
ma il programma funziona correttamente anche se si riempie nella fase di inizializzazione.

Anche nelle fasi successive T(n) si è posto un limite massimo di 15 ai nodi da generare (quando possibile)
per garantire almeno un paio di iterazioni.

I nodi vengono generati lasciando un margine dal limite massimo della MainWindow

Anche il punto B ed il punto C sono stati limitati per non compromettere eccessivamente la leggibilità del grafo
*/
void MainWindow::iterationUpdate()
{
	//se il grafo ha raggiunto 30 nodi fermo il timer e non faccio più nulla
	if(grafo.getSize() >= 30)
		timer->stop();
	else
	{
		QPoint punto;
		int nodeRandomLimit = 0;
		int nodiAmmessi = grafo.maxNodi - grafo.getSize();
		bool posizioneValida;
		bool inizializzazione = false;

		if(grafo.getSize() == 0)		//T0: inizializzazione, almeno 5 nodi per comodità
		{
			nodeRandomLimit = grafo.randomNumber(2, 15);
			inizializzazione = true;
		} else							//Tn
		{
			if(nodiAmmessi > 10)
				nodeRandomLimit = grafo.randomNumber(0, 15);
			else
				nodeRandomLimit = grafo.randomNumber(0, nodiAmmessi);
		}
		//qDebug() << "Inserirò " << nodeRandomLimit << " nodi.";
		for(int i=0; i<nodeRandomLimit; ++i)
		{
			posizioneValida = false;
			while(!posizioneValida)
			{
				posizioneValida = true;
				punto.setX(rand()%760+20);
				punto.setY(rand()%560+20);
				//qDebug() << "punto.x() = " << punto.x() << ", punto.y() = " << punto.y();
				for(int j=0; j<grafo.getSize(); ++j)
				{
					if( qFabs(punto.x() - grafo.getArray()[j].x()) < minDistance && qFabs( punto.y() - grafo.getArray()[j].y() ) < minDistance)
						posizioneValida = false;
				}
			}
			grafo.add(punto);
		}
		//formula che determina il numero massimo possibile di archi (n*(n-1))/2 - archi esistenti, serve a non chiederne più del possibile
		int archiRandomLimit = ((grafo.getSize()*(grafo.getSize()-1)) / 2) - grafo.getTotalInteractions();
		//siccome gli archi totali possibili sono molti e complicherebbero troppo la visione del grafo li riduciamo qui e anche dopo
		int nuoviArchi = 0;
		if(!inizializzazione)
		{
			//punto B: aggiunta di nuovi archi
			do
			{
				nuoviArchi = grafo.randomNumber(0, 5);
			} while(nuoviArchi > archiRandomLimit);
			//qDebug() << "Punto B, aggiungo: " << nuoviArchi << " nuovi archi.";

			//ad ogni iterazione di questo ciclo aggiungerò un arco nuovo nel grafo
			for(int a=0; a<nuoviArchi; ++a)
			{
				posizioneValida = false;
				while(!posizioneValida)
				{
					//scelgo due interi i e j da usare per scegliere la cella della matrice di adiacenza
					int i = grafo.randomNumber(0, grafo.getSize()-1);
					int j = grafo.randomNumber(0, grafo.getSize()-1);
					//non possono essere uguali, non devo scrivere sulla matrice ora
					while(j == i)
						j = grafo.randomNumber(0, grafo.getSize()-1);
					//dato che uso solo la metà inferiore (o sinistra) della matrice di adiacenza
					//se i<j non va bene perché avrei scelto una cella nella metà superiore, così
					//scambiando i due nodi mantendo l'interazione scelta ma la scrivo solo nella
					//metà inferiore
					if(i<j)
						std::swap(i,j);
					//solo se non esiste un arco tra quei due nodi ne creo uno e metto "true" la variabile
					//booleana, altrimenti il while verrà ripetuto (senza incrementare il contatore "a" del ciclo for
					//esso aumenterà solo quando avrò trovato un arco nuovo da creare
					if(grafo.getAdjMatrix(i,j) == 0)
					{
						grafo.setAdjMatrix(i,j,1);
						posizioneValida = true;
						grafo.setTotalInteractions(grafo.getTotalInteractions()+1);
						//qDebug() << "Creo nuovo arco (i,j): (" << i << ", " << j << ")";
					}
				}
			}
			//Punto C: incrementare interazioni random (anche generando nuovi archi)
			int incrementoInterazioni = 0;
			do
			{
				incrementoInterazioni = grafo.randomNumber(0, 5);
			} while(incrementoInterazioni > archiRandomLimit /2);
			//qDebug() << "aggiungo: " << incrementoInterazioni << " interazioni.";
			for(int c=0; c< incrementoInterazioni; ++c)
			{
				int i = 0;
				int j = 0;
				do
				{
					//scelgo due numeri casuali corrispondenti a due nodi tra cui creare interazioni
					i = grafo.randomNumber(0, grafo.getSize()-1);
					j = grafo.randomNumber(0, grafo.getSize()-1);
					//se sono uguali ne cambio uno, i cappi non sono considerati interazioni
					while(j == i)
						j = grafo.randomNumber(0, grafo.getSize()-1);

					//dato che uso solo la metà inferiore (o sinistra) della matrice di adiacenza
					//se i<j non va bene perché avrei scelto una cella nella metà superiore, così
					//scambiando i due nodi mantendo l'interazione scelta ma la scrivo solo nella
					//metà inferiore
					if(i<j)
						std::swap(i,j);
				//dato che solo il punto C può incrementare le interazioni si è deciso di limitare
				//la creazione di nuove interazioni per assicurarsi di vedere almeno qualche numero
				//di interazioni > 1. Quindi nella prima metà del for si cerca una coppia di nodi con già interazioni
				//si può fare solo se c'è almeno
				} while(grafo.getAdjMatrix(i,j) == 0 && c < incrementoInterazioni/2 && grafo.getTotalInteractions() > 1);
				//qDebug() << "Incremento arco (i,j): (" << i << ", " << j << ")";
				grafo.setAdjMatrix(i, j, grafo.getAdjMatrix(i,j)+1);
			}
			//ho aggiunto nuovi archi, aggiorno il max di collegamenti di ogni nodo (diagonale)
			for(int i=0; i<grafo.getSize(); ++i)
			{
				grafo.setAdjMatrix(i,i,grafo.maxInteractionNode(i));
			}
		}
		/* Stampe di debug per vedere la matrice di adiacenza oppure solo la diagonale contenente
		 * il numero massimo di interazioni che un nodo ha tra tutte le sue interazioni con altri nodi
		 *
		qDebug() << "myFunction: grafo._size: " << grafo.getSize();
		grafo.printAdjMatrix();
		QString s = "";
		for(int i=0; i<grafo.getSize(); ++i)
			s += QString::number(grafo.getAdjMatrix(i,i));
		qDebug() << "Stampa della diagonale: " << s;
		*/


		//metodo update() che causa la chiamata del paintEvent
		update();
	}
}

