#include <iostream>

#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include <QApplication>

#include <io/term-out.h>
#include <gui/main-w.h>


#include <xmath/field.h>
#include <util.h>


struct thread_data{
	bool play;
	fluid_sim *fs;
};


void* thread_fs_compute(void *data){
	thread_data *td = (thread_data*)data;

	while (true){
		if (td->play)
			td->fs->compute();
		else
			usleep(10 * 1000);
	}
}


int main(int argc, char *argv[]){

	using namespace xmath;
	using namespace std;

	// linmapf lm({0.0f, 10.0f}, {-1.0f, 1.0f});
	// linmapf inv_lm = inv(lm);
	// linmapf lm2(2, 1), lm3({0,2});


	// std::cout << lm << "\n\n" << inv_lm << "\n\n";
	// std::cout << lm2 << "\n\n" << lm3 << "\n\n";

	// for (float i = 0.0f; i <= 10.0f; i+=1.0f)
	// 	std::cout << i << ":" << lm(i) << "\t";
	// std::cout << "\n";

	// for (float i = -1.0f; i <= 1.0f; i+=0.2f)
	// 	std::cout << i << ":" << inv_lm(i) << "\t";
	// std::cout << "\n";

	// vecf2 v{1.0f};
	// vecf2 w{-2.33f,7.5f};
	// vecf2 a{0,0}, b{5,6}, c{-1,2}, d{1,3};
	// vec<float,10> vv(0);
	// vv = {1,2,3,4,5,6,7,8,9,10};
	// vec<float,10> ww(5);
	// vec<uint,2> q{7,10};
	// vec<float,2> p(q);
	// // q = w;

	// std::cout << v << "\n" << w << "\n" << v+w << "\n\n";
	// std::cout << vv << "\n" << ww << "\n" << vv+ww << "\n" << add<6>(vv,ww) << "\n" << add(vv,ww) << "\n" << min(vv,ww) << "\n" << sq(vv) << "\n" << -vv << "\n" << q << "\n" << p << "\n" << 10.0f*vv << "\n" << static_cast<vec<float,20>>(q) << "\n\n";

	// linmap<vecf2> lmv({a,b},{c,d});
	// std::cout << lmv << "\n\n" << inv(lmv) << "\n";
	// std::cout << fl(w) << "\n" << cl(w) << "\n";



	// CFS cfs(200, 20, {vecf2(), vecf2{10,1}});
	// cfs.jacobi_it_max = 100;
	// cfs.sor_it_max = 20;
	// cfs.reynold = 100;
	// // cfs.force = vecf2{0.5f, -0.5f};

	// // for (uint i = 0; i < cfs.p.size[0]; i++){
	// // 	for (uint j = 0; j < cfs.p.size[1]; j++){
	// // 		const vecf2 pos = cfs.p.itos(vecf2{(float)i,(float)j});
	// // 		cfs.p(i,j) = sqrt(pos[0]) * sin(3*M_PI*pos[0]) * sin(M_PI*pos[1]);
	// // 	}
	// // }

	// for (uint i = 0; i < cfs.vx.size[0]; i++){
	// 	for (uint j = 0; j < cfs.vx.size[1]; j++){
	// 		const vecf2 pos = cfs.vx.itos(vecf2{(float)i,(float)j});
	// 		// cfs.vx(i,j) = sqrt(pos[0]) * sin(3*M_PI*pos[0]) * sin(M_PI*pos[1]);
	// 		cfs.vx(i,j) = 0.5f;
	// 	}
	// }

	// // for (uint i = 0; i < cfs.vy.size[0]; i++){
	// // 	for (uint j = 0; j < cfs.vy.size[1]; j++){
	// // 		const vecf2 pos = cfs.vy.itos(vecf2{(float)i,(float)j});
	// // 		cfs.vy(i,j) = pos.x;
	// // 	}
	// // }


	fluid_sim cfs(1<<11, 1<<8, {vecf2(), vecf2(8.0f,1.0f)});
	cfs.set_reynold(10000.0f);
	cfs.jacobi_it_max = 10;

	thread_data *td = new thread_data;
	td->play = true;
	td->fs = &cfs;

	pthread_t fs_thread;

	pthread_create(&fs_thread, NULL, thread_fs_compute, (void*)td);


	QApplication app(argc, argv);

	MainW *main_w = new MainW(&cfs);
	main_w->fs_play = &(td->play);
	main_w->resize(800, 450);
	main_w->show();

	app.exec();
	
	delete main_w;


	// intvlv domain{vecf2{0,1}, vecf2{2,3}};

	// fieldf f(10, 5, domain);

	// for (uint i = 0; i < f.size(); i++){
	// 	f[i] = i*i;
	// }

	// cout << f << endl;


	// fieldf *g = new fieldf(f);

	// for (uint i = 0; i < op::min(f.dim_x(),f.dim_y()); i++){
	// 	(*g)(i,i) = 0.0f;
	// }

	// cout << *g << endl;

	// fieldf h(2,2,domain);
	// h = *g;

	// cout << h << endl;

	// delete g;

	// cout << f << endl;
	// cout << h << endl;


	// f = fieldf(5,5,domain);
	// f.setzero();

	// cout << f << endl;


	return 0;
}
