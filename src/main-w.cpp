#include "main-w.h"


MainW::MainW(CFS* in_cfs, QWidget *parent): QWidget(parent), ready(true), _cfs(in_cfs), rand_pos(nullptr), rand_pos_size(0){
	setMouseTracking(true);

	// init rendering
	render_w = new RenderW(this);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	timer->start(_refresh_time_);


	ctrl_w = new CtrlW(this);
	ctrl_w->setFixedWidth(200);


	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(render_w);
	layout->addWidget(ctrl_w);


	_colormap.add_base({0.0f, {0.0f,0.5f,1.0f}});
	_colormap.add_base({1.0f, {1.0f,0.5f,0.0f}});
	_colormap.add_base({0.33f, {0.0f,1.0f,1.0f}});
	_colormap.add_base({0.66f, {1.0f,1.0f,0.0f}});
	_colormap.add_base({0.5f, {1,1,1}});
	_colormap.add_base({1.1f, {1,0,0}});
	_colormap.add_base({-0.1f, {0,0,1}});


	init_rand_pos();
	rand_pos_size = 100;
}

MainW::~MainW(){
	delete[] rand_pos;
}

void MainW::repaint(){
	_cfs->compute_time_it();

	if (!ready)
		return;

	update();
}

void MainW::init_rand_pos(){
	if (rand_pos != nullptr)
		delete[] rand_pos;

	rand_pos = new vec2[_rand_pos_size_max_];

	gen_rand_pos();
}

void MainW::gen_rand_pos(){
	for (uint i = 0; i < _rand_pos_size_max_; i++){
		rand_pos[i][0] = (float(rand())/float(RAND_MAX)) * _cfs->grid_map()[0].length() + _cfs->grid_map()[0].min();
		rand_pos[i][1] = (float(rand())/float(RAND_MAX)) * _cfs->grid_map()[1].length() + _cfs->grid_map()[1].min();
	}
}

void MainW::set_rand_pos_size_slot(int i){
	rand_pos_size = i;
	set_ready();
}

void MainW::gen_rand_pos_slot(){
	gen_rand_pos();
	set_ready();
}

MainW::RenderW::RenderW(MainW *parent): QWidget(parent), main_w(parent){
	path_step = 0.05f;

	setMouseTracking(true);
}

void MainW::RenderW::paintEvent(QPaintEvent *event){
	if (!ready())
		return;

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing, true);
	QImage map(width(), height(), QImage::Format_ARGB32);
	map.fill(Qt::black);


	const int i_min = ceil( _pix_grid_map[0].cell_idx(cfs().grid_map()[0].node_pos(1)) );
	const int i_max = floor( _pix_grid_map[0].cell_idx(cfs().grid_map()[0].node_pos(cfs().grid_map()[0].size()-1)) );
	const int j_min = ceil( _pix_grid_map[1].cell_idx(cfs().grid_map()[1].node_pos(1)) );
	const int j_max = floor( _pix_grid_map[1].cell_idx(cfs().grid_map()[1].node_pos(cfs().grid_map()[1].size()-1)) );


	// const int i_min = 0;
	// const int i_max = width()-1;
	// const int j_min = 0;
	// const int j_max = height()-1;

	for (int i = i_min; i <= i_max; i++){
		for (int j = j_min; j <= j_max; j++){
			const float x = _pix_grid_map[0].cell_pos(i);
			const float y = _pix_grid_map[1].cell_pos(height() - j - 1);

			const color_rgbf tmp = 255.0f * color_map()(cfs().p_save(x,y));
			const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
			map.setPixel(i,j,col);
		}
	}

	const QRect rect = QRect(0, 0, width(), height());
	painter.drawImage(rect, map, rect);


	for (int i = 0; i < main_w->rand_pos_size; i++){
		// float pos_x = _pix_grid_map[0].cell_pos(mouse_x);
		// float pos_y = _pix_grid_map[1].cell_pos(height()-mouse_y);
		// QPainterPath path(QPointF(mouse_x, mouse_y));
		float pos_x = main_w->rand_pos[i][0];
		float pos_y = main_w->rand_pos[i][1];
		const float idxx = _pix_grid_map[0].cell_idx(pos_x);
		const float idxy = _pix_grid_map[1].cell_idx(pos_y);
		QPainterPath path(QPointF(idxx,height()-idxy));

		for (int t = 0; t < 1000; t++){
			const float tmp_pos_x = pos_x + path_step * cfs().vx(pos_x, pos_y);
			const float tmp_pos_y = pos_y + path_step * cfs().vy(pos_x, pos_y);
		
			if (cfs().pos_out_vx_bound(tmp_pos_x, tmp_pos_y) || cfs().pos_out_vy_bound(tmp_pos_x, tmp_pos_y))
				break;

			pos_x = tmp_pos_x;
			pos_y = tmp_pos_y;

			const float idxx = _pix_grid_map[0].cell_idx(pos_x);
			const float idxy = _pix_grid_map[1].cell_idx(pos_y);

			path.lineTo(idxx, height()-idxy);
		}

		painter.setBrush(Qt::NoBrush);
		painter.drawPath(path);
	}



	painter.end();

	ready() = false;
}

void MainW::RenderW::resizeEvent(QResizeEvent *event){
	_pix_grid_map[0].set_cell_count(width());
	_pix_grid_map[1].set_cell_count(height());

	const float grid_ar = cfs().grid_map()[0].length() / cfs().grid_map()[1].length();			
	const float ar = float(width())/float(height());
	// const double ar = double(event->size().width())/double(event->size().height());

	if (grid_ar/ar <= 1.0f){
		_pix_grid_map[1].set_intvl(cfs().grid_map()[1].min(), cfs().grid_map()[1].max());
		const float l = cfs().grid_map()[1].max() - cfs().grid_map()[1].min();
		const float mid = 0.5f * (cfs().grid_map()[0].min() + cfs().grid_map()[0].max());

		_pix_grid_map[0].set_intvl(mid - 0.5f*ar*l, mid + 0.5f*ar*l);
	}else{
		const float ar = float(height())/float(width());
		_pix_grid_map[0].set_intvl(cfs().grid_map()[0].min(), cfs().grid_map()[0].max());
		const float l = cfs().grid_map()[0].max() - cfs().grid_map()[0].min();
		const float mid = 0.5f * (cfs().grid_map()[1].min() + cfs().grid_map()[1].max());

		_pix_grid_map[1].set_intvl(mid - 0.5f*ar*l, mid + 0.5f*ar*l);
	}

	ready() = true;
}

MainW::CtrlW::CtrlW(MainW *parent): QWidget(parent), main_w(parent){
	main_gb = new QGroupBox("controls:", this);
	main_gb->setFixedWidth(200);
	// main_gb->setFlat(false);

	QVBoxLayout *main_gb_layout = new QVBoxLayout(main_gb);

	grid_sb = new QSpinBox(this);
	grid_sb->setMinimum(3);
	grid_sb->setMaximum(10000);
	grid_sb->setValue(100);
	main_gb_layout->addWidget(grid_sb);
	connect(grid_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_grid(int)));


	render_gb = new QGroupBox("render:", this);
	QVBoxLayout *render_gb_layout = new QVBoxLayout(render_gb);

	rand_pos_size_sb = new QSpinBox(this);
	rand_pos_size_sb->setMinimum(0);
	rand_pos_size_sb->setMaximum(MainW::_rand_pos_size_max_);
	rand_pos_size_sb->setValue(main_w->rand_pos_size);
	render_gb_layout->addWidget(rand_pos_size_sb);
	connect(rand_pos_size_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_rand_pos_size_slot(int)));

	gen_rand_pos_pb = new QPushButton("gen rand pos", this);
	render_gb_layout->addWidget(gen_rand_pos_pb);
	connect(gen_rand_pos_pb, SIGNAL(clicked()), main_w, SLOT(gen_rand_pos_slot()));

	render_gb->adjustSize();
	main_gb_layout->addWidget(render_gb);
	

	main_gb->adjustSize();
}