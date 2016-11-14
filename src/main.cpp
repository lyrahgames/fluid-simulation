#include <iostream>
// using namespace std;

#include <QApplication>

#include <cfs.h>
#include <main-w.h>


int main(int argc, char *argv[]){

	CFS cfs{gridmap(0.0f, 2.0f, 100), gridmap(0.0f, 1.0f, 100)};

	QApplication app(argc, argv);

	MainW *main_w = new MainW(&cfs);
	main_w->resize(600, 600);
	main_w->show();

	app.exec();
	
	delete main_w;
	
	return 0;
}