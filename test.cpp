#include "main-w.h"


MainW::MainW(CFS* cfs_src, QWidget *parent):
QMainWindow(parent), ready(true), mouse(veci2{}), mouse_press(Qt::NoButton), cfs(cfs_src), view(cfs_src->space), 
rand_pos(nullptr), rand_pos_size(0), part_pos(nullptr),
frame(0,0,cfs->space), vx_frame(0,0,cfs->space), vy_frame(0,0,cfs->space){

	init_rand_pos();
	rand_pos_size = 10;

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

	// ctrl_w->setFixedWidth(250);
	// ctrl_w->adjustSize();

	// resize(width(), height());
}

MainW::MainW(fluid_sim* pfs, QWidget *parent):
QMainWindow(parent), ready(true), mouse(veci2{}), mouse_press(Qt::NoButton), fs(pfs), view(pfs->geom), 
rand_pos(nullptr), rand_pos_size(0), part_pos(nullptr),
frame(pfs->p), vx_frame(pfs->vx), vy_frame(pfs->vy){

	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);

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

	v_colormap.add_base({-100.0f, {0.0f,0.0f,1.0f}});
	v_colormap.add_base({0.0f, {0.0f,0.0f,1.0f}});
	// v_colormap.add_base({0.25f, {0.5f,1.0f,0.0f}});
	v_colormap.add_base({0.5f, {0,0.8,1}});
	// v_colormap.add_base({0.75f, {1,0.5,0}});
	v_colormap.add_base({1.0f, {1,1,1}});
	v_colormap.add_base({10000.0f, {1.0f,1.0f,1.0f}});

	res_colormap.add_base({-1.0f, {0.0f,0.0f,1.0f}});
	res_colormap.add_base({-0.5f, {0.0f,1.0f,1.0f}});
	res_colormap.add_base({0.0f, {1.0f,1.0f,1.0f}});
	res_colormap.add_base({0.5f, {1.0f,1.0f,0.0f}});
	res_colormap.add_base({1.0f, {1.0f,0.0f,0.0f}});

	obs_colormap.add_base({-1.0f, {1.0f,1.0f,1.0f}});
	obs_colormap.add_base({0.0f, {1.0f,1.0f,1.0f}});
	obs_colormap.add_base({1.0f, {0.0f,0.0f,0.0f}});
	obs_colormap.add_base({2.0f, {0.0f,0.0f,0.0f}});


	render_w = new RenderW(this);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	connect(timer, SIGNAL(timeout()), this, SLOT(loop_slot()));
	timer->start(_refresh_time_);



	setCentralWidget(render_w);


	ctrl_dw = new QDockWidget("controls", this);
	ctrl_dw->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);


	ctrl_w = new CtrlW(this);

	ctrl_dw->setWidget(ctrl_w);
	addDockWidget(Qt::RightDockWidgetArea, ctrl_dw);



	// info_w
	info_dw = new QDockWidget("info", this);
	info_dw->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

	info_w = new InfoW(this);

	info_dw->setWidget(info_w);
	addDockWidget(Qt::LeftDockWidgetArea, info_dw);

	info_update_t = new QTimer(this);
	connect(info_update_t, SIGNAL(timeout()), this, SLOT(info_update_info_slot()));
	info_update_t->start(_info_refresh_time_);


	// QHBoxLayout *layout = new QHBoxLayout(this);
	// layout->addWidget(info_w);
	// layout->addWidget(render_w);
	// layout->addWidget(ctrl_w);

	// ctrl_w->setFixedWidth(250);


	init_recording();
	play_recording = false;

	rec_timer = new QTimer(this);
	connect(rec_timer, SIGNAL(timeout()), this, SLOT(record_frame()));
	rec_timer->start(5000);


	main_menu = new QMenu(this);
	main_menu->addAction(ctrl_dw->toggleViewAction());
	main_menu->addAction(info_dw->toggleViewAction());
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
	if (mouse_press == Qt::LeftButton){
		const vecf2 pos = render_w->pix_itos(mouse);
		const vecf2 tmp = fs->p.domain_to_grid()(pos);
		const vecu2 idx = fl(tmp);
		const float scale = 10.f;
		vecf2 move = scale * vecf2(mouse - mouse_old);
		// move.y = -move.y;
		move = move * render_w->pix_itos.slope;



		if (ctrl_pressed){
			// for (int j = 1-(int)radius; j <= (int)radius; j++){
			// 	for (int i = 1-(int)radius; i <= (int)radius; i++){
			// 		const uint idxx = ((int)idx[0] + i);
			// 		const uint idxy = ((int)idx[1] + j);

			// 		if ((idxx < fs->vx.dim_x()) && (idxy < fs->vy.dim_y())){
			// 			fs->obs(idxx, idxy) = 1.0f;
			// 		}
			// 	}
			// }

			for (int j = 1-(int)radius; j <= (int)radius; j++){
				for (int i = -(int)sqrtf(sq(radius)-sq(j)); i <= (int)sqrtf(sq(radius)-sq(j)); i++){
					const uint idxx = ((int)idx[0] + i);
					const uint idxy = ((int)idx[1] + j);

					if ((idxx < fs->vx.dim_x()) && (idxy < fs->vy.dim_y())){
						fs->obs(idxx, idxy) = 1.0f;
					}
				}
			}
		}else if (shift_pressed){
			for (int j = 1-(int)radius; j <= (int)radius; j++){
				for (int i = 1-(int)radius; i <= (int)radius; i++){
					const uint idxx = ((int)idx[0] + i);
					const uint idxy = ((int)idx[1] + j);

					if ((idxx < fs->vx.dim_x()) && (idxy < fs->vy.dim_y())){
						fs->obs(idxx, idxy) = 0.0f;
					}
				}
			}
		}else{
			fs->user_idx = min(idx, vecu2{fs->vx.dim_x()-1, fs->vy.dim_y()-1});
			fs->user_v = move;
		}
		

		// if (pthread_mutex_lock(fs_mutex) == 0){
			// fs->vx(idx[0], idx[1]) = move.x;
			// fs->vy(idx[0], idx[1]) = move.y;

			// fs->vx(idx[0], idx[1]) = scale;
			// fs->vy(idx[0], idx[1]) = scale;


		// 	pthread_mutex_unlock(fs_mutex);
		// }


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
	}else if(mouse_press == Qt::RightButton){
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
	}else{
		fs->user_idx = vecu2{0,0};
		fs->user_v = vecf2();
	}

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

	mouse_old = mouse;
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

void MainW::init_frame_data(){
	frame_data = new float[frame.size() * max_frame_count];
	vx_frame_data = new float[vx_frame.size() * max_frame_count];
	vy_frame_data = new float[vy_frame.size() * max_frame_count];
}

void MainW::init_frame(){
	frame = fieldf(fs->dim, fs->geom);
	vx_frame = fieldf(fs->vx.dim(), fs->vx.domain());
	vy_frame = fieldf(fs->vy.dim(), fs->vy.domain());
}

void MainW::init_recording(){
	max_frame_count = 100;
	cur_frame_count = 1;
	cur_rec_frame = 0;
	cur_play_frame = 0;
	init_frame();
	init_frame_data();
}

void MainW::record_frame(){

	// record_count++;

	// p_file.seekp(0,ios_base::beg);
	// p_file.write((char*)&record_count, sizeof(uint));
	// p_file.seekp(0,ios_base::end);
	// p_file.write((char*)fs->p.data(), fs->.p.size()*sizeof(float));
	// vx_file.write((char*)fs->p.data(), fs->.vx.size()*sizeof(float));
	// vy_file.write((char*)fs->p.data(), fs->.vy.size()*sizeof(float));

	const uint idx = cur_rec_frame * frame.size();
	memcpy(&frame_data[idx], fs->p.data(), frame.size() * sizeof(float));

	const uint vx_idx = cur_rec_frame * vx_frame.size();
	memcpy(&vx_frame_data[vx_idx], fs->vx.data(), vx_frame.size() * sizeof(float));

	const uint vy_idx = cur_rec_frame * vy_frame.size();
	memcpy(&vy_frame_data[vy_idx], fs->vy.data(), vy_frame.size() * sizeof(float));


	cur_frame_count = min(max_frame_count, cur_frame_count+1);
	cur_rec_frame = (cur_rec_frame + 1) % cur_frame_count;

	printf("%u\t%u\t%u\t%u\n", max_frame_count, cur_frame_count, cur_rec_frame, cur_play_frame);
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

void MainW::radius_sb_slot(int val){
	radius = val;
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


void MainW::set_p_render_slot(int val){
	p_render = val;
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




void MainW::nse_commit_slot(){

	pthread_mutex_lock(fs_mutex);

	fs->set_reynold(ctrl_w->reynold_dsb->value()); 
	fs->deriv_w = ctrl_w->deriv_weight_dsb->value();
	fs->border_v = ctrl_w->border_v_dsb->value();
	fs->jacobi_it_max = ctrl_w->jacobi_it_max_sb->value();
	fs->jacobi_w = ctrl_w->jacobi_weight_dsb->value();

	fs->down_boundx = ctrl_w->downx_cb->currentIndex() +1;
	fs->down_boundy = ctrl_w->downy_cb->currentIndex() +1;
	fs->up_boundx = ctrl_w->upx_cb->currentIndex() +1;
	fs->up_boundy = ctrl_w->upy_cb->currentIndex() +1;
	fs->left_boundx = ctrl_w->leftx_cb->currentIndex() +1;
	fs->left_boundy = ctrl_w->lefty_cb->currentIndex() +1;
	fs->right_boundx = ctrl_w->rightx_cb->currentIndex() +1;
	fs->right_boundy = ctrl_w->righty_cb->currentIndex() +1;

	fs->obs_number = ctrl_w->obs_cb->currentIndex();
	

	pthread_mutex_unlock(fs_mutex);
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



	if (main_w->play_recording && (main_w->cur_play_frame < main_w->cur_frame_count)){


		const uint frame_data_idx = main_w->cur_play_frame * main_w->frame.size();
		main_w->frame.copy_data(&main_w->frame_data[frame_data_idx]);

		const uint vx_frame_data_idx = main_w->cur_play_frame * main_w->vx_frame.size();
		main_w->vx_frame.copy_data(&main_w->vx_frame_data[vx_frame_data_idx]);

		const uint vy_frame_data_idx = main_w->cur_play_frame * main_w->vy_frame.size();
		main_w->vy_frame.copy_data(&main_w->vy_frame_data[vy_frame_data_idx]);


		const vecf2 bound0 = pix_stoi( main_w->frame.domain().min + main_w->frame.grid_to_domain().slope );
		const vecf2 bound1 = pix_stoi( main_w->frame.domain().max - main_w->frame.grid_to_domain().slope );

		const int i_min = ceilf(bound0.x);
		const int i_max = floorf(bound1.x);
		const int j_min = ceilf(bound1.y);
		const int j_max = floorf(bound0.y);

		

		if (main_w->ctrl_w->p_rb->isChecked()){ // render p
			for (int i = i_min; i <= i_max; i++){
				for (int j = j_min; j <= j_max; j++){
					const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
					const vecf2 pos = pix_itos(idx);

					const float value = main_w->frame(pos) * main_w->p_ref;

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

					const float mag = sqrtf( xmath::op::sq(main_w->vx_frame(pos)) + xmath::op::sq(main_w->vy_frame(pos)) );
					const float value = mag * main_w->v_ref;
					const color_rgbf tmp = 255.0f * (main_w->v_colormap)(value);
					const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
					map.setPixel(i,j,col);
				}
			}

		}

		main_w->cur_play_frame = (main_w->cur_play_frame + 1) % main_w->cur_frame_count;

		const QRect rect = QRect(0, 0, width(), height());
		painter.drawImage(rect, map, rect);


		if (main_w->ctrl_w->stream_render_chb->isChecked()){
			const float path_step = 0.005f;

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
					const vecf2 tmp_pos = pos + path_step * vecf2(main_w->vx_frame(pos), main_w->vy_frame(pos));
				
					if (main_w->vx_frame.out_bound(tmp_pos) || main_w->vy_frame.out_bound(tmp_pos))
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
			const float part_step = 0.005f;

			for (int i = 0; i < main_w->part_count; i++){
				vecf2 pos = main_w->part_pos[i];
				vecf2 idx = pix_stoi(pos);

				painter.drawPoint(idx[0], idx[1]);

				// const vecf2 tmp_pos = pos + part_step * vecf2(cfs().vx(pos), cfs().vy(pos));
				const vecf2 tmp_pos = pos + part_step * vecf2(main_w->vx_frame(pos), main_w->vy_frame(pos));

				// if (cfs().vx.out_bound(tmp_pos) || cfs().vy.out_bound(tmp_pos)){
				// 	main_w->part_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(cfs().vx.space).x + cfs().vx.space.min.x;
				// 	main_w->part_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(cfs().vy.space).y + cfs().vy.space.min.y;
				// 	continue;
				// }

				if (main_w->vx_frame.out_bound(tmp_pos) || main_w->vy_frame.out_bound(tmp_pos)){
					main_w->part_pos[i][0] = (float(rand())/float(RAND_MAX)) * len(main_w->fs->geom).x + main_w->fs->geom.min.x;
					main_w->part_pos[i][1] = (float(rand())/float(RAND_MAX)) * len(main_w->fs->geom).y + main_w->fs->geom.min.y;
					continue;
				}
				
				main_w->part_pos[i] = tmp_pos;
			}
		}


		return;
	}


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

		for (int i = i_min; i <= i_max; i++){
			for (int j = j_min; j <= j_max; j++){
				const vecf2 idx{static_cast<float>(i), static_cast<float>(j)};
				const vecf2 pos = pix_itos(idx);

				const color_rgbf tmp = main_w->obs_colormap(main_w->fs->obs(pos));
				const QColor map_col = map.pixel(i,j);
				const QRgb col = qRgb(map_col.red() * tmp[0],map_col.green() * tmp[1], map_col.blue() * tmp[2]);
				map.setPixel(i,j,col);
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
	// main_gb = new QGroupBox("controls:", this);

	// main_sa = new QScrollArea(main_gb);
	// main_sa->setBackgroundRole(QPalette::Midlight);
	// main_sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// main_sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	// main_sw = new QWidget(main_sa);

	scroll_w = new ScrollW(this);
	

	play_pb = new QPushButton("play/pause", this);
	connect(play_pb, SIGNAL(clicked()), main_w, SLOT(play_slot()));

	clear_pb = new QPushButton("clear", this);
	connect(clear_pb, SIGNAL(clicked()), main_w, SLOT(clear_slot()));


	mouse_int_gb = new QGroupBox("mouse interaction", this);

	mouse_int_cb = new QComboBox(this);
	mouse_int_cb->addItem("off");
	// mouse_int_cb->addItem("get current vx here");
	// mouse_int_cb->addItem("get current vy here");
	// mouse_int_cb->addItem("get current p here");
	mouse_int_cb->addItem("raise vx temp");
	mouse_int_cb->addItem("raise vy temp");
	mouse_int_cb->addItem("raise p temp");
	mouse_int_cb->addItem("set vx persis");
	mouse_int_cb->addItem("set vy persis");
	mouse_int_cb->addItem("set p persis");
	mouse_int_cb->addItem("set obstacle persis");
	mouse_int_cb->addItem("erase persistents");

	radius_l = new QLabel("interaction radius", this);
	radius_sb = new QSpinBox(this);
	radius_sb->setRange(0, 100);
	radius_sb->setValue(1);
	connect(radius_sb, SIGNAL(valueChanged(int)), main_w, SLOT(radius_sb_slot(int)));


	QVBoxLayout *mouse_int_layout = new QVBoxLayout(mouse_int_gb);
	mouse_int_layout->addWidget(mouse_int_cb);


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
	// render_gb = new QGroupBox("render:", this);

	render_tw = new ToggleW("render settings", scroll_w);

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


	colormap_gb = new QGroupBox("colormap", this);
	colormap_gb->setCheckable(true);

	// colormap_tw = new ToggleW("colormap settings", this);

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

	// QVBoxLayout *colormap_tw_layout = new QVBoxLayout(colormap_tw->main_gb);
	// colormap_tw_layout->addWidget(p_rb);
	// colormap_tw_layout->addWidget(v_rb);
	// colormap_tw_layout->addWidget(vx_rb);
	// colormap_tw_layout->addWidget(vy_rb);
	// colormap_tw_layout->addWidget(res_rb);

	p_colormap_ref_dsb = new QDoubleSpinBox(this);
	p_colormap_ref_dsb->setRange(0.0f, 100.0f);
	p_colormap_ref_dsb->setDecimals(4);
	p_colormap_ref_dsb->setSingleStep(0.001f);
	connect(p_colormap_ref_dsb, SIGNAL(valueChanged(double)), main_w, SLOT(set_colormap_ref_slot(double)));


	// QVBoxLayout *render_gb_layout = new QVBoxLayout(render_gb);
	render_tw->addWidget(stream_render_chb);
	render_tw->addWidget(rand_pos_size_sb);
	render_tw->addWidget(gen_rand_pos_pb);
	render_tw->addWidget(part_render_chb);
	render_tw->addWidget(part_count_sb);
	render_tw->addWidget(gen_part_pos_pb);
	render_tw->addWidget(colormap_gb);
	// render_tw->addWidget(colormap_tw);
	render_tw->addWidget(p_colormap_ref_dsb);
	

	// nse tw
	nse_tw = new ToggleW("nse settings", scroll_w);


	nse_commit_pb = new QPushButton("commit parameters", nse_tw);
	connect(nse_commit_pb, SIGNAL(clicked()), main_w, SLOT(nse_commit_slot()));

	reynold_l = new QLabel("reynold number", this);
	reynold_dsb = new QDoubleSpinBox(this);
	reynold_dsb->setRange(0.0, 10000.0);
	reynold_dsb->setSingleStep(10.0);
	reynold_dsb->setValue(100.0f);

	deriv_weight_l = new QLabel("derivative weight:", nse_tw);
	deriv_weight_dsb = new QDoubleSpinBox(nse_tw);
	deriv_weight_dsb->setDecimals(2);
	deriv_weight_dsb->setRange(0, 1.0);
	deriv_weight_dsb->setSingleStep(0.01);
	deriv_weight_dsb->setValue(main_w->fs->deriv_weight());

	jacobi_it_max_l = new QLabel("jacobi max it", nse_tw);
	jacobi_it_max_sb = new QSpinBox(nse_tw);
	jacobi_it_max_sb->setRange(1, 10000);
	jacobi_it_max_sb->setValue(main_w->fs->jacobi_max_iteration());

	jacobi_weight_l = new QLabel("jacobi weight", this);
	jacobi_weight_dsb = new QDoubleSpinBox(this);
	jacobi_weight_dsb->setDecimals(2);
	jacobi_weight_dsb->setRange(0, 1.0);
	jacobi_weight_dsb->setSingleStep(0.01);
	jacobi_weight_dsb->setValue(main_w->fs->jacobi_weight());

	border_v_l = new QLabel("border velocity:", nse_tw);
	border_v_dsb = new QDoubleSpinBox(this);
	border_v_dsb->setDecimals(2);
	border_v_dsb->setRange(0, 1000.0);
	border_v_dsb->setSingleStep(0.01);
	border_v_dsb->setValue(main_w->fs->border_speed());







	// border conditions
	downx_cb = new QComboBox(nse_tw);
	downx_cb->addItem("no slip");
	downx_cb->addItem("driven +w");
	downx_cb->addItem("driven -w");
	downx_cb->addItem("slip");
	downx_cb->addItem("periodic");

	downy_cb = new QComboBox(nse_tw);
	downy_cb->addItem("closed");
	downy_cb->addItem("inflow");
	downy_cb->addItem("outflow");
	downy_cb->addItem("open");
	downy_cb->addItem("periodic");

	border_down_l = new QLabel("Down vx vy", nse_tw);
	border_down_w = new QWidget(nse_tw);
	QHBoxLayout *border_down_w_layout = new QHBoxLayout(border_down_w);
	border_down_w_layout->addWidget(downx_cb);
	border_down_w_layout->addWidget(downy_cb);

	upx_cb = new QComboBox(nse_tw);
	upx_cb->addItem("no slip");
	upx_cb->addItem("driven +w");
	upx_cb->addItem("driven -w");
	upx_cb->addItem("slip");
	upx_cb->addItem("periodic");

	upy_cb = new QComboBox(nse_tw);
	upy_cb->addItem("closed");
	upy_cb->addItem("inflow");
	upy_cb->addItem("outflow");
	upy_cb->addItem("open");
	upy_cb->addItem("periodic");

	border_up_l = new QLabel("Up vx vy", nse_tw);
	border_up_w = new QWidget(nse_tw);
	QHBoxLayout *border_up_w_layout = new QHBoxLayout(border_up_w);
	border_up_w_layout->addWidget(upx_cb);
	border_up_w_layout->addWidget(upy_cb);

	lefty_cb = new QComboBox(nse_tw);
	lefty_cb->addItem("no slip");
	lefty_cb->addItem("driven +w");
	lefty_cb->addItem("driven -w");
	lefty_cb->addItem("slip");
	lefty_cb->addItem("periodic");

	leftx_cb = new QComboBox(nse_tw);
	leftx_cb->addItem("closed");
	leftx_cb->addItem("inflow");
	leftx_cb->addItem("outflow");
	leftx_cb->addItem("open");
	leftx_cb->addItem("periodic");

	border_left_l = new QLabel("Left vx vy", nse_tw);
	border_left_w = new QWidget(nse_tw);
	QHBoxLayout *border_left_w_layout = new QHBoxLayout(border_left_w);
	border_left_w_layout->addWidget(leftx_cb);
	border_left_w_layout->addWidget(lefty_cb);

	righty_cb = new QComboBox(nse_tw);
	righty_cb->addItem("no slip");
	righty_cb->addItem("driven +w");
	righty_cb->addItem("driven -w");
	righty_cb->addItem("slip");
	righty_cb->addItem("periodic");

	rightx_cb = new QComboBox(nse_tw);
	rightx_cb->addItem("closed");
	rightx_cb->addItem("inflow");
	rightx_cb->addItem("outflow");
	rightx_cb->addItem("open");
	rightx_cb->addItem("periodic");

	border_right_l = new QLabel("Right vx vy", nse_tw);
	border_right_w = new QWidget(nse_tw);
	QHBoxLayout *border_right_w_layout = new QHBoxLayout(border_right_w);
	border_right_w_layout->addWidget(rightx_cb);
	border_right_w_layout->addWidget(righty_cb);
	// border cond end

	// los obstaclos
	obs_l = new QLabel("Los Obstaclos", nse_tw);
	obs_w = new QGroupBox(nse_tw);
	QVBoxLayout *obs_w_layout = new QVBoxLayout(obs_w);
	obs_cb = new QComboBox(nse_tw);
	obs_cb->addItem("none");
	obs_cb->addItem("sphere");
	obs_cb->addItem("box");
	obs_cb->addItem("triangel");
	obs_cb->addItem("parabol");
	obs_cb->addItem("diagonal box");
	obs_w_layout->addWidget(obs_cb);



	

	nse_tw->addWidget(nse_commit_pb);
	nse_tw->addWidget(reynold_l);
	nse_tw->addWidget(reynold_dsb);
	nse_tw->addWidget(deriv_weight_l);
	nse_tw->addWidget(deriv_weight_dsb);
	nse_tw->addWidget(jacobi_it_max_l);
	nse_tw->addWidget(jacobi_it_max_sb);
	nse_tw->addWidget(jacobi_weight_l);
	nse_tw->addWidget(jacobi_weight_dsb);
	nse_tw->addWidget(border_v_l);
	nse_tw->addWidget(border_v_dsb);
	nse_tw->addWidget(border_down_l);
	nse_tw->addWidget(border_down_w);
	nse_tw->addWidget(border_up_l);
	nse_tw->addWidget(border_up_w);
	nse_tw->addWidget(border_left_l);
	nse_tw->addWidget(border_left_w);
	nse_tw->addWidget(border_right_l);
	nse_tw->addWidget(border_right_w);
	nse_tw->addWidget(obs_l);
	nse_tw->addWidget(obs_w);



	
	// main layouts
	scroll_w->addWidget(play_pb);
	scroll_w->addWidget(clear_pb);
	scroll_w->addWidget(mouse_int_gb);
	scroll_w->addWidget(wave_gb);
	scroll_w->addToggleW(render_tw);
	scroll_w->addWidget(nse_tw);


	// main_sa->setWidget(main_sw);

	// QVBoxLayout *main_gb_layout = new QVBoxLayout(main_gb);
	// main_gb_layout->addWidget(main_sa);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(scroll_w);
}

void MainW::CtrlW::resizeEvent(QResizeEvent* event){
	
}

MainW::InfoW::InfoW(MainW *parent): QWidget(parent), main_w(parent){
	scroll_w = new ScrollW(this);	



	phd_tw = new ToggleW("physical data", this);

		geom_ilw = new InfoLabelW(phd_tw);
		geom_ilw->setName("geometry:");

		reynold_ilw = new InfoLabelW(phd_tw);
		reynold_ilw->setName("reynold:");

		force_ilw = new InfoLabelW(phd_tw);
		force_ilw->setName("force:");

		time_ilw = new InfoLabelW(phd_tw);
		time_ilw->setName("time:");

		max_vx_ilw = new InfoLabelW(phd_tw);
		max_vx_ilw->setName("max. x-speed:");

		max_vy_ilw = new InfoLabelW(phd_tw);
		max_vy_ilw->setName("max. y-speed:");

		max_v_ilw = new InfoLabelW(phd_tw);
		max_v_ilw->setName("max. speed:");

		mouse_p_ilw = new InfoLabelW("mouse pressure:", phd_tw);
		mouse_vx_ilw = new InfoLabelW("mouse x-speed:", phd_tw);
		mouse_vy_ilw = new InfoLabelW("mouse y-speed:", phd_tw);
		mouse_v_ilw = new InfoLabelW("mouse speed:", phd_tw);


	phd_tw->addWidget(geom_ilw);
	phd_tw->addWidget(reynold_ilw);
	phd_tw->addWidget(force_ilw);
	phd_tw->addWidget(time_ilw);
	phd_tw->addWidget(max_vx_ilw);
	phd_tw->addWidget(max_vy_ilw);
	phd_tw->addWidget(max_v_ilw);
	phd_tw->addWidget(mouse_p_ilw);
	phd_tw->addWidget(mouse_vx_ilw);
	phd_tw->addWidget(mouse_vy_ilw);
	phd_tw->addWidget(mouse_v_ilw);


	nd_tw = new ToggleW("numerical data",this);

		grid_dim_ilw = new InfoLabelW(nd_tw);
		grid_dim_ilw->setName("grid size:");

		x_step_ilw = new InfoLabelW(nd_tw);
		x_step_ilw->setName("x step:");

		y_step_ilw = new InfoLabelW(nd_tw);
		y_step_ilw->setName("y step:");

		it_ilw = new InfoLabelW(nd_tw);
		it_ilw->setName("iteration:");

		time_step_ilw = new InfoLabelW(nd_tw);
		time_step_ilw->setName("time step:");

		time_step_safe_ilw = new InfoLabelW(nd_tw);
		time_step_safe_ilw->setName("time step safety:");

		time_step_bound_ilw = new InfoLabelW(nd_tw);
		time_step_bound_ilw->setName("time step bound:");

		jacobi_max_iteration_ilw = new InfoLabelW(nd_tw);
		jacobi_max_iteration_ilw->setName("jacobi iterations:");

		jacobi_weight_ilw = new InfoLabelW(nd_tw);
		jacobi_weight_ilw->setName("jacobi weight:");

	nd_tw->addWidget(grid_dim_ilw);	
	nd_tw->addWidget(x_step_ilw);
	nd_tw->addWidget(y_step_ilw);
	nd_tw->addWidget(it_ilw);
	nd_tw->addWidget(time_step_ilw);
	nd_tw->addWidget(time_step_safe_ilw);
	nd_tw->addWidget(time_step_bound_ilw);
	nd_tw->addWidget(jacobi_max_iteration_ilw);
	nd_tw->addWidget(jacobi_weight_ilw);



	scroll_w->addToggleW(phd_tw);
	scroll_w->addToggleW(nd_tw);


	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(scroll_w);

	updateInfo();
}

void MainW::InfoW::updateInfo(){
	QString geom_str;
	geom_str += "[";
	geom_str += QString::number(fs()->geometry().min.x);
	geom_str += ", ";
	geom_str += QString::number(fs()->geometry().max.x);
	geom_str += "] x [";
	geom_str += QString::number(fs()->geometry().min.y);
	geom_str += ", ";
	geom_str += QString::number(fs()->geometry().max.y);
	geom_str += "]";
	geom_ilw->setInfo(geom_str);


	reynold_ilw->setInfo(QString::number(fs()->reynold(), 'f', 2));

	QString force_str;
	force_str += "(";
	force_str += QString::number(fs()->force().x);
	force_str += ", ";
	force_str += QString::number(fs()->force().y);
	force_str += ")";
	force_ilw->setInfo(force_str);


	time_ilw->setInfo(QString::number(fs()->time(), 'f', 6));

	max_vx_ilw->setInfo(QString::number(fs()->max_speed_x(), 'f', 6));
	max_vy_ilw->setInfo(QString::number(fs()->max_speed_y(), 'f', 6));
	max_v_ilw->setInfo(QString::number(fs()->max_speed(), 'f', 6));

	const float mp = fs()->p(main_w->render_w->pix_itos((vecf2)main_w->mouse));
	mouse_p_ilw->setInfo(QString::number(mp, 'f', 6));

	const float mvx = fs()->vx(main_w->render_w->pix_itos((vecf2)main_w->mouse));
	mouse_vx_ilw->setInfo(QString::number(mvx, 'f', 6));

	const float mvy = fs()->vy(main_w->render_w->pix_itos((vecf2)main_w->mouse));
	mouse_vy_ilw->setInfo(QString::number(mvy, 'f', 6));

	const float mv = sqrtf(sq(mvx)+sq(mvy));
	mouse_v_ilw->setInfo(QString::number(mv, 'f', 6));




	// numerical data
	QString grid_dim_str;
	grid_dim_str += QString::number(fs()->grid_dim()[0]);
	grid_dim_str += " x ";
	grid_dim_str += QString::number(fs()->grid_dim()[1]);
	grid_dim_ilw->setInfo(grid_dim_str);

	x_step_ilw->setInfo(QString::number(fs()->x_step()));
	y_step_ilw->setInfo(QString::number(fs()->y_step()));
	it_ilw->setInfo(QString::number(fs()->iteration()));
	time_step_ilw->setInfo(QString::number(fs()->time_step(), 'f', 6));
	time_step_safe_ilw->setInfo(QString::number(fs()->time_step_safe(), 'f', 6));
	time_step_bound_ilw->setInfo(QString::number(fs()->time_step_bound(), 'f', 6));
	jacobi_max_iteration_ilw->setInfo(QString::number(fs()->jacobi_max_iteration()));
	jacobi_weight_ilw->setInfo(QString::number(fs()->jacobi_weight(), 'f', 2));
}