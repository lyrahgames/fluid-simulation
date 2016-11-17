#include <iostream>

#include <QApplication>

#include <io/term-out.h>
#include <main-w.h>


int main(int argc, char *argv[]){

	using namespace xmath;

	linmapf lm({0.0f, 10.0f}, {-1.0f, 1.0f});
	linmapf inv_lm = inv(lm);
	linmapf lm2(2, 1), lm3({0,2});


	std::cout << lm << "\n\n" << inv_lm << "\n\n";
	std::cout << lm2 << "\n\n" << lm3 << "\n\n";

	for (float i = 0.0f; i <= 10.0f; i+=1.0f)
		std::cout << i << ":" << lm(i) << "\t";
	std::cout << "\n";

	for (float i = -1.0f; i <= 1.0f; i+=0.2f)
		std::cout << i << ":" << inv_lm(i) << "\t";
	std::cout << "\n";

	vecf2 v{1.0f};
	vecf2 w{2.33f,7.5f};
	vecf2 a{0,0}, b{5,6}, c{-1,2}, d{1,3};
	vec<float,10> vv(0);
	vv = {1,2,3,4,5,6,7,8,9,10};
	vec<float,10> ww(5);
	vec<uint,2> q;
	q = w;

	std::cout << v << "\n" << w << "\n" << v+w << "\n\n";
	std::cout << vv << "\n" << ww << "\n" << vv+ww << "\n" << add<6>(vv,ww) << "\n" << add(vv,ww) << "\n" << min(vv,ww) << "\n" << sq(vv) << "\n" << -vv << "\n" << q << "\n" << 10.0f*vv << "\n\n";

	linmap<vecf2> lmv({a,b},{c,d});
	std::cout << lmv << "\n\n" << inv(lmv) << "\n";


	// CFS cfs{gridmap(0.0f, 1.0f, 32), gridmap(0.0f, 1.0f, 32)};

	// QApplication app(argc, argv);

	// MainW *main_w = new MainW(&cfs);
	// main_w->resize(800, 450);
	// main_w->show();

	// app.exec();
	
	// delete main_w;
	
	return 0;
}