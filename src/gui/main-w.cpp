#include "main-w.h"


MainW::MainW(CFS* cfs_src, QWidget *parent):
QWidget(parent), ready(true), mouse(veci2{}), mouse_press(Qt::NoButton), cfs(cfs_src), view(cfs_src->space), 
rand_pos(nullptr), rand_pos_size(0), part_pos(nullptr){

	init_rand_pos();
	rand_pos_size = 100;

	init_part_pos();
	part_count = 10000;
	setMouseTracking(true);

	p_colormap.add_base({-6.0f, {0.0f,0.5f,1.0f}});
	p_colormap.add_base({1.0f, {1.0f,0.5f,0.0f}});
	p_colormap.add_base({-3.0f, {0.0f,1.0f,1.0f}});
	p_colormap.add_base({0.25f, {1.0f,1.0f,0.0f}});
	p_colormap.add_base({0.0f, {1,1,1}});
	p_colormap.add_base({100.0f, {1,0,0}});
	// p_colormap.add_base({2.0f, {1,0,1}});
	p_colormap.add_base({-100.0f, {0,0,1}});
	// p_colormap.add_base({-2.0f, {0,1,0}});


	v_colormap.add_base({0.0f, {1.0f,1.0f,1.0f}});
	v_colormap.add_base({0.5f, {0.0f,0.5f,0.5f}});
	v_colormap.add_base({1.0f, {0.0f,0.0f,1.0f}});
	v_colormap.add_base({100.0f, {1.0f,0.0f,1.0f}});
	// v_colormap.add_base({0.0f, {1,1,1}});
	// v_colormap.add_base({1.0f, {1,0,0}});
	// v_colormap.add_base({2.0f, {1,0,1}});
	// v_colormap.add_base({-100.0f, {0,0,1}});
	// v_colormap.add_base({-2.0f, {0,1,0}});


	render_w = new RenderW(this);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	connect(timer, SIGNAL(timeout()), this, SLOT(loop_slot()));
	timer->start(_refresh_time_);


	ctrl_w = new CtrlW(this);


	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(render_w);
	layout->addWidget(ctrl_w);

	ctrl_w->setFixedWidth(250);
	// ctrl_w->adjustSize();

	// resize(width(), height());
}

MainW::MainW(fluid_sim* pfs, QWidget *parent):
QWidget(parent), ready(true), mouse(veci2{}), mouse_press(Qt::NoButton), fs(pfs), view(pfs->geom), 
rand_pos(nullptr), rand_pos_size(0), part_pos(nullptr){
	init_rand_pos();
	rand_pos_size = 100;

	init_part_pos();
	part_count = 10000;
	setMouseTracking(true);

	p_colormap.add_base({-1.0f, {0.0f,0.0f,1.0f}});
	p_colormap.add_base({-0.5f, {0.0f,0.5f,1.0f}});
	p_colormap.add_base({0.0f, {1,1,1}});
	p_colormap.add_base({0.5f, {1,0.5,0}});
	p_colormap.add_base({1.0f, {1,0,0}});

	v_colormap.add_base({0.0f, {0.0f,0.0f,1.0f}});
	v_colormap.add_base({0.25f, {0.0f,0.5f,1.0f}});
	v_colormap.add_base({0.5f, {1,1,1}});
	v_colormap.add_base({0.75f, {1,0.5,0}});
	v_colormap.add_base({1.0f, {1,0,0}});

	res_colormap.add_base({-1.0f, {0.0f,0.0f,1.0f}});
	res_colormap.add_base({-0.5f, {0.0f,1.0f,1.0f}});
	res_colormap.add_base({0.0f, {1.0f,1.0f,1.0f}});
	res_colormap.add_base({0.5f, {1.0f,1.0f,0.0f}});
	res_colormap.add_base({1.0f, {1.0f,0.0f,0.0f}});


	render_w = new RenderW(this);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	connect(timer, SIGNAL(timeout()), this, SLOT(loop_slot()));
	timer->start(_refresh_time_);


	ctrl_w = new CtrlW(this);


	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(render_w);
	layout->addWidget(ctrl_w);

	ctrl_w->setFixedWidth(250);
}

MainW::~MainW(){
	delete[] rand_pos;
}

void MainW::repaint(){
	if (!ready)
		return;

	update();
}

void MainW::loop_slot(){
	// if (mouse_press == Qt::LeftButton){
	// 	const vecf2 pos = render_w->pix_itos(mouse);
	// 	const vecf2 tmp = cfs->p.stoi(pos);
	// 	const vecu2 idx = fl(tmp);
	// 	const float scale = 0.01f;

		// if (!cfs->p.out_bound(idx)){
			// cfs->p(idx) += scale * static_cast<float>(_refresh_time_);
			// cfs->vx(idx) += scale * static_cast<float>(_refresh_time_);
			// cfs->add_vx_persis(idx[0], idx[1], scale * static_cast<float>(_refresh_time_));
			// cfs->rhs[cfs->p.memidx(idx)] += scale * static_cast<float>(_refresh_time_);
		// }
		// if (!cfs->p.out_bound(idx + vecu2{1,0})){
		// 	cfs->p(idx + vecu2{1,0}) += scale * static_cast<float>(_refresh_time_);

		// }
		// if (!cfs->p.out_bound(idx + vecu2{0,1})){
		// 	cfs->p(idx + vecu2{0,1}) += scale * static_cast<float>(_refresh_time_);

		// }
		// if (!cfs->p.out_bound(idx + vecu2{1,1})){
		// 	cfs->p(idx + vecu2{1,1}) += scale * static_cast<float>(_refresh_time_);

		// }
	// }else if(mouse_press == Qt::RightButton){
	// 	const vecf2 pos = render_w->pix_itos(mouse);
	// 	const vecf2 tmp = cfs->p.stoi(pos);
	// 	const vecu2 idx = fl(tmp);
	// 	const float scale = -0.01f;

		// if (!cfs->p.out_bound(idx)){
			// cfs->p(idx) += scale * static_cast<float>(_refresh_time_);
			// cfs->add_vx_persis(idx[0], idx[1], scale * static_cast<float>(_refresh_time_));
			// cfs->rhs[cfs->p.memidx(idx)] += scale * static_cast<float>(_refresh_time_);
		// }
		// if (!cfs->p.out_bound(idx + vecu2{1,0})){
		// 	cfs->p(idx + vecu2{1,0}) += scale * static_cast<float>(_refresh_time_);

		// }
		// if (!cfs->p.out_bound(idx + vecu2{0,1})){
		// 	cfs->p(idx + vecu2{0,1}) += scale * static_cast<float>(_refresh_time_);

		// }
		// if (!cfs->p.out_bound(idx + vecu2{1,1})){
		// 	cfs->p(idx + vecu2{1,1}) += scale * static_cast<float>(_refresh_time_);

		// }
	// }

	if (play){
		// cfs->poisson_test_jacobi_it();
		// cfs->wave_it();
		// cfs->poisson_p_jacobi_it();
		// cfs->grad();
		// cfs->compute_time_it();
		// printf("step:%i\ttime:%f\n", cfs->it_step, cfs->time);

		// fs->compute();
		// printf("step:\t%u\ttime:%f\tdt:%f\t%f\n", fs->it, fs->t, fs->dt, fs->dy);
	}
	// cfs->poisson_p_sor_it();
	set_ready();
}

void MainW::init_rand_pos(){
	if (rand_pos != nullptr)
		delete[] rand_pos;

	rand_pos = new vecf2[_rand_pos_size_max_];

	gen_rand_pos();
}

void MainW::gen_rand_pos(){
	for (uint i = 0; i < _rand_pos_size_max_; i++){
		// rand_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(cfs->vx.space).x + cfs->vx.space.min.x;
		// rand_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(cfs->vy.space).y + cfs->vy.space.min.y;
		rand_pos[i][0] = (float(rand())/float(RAND_MAX)) * (len(fs->geom).x - 2.0f*fs->dx) + fs->geom.min.x + fs->dx;
		rand_pos[i][1] = (float(rand())/float(RAND_MAX)) * (len(fs->geom).y - 2.0f*fs->dy) + fs->geom.min.y + fs->dy;
	}
}

void MainW::init_part_pos(){
	if (part_pos != nullptr)
		delete[] part_pos;

	part_pos = new vecf2[_part_count_max_];

	gen_part_pos();
}

void MainW::gen_part_pos(){
	for (uint i = 0; i < _part_count_max_; i++){
		// part_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(cfs->vx.space).x + cfs->vx.space.min.x;
		// part_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(cfs->vy.space).y + cfs->vy.space.min.y;
		part_pos[i][0] = (float(rand())/float(RAND_MAX)) * (len(fs->geom).x - 2.0f*fs->dx) + fs->geom.min.x + fs->dx;
		part_pos[i][1] = (float(rand())/float(RAND_MAX)) * (len(fs->geom).y - 2.0f*fs->dy) + fs->geom.min.y + fs->dy;
	}
}

void MainW::play_slot(){
	// play = !play;
	const bool tmp = *fs_play;
	*fs_play = !tmp;
}

void MainW::clear_slot(){
	// cfs->clear();
	fs->clear();
	set_ready();
}

void MainW::set_rand_pos_size_slot(int i){
	rand_pos_size = i;
	set_ready();
}

void MainW::gen_rand_pos_slot(){
	gen_rand_pos();
	set_ready();
}

void MainW::set_part_count_slot(int i){
	part_count = i;
	set_ready();
}

void MainW::gen_part_pos_slot(){
	gen_part_pos();
	set_ready();
}

void MainW::set_wave_damp_slot(double val){
	// cfs->wave_damp = val;
}

void MainW::set_wave_c_slot(double val){
	// cfs->wave_c = val;
}

void MainW::set_wave_dt_slot(double val){
	// cfs->wave_dt = val;
}

// mens changes
void MainW::set_reynold_slot(double val){
	// cfs->reynold = val;
	fs->set_reynold(val);
}

void MainW::border_pb_slot(){
	// cfs->down_bound = down_cb.currentIndex();
}

void MainW::set_p_render_slot(int val){
	p_render = val;
}

void MainW::set_jacobi_max_it_slot(int val){
	fs->jacobi_it_max = val;
}

void MainW::set_colormap_ref_slot(double val){
	if (ctrl_w->p_rb->isChecked()){
		p_ref = val;
	}else if (ctrl_w->v_rb->isChecked() || ctrl_w->vx_rb->isChecked() || ctrl_w->vy_rb->isChecked()){
		v_ref = val;
	}else if (ctrl_w->res_rb->isChecked()){
		res_ref = val;
	}
}

MainW::RenderW::RenderW(MainW *parent): QWidget(parent), main_w(parent){
	setMouseTracking(true);
}

void MainW::RenderW::paintEvent(QPaintEvent *event){
	if (!ready())
		return;

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing, true);
	QImage map(width(), height(), QImage::Format_ARGB32);
	map.fill(Qt::black);


	// const int i_min = 0;
	// const int i_max = width()-1;
	// const int j_min = 0;
	// const int j_max = height()-1;

	if (main_w->ctrl_w->colormap_gb->isChecked()){

		const vecf2 bound0 = pix_stoi( main_w->fs->geom.min + vecf2{main_w->fs->dx, main_w->fs->dy} );
		const vecf2 bound1 = pix_stoi( main_w->fs->geom.max - vecf2{main_w->fs->dx, main_w->fs->dy} );

		const int i_min = ceilf(bound0.x);
		const int i_max = floorf(bound1.x);
		const int j_min = ceilf(bound1.y);
		const int j_max = floorf(bound0.y);

		if (main_w->ctrl_w->p_rb->isChecked()){ // render p

			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float value = main_w->fs->p(pos) * main_w->p_ref;

					const color_rgbf tmp = 255.0f * p_colormap()(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}else if (main_w->ctrl_w->v_rb->isChecked()){ // render v

			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float mag = sqrtf( xmath::op::sq(main_w->fs->vx(pos)) + xmath::op::sq(main_w->fs->vy(pos)) );
					const float value = mag * main_w->v_ref;
					const color_rgbf tmp = 255.0f * (main_w->v_colormap)(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}else if (main_w->ctrl_w->vx_rb->isChecked()){

			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float value = main_w->fs->vx(pos) * main_w->v_ref;

					const color_rgbf tmp = 255.0f * (main_w->v_colormap)(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}else if (main_w->ctrl_w->vy_rb->isChecked()){

			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float value = main_w->fs->vy(pos) * main_w->v_ref;

					const color_rgbf tmp = 255.0f * (main_w->v_colormap)(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}else if (main_w->ctrl_w->res_rb->isChecked()){

			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float value = main_w->fs->res(pos) * main_w->res_ref;

					const color_rgbf tmp = 255.0f * (main_w->res_colormap)(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}

		const QRect rect = QRect(0, 0, width(), height());
		painter.drawImage(rect, map, rect);

		/*// const vecf2 bound0 = pix_stoi( cfs().p.itos(vecf2(1,1)) );
		// const vecf2 bound1 = pix_stoi( cfs().p.itos(vecf2(cfs().p.size[0]-1, cfs().p.size[1]-1)) );

		const vecf2 bound0 = pix_stoi( main_w->fs->geom.min + vecf2{main_w->fs->dx, main_w->fs->dy} );
		const vecf2 bound1 = pix_stoi( main_w->fs->geom.max - vecf2{main_w->fs->dx, main_w->fs->dy} );

		const int i_min = ceilf(bound0.x);
		const int i_max = floorf(bound1.x);
		const int j_min = ceilf(bound1.y);
		const int j_max = floorf(bound0.y);

		for (int i = i_min; i <= i_max; i++){
			for (int j = j_min; j <= j_max; j++){
				const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
				const vecf2 pos = pix_itos(idx);

				// const float mag = sqrtf(xmath::op::sq(cfs().vx(pos)) + xmath::op::sq(cfs().vy(pos)));
				const float mag = sqrtf( xmath::op::sq(main_w->fs->vx(pos)) + xmath::op::sq(main_w->fs->vy(pos)) );

				// const color_rgbf tmp = 255.0f * p_colormap()(cfs().p(pos));
				// const color_rgbf tmp = 255.0f * p_colormap()(main_w->fs->res(pos));
				const color_rgbf tmp = 255.0f * (main_w->v_colormap)(mag);
				// const color_rgbf tmp = 255.0f * (main_w->res_colormap)(main_w->fs->res(pos));
				const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
				map.setPixel(i,j,col);
			}
		}

		const QRect rect = QRect(0, 0, width(), height());
		painter.drawImage(rect, map, rect);*/
	}


	if (main_w->ctrl_w->stream_render_chb->isChecked()){
		const float path_step = 0.01f;

		for (int i = 0; i < main_w->rand_pos_size; i++){
			// float pos_x = _pix_grid_map[0].cell_pos(mouse_x);
			// float pos_y = _pix_grid_map[1].cell_pos(height()-mouse_y);
			// QPainterPath path(QPointF(mouse_x, mouse_y));
			// float pos_x = main_w->rand_pos[i][0];
			// float pos_y = main_w->rand_pos[i][1];
			// const float idxx = _pix_grid_map[0].cell_idx(pos_x);
			// const float idxy = _pix_grid_map[1].cell_idx(pos_y);
			vecf2 pos = main_w->rand_pos[i];
			vecf2 idx = pix_stoi(pos);
			QPainterPath path(QPointF(idx[0],idx[1]));

			for (int t = 0; t < 1000; t++){
				// const float tmp_pos_x = pos_x + path_step * cfs().vx(pos_x, pos_y);
				// const float tmp_pos_y = pos_y + path_step * cfs().vy(pos_x, pos_y);
				// const vecf2 tmp_pos = pos + path_step * vecf2(cfs().vx(pos), cfs().vy(pos));
				const vecf2 tmp_pos = pos + path_step * vecf2(main_w->fs->vx(pos), main_w->fs->vy(pos));
			
				if (main_w->fs->vx.out_bound(tmp_pos) || main_w->fs->vy.out_bound(tmp_pos))
					break;

				// pos_x = tmp_pos_x;
				// pos_y = tmp_pos_y;

				pos = tmp_pos;

				// const float idxx = _pix_grid_map[0].cell_idx(pos_x);
				// const float idxy = _pix_grid_map[1].cell_idx(pos_y);
				const vecf2 idx = pix_stoi(pos);

				path.lineTo(idx.x, idx.y);
			}

			painter.setBrush(Qt::NoBrush);
			painter.drawPath(path);
		}
	}


	if (main_w->ctrl_w->part_render_chb->isChecked()){
		const float part_step = 0.5f;

		for (int i = 0; i < main_w->part_count; i++){
			vecf2 pos = main_w->part_pos[i];
			vecf2 idx = pix_stoi(pos);

			painter.drawPoint(idx[0], idx[1]);

			// const vecf2 tmp_pos = pos + part_step * vecf2(cfs().vx(pos), cfs().vy(pos));
			const vecf2 tmp_pos = pos + part_step * vecf2(main_w->fs->vx(pos), main_w->fs->vy(pos));

			// if (cfs().vx.out_bound(tmp_pos) || cfs().vy.out_bound(tmp_pos)){
			// 	main_w->part_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(cfs().vx.space).x + cfs().vx.space.min.x;
			// 	main_w->part_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(cfs().vy.space).y + cfs().vy.space.min.y;
			// 	continue;
			// }

			if (main_w->fs->vx.out_bound(tmp_pos) || main_w->fs->vy.out_bound(tmp_pos)){
				main_w->part_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(main_w->fs->geom).x + main_w->fs->geom.min.x;
				main_w->part_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(main_w->fs->geom).y + main_w->fs->geom.min.y;
				continue;
			}
			
			main_w->part_pos[i] = tmp_pos;
		}
	}



	painter.end();

	set_ready(false);
}

void MainW::RenderW::resizeEvent(QResizeEvent *event){
	const vecf2 domain_len = vecf2{static_cast<float>(width()-1), static_cast<float>(height()-1)};
	const vecf2 view_len = len(main_w->view);

	intvlv range;

	if (view_len.x * domain_len.y < view_len.y * domain_len.x){
		const float mid = 0.5f * (main_w->view.min.x + main_w->view.max.x);
		const float domain_ar = domain_len.x/domain_len.y;
		
		range = intvlv{
			vecf2{mid - 0.5f * domain_ar * view_len.y, main_w->view.min.y},
			vecf2{mid + 0.5f * domain_ar * view_len.y, main_w->view.max.y}
		};
	}else{
		const float mid = 0.5f * (main_w->view.min.y+ main_w->view.max.y);
		const float domain_ar = domain_len.y/domain_len.x;

		range = intvlv{
			vecf2{main_w->view.min.x, mid - 0.5f * domain_ar * view_len.x},
			vecf2{main_w->view.max.x, mid + 0.5f * domain_ar * view_len.x}
		};
	}

	// origin is in the bottom left corner
	const intvlv domain{
		vecf2{0.0f, domain_len.y},
		vecf2{domain_len.x, 0.0f}
	};

	pix_itos = linmapv(domain, range);
	pix_stoi = inv(pix_itos);
}

MainW::CtrlW::CtrlW(MainW *parent): QWidget(parent), main_w(parent){
	// main group box and scroll area with scroll widget
	main_gb = new QGroupBox("controls:", this);

	main_sa = new QScrollArea(main_gb);
	main_sa->setBackgroundRole(QPalette::Midlight);
	main_sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	main_sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	main_sw = new QWidget(main_sa);
	

	play_pb = new QPushButton("play/pause", this);
	connect(play_pb, SIGNAL(clicked()), main_w, SLOT(play_slot()));

	clear_pb = new QPushButton("clear", this);
	connect(clear_pb, SIGNAL(clicked()), main_w, SLOT(clear_slot()));


	// wave group box
	wave_gb = new QGroupBox("wave equation", this);

	wave_damp_l = new QLabel("damping", this);

	wave_damp_dsb = new QDoubleSpinBox(this);
	connect(wave_damp_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_wave_damp_slot(double)));

	wave_c_l = new QLabel("velocity");

	wave_c_dsb = new QDoubleSpinBox(this);
	connect(wave_c_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_wave_c_slot(double)));

	wave_dt_l = new QLabel("time step");

	wave_dt_dsb = new QDoubleSpinBox(this);
	wave_dt_dsb->setDecimals(6);
	wave_dt_dsb->setRange(0.000001, 0.1);
	wave_dt_dsb->setSingleStep(0.00001);
	connect(wave_dt_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_wave_dt_slot(double)));

	QVBoxLayout *wave_gb_layout = new QVBoxLayout(wave_gb);
	wave_gb_layout->addWidget(wave_damp_l);
	wave_gb_layout->addWidget(wave_damp_dsb);
	wave_gb_layout->addWidget(wave_c_l);
	wave_gb_layout->addWidget(wave_c_dsb);
	wave_gb_layout->addWidget(wave_dt_l);
	wave_gb_layout->addWidget(wave_dt_dsb);
	wave_gb->setVisible(false);


	// render group box
	render_gb = new QGroupBox("render:", this);

	rand_pos_size_sb = new QSpinBox(this);
	rand_pos_size_sb->setMinimum(0);
	rand_pos_size_sb->setMaximum(MainW::_rand_pos_size_max_);
	rand_pos_size_sb->setValue(main_w->rand_pos_size);
	connect(rand_pos_size_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_rand_pos_size_slot(int)));

	gen_rand_pos_pb = new QPushButton("gen rand pos", this);
	connect(gen_rand_pos_pb, SIGNAL(clicked()), main_w, SLOT(gen_rand_pos_slot()));

	stream_render_chb = new QCheckBox("show streamlines", this);

	part_count_sb = new QSpinBox(this);
	part_count_sb->setMinimum(0);
	part_count_sb->setMaximum(MainW::_part_count_max_);
	part_count_sb->setValue(main_w->part_count);
	connect(part_count_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_part_count_slot(int)));

	gen_part_pos_pb = new QPushButton("gen part pos", this);
	connect(gen_part_pos_pb, SIGNAL(clicked()), main_w, SLOT(gen_part_pos_slot()));

	part_render_chb = new QCheckBox("show particles", this);


	colormap_gb = new QGroupBox("p_colormap", this);
	colormap_gb->setCheckable(true);

	p_rb = new QRadioButton("p", colormap_gb);
	p_rb->setChecked(true);
	v_rb = new QRadioButton("v", colormap_gb);
	vx_rb = new QRadioButton("vx", colormap_gb);
	vy_rb = new QRadioButton("vy", colormap_gb);
	res_rb = new QRadioButton("res", colormap_gb);

	QVBoxLayout *colormap_gb_layout = new QVBoxLayout(colormap_gb);
	colormap_gb_layout->addWidget(p_rb);
	colormap_gb_layout->addWidget(v_rb);
	colormap_gb_layout->addWidget(vx_rb);
	colormap_gb_layout->addWidget(vy_rb);
	colormap_gb_layout->addWidget(res_rb);

	p_colormap_ref_dsb = new QDoubleSpinBox(this);
	p_colormap_ref_dsb->setRange(0.0f, 100.0f);
	p_colormap_ref_dsb->setDecimals(4);
	p_colormap_ref_dsb->setSingleStep(0.001f);
	connect(p_colormap_ref_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_colormap_ref_slot(double)));


	QVBoxLayout *render_gb_layout = new QVBoxLayout(render_gb);
	render_gb_layout->addWidget(stream_render_chb);
	render_gb_layout->addWidget(rand_pos_size_sb);
	render_gb_layout->addWidget(gen_rand_pos_pb);
	render_gb_layout->addWidget(part_render_chb);
	render_gb_layout->addWidget(part_count_sb);
	render_gb_layout->addWidget(gen_part_pos_pb);
	render_gb_layout->addWidget(colormap_gb);
	render_gb_layout->addWidget(p_colormap_ref_dsb);
	

	// mens changes
	// nse group box
	nse_gb = new QGroupBox("navier-stokes-equation:", this);

	reynold_l = new QLabel("reynold number", this);

	reynold_dsb = new QDoubleSpinBox(this);
	reynold_dsb->setRange(0.0, 10000.0);
	reynold_dsb->setSingleStep(10.0);
	reynold_dsb->setValue(100.0f);
	connect(reynold_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_reynold_slot(double)));

	QVBoxLayout *nse_gb_layout = new QVBoxLayout(nse_gb);
	nse_gb_layout->addWidget(reynold_l);
	nse_gb_layout->addWidget(reynold_dsb);

	border_l = new QLabel("border cond [DULR]", this);
	nse_gb_layout->addWidget(border_l);

	down_cb = new QComboBox(this);
	down_cb->addItem("Haftbedingung");
	down_cb->addItem("slip");
	down_cb->addItem("p stream +");
	down_cb->addItem("p stream -");
	down_cb->addItem("o stream +");
	down_cb->addItem("o stream -");

	border_pb = new QPushButton("commit border cond", this);

	nse_gb_layout->addWidget(down_cb);
	nse_gb_layout->addWidget(border_pb);
	// mens changes end


	jacobi_max_it_sb = new QSpinBox(this);
	jacobi_max_it_sb->setMinimum(1);
	jacobi_max_it_sb->setMaximum(1000);
	connect(jacobi_max_it_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_jacobi_max_it_slot(int)));

	
	// main layouts
	QVBoxLayout *main_sw_layout = new QVBoxLayout(main_sw);
	main_sw_layout->addWidget(play_pb);
	main_sw_layout->addWidget(clear_pb);
	main_sw_layout->addWidget(wave_gb);
	main_sw_layout->addWidget(render_gb);
	main_sw_layout->addWidget(nse_gb);
	main_sw_layout->addWidget(jacobi_max_it_sb);

	main_sa->setWidget(main_sw);

	QVBoxLayout *main_gb_layout = new QVBoxLayout(main_gb);
	main_gb_layout->addWidget(main_sa);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(main_gb);
}

void MainW::CtrlW::resizeEvent(QResizeEvent* event){
	
}