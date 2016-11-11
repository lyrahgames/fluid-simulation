#include <iostream>
// using namespace std;

#include <QApplication>

#include <cfs.h>
#include <render-w.h>


int main(int argc, char *argv[]){

	CFS cfs;

	QApplication app(argc, argv);

	RenderW *render = new RenderW(&cfs);
	render->resize(600, 600);
	render->show();

	app.exec();
	
	delete render;
	
	return 0;
}