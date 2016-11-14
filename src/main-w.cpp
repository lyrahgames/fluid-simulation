#include "main-w.h"


MainW::MainW(CFS* in_cfs, QWidget *parent): QWidget(parent), _cfs(in_cfs){
	render_w = new RenderW(this);

	ctrl_w = new CtrlW(this);
	ctrl_w->setFixedWidth(200);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(render_w);
	layout->addWidget(ctrl_w);


	_colormap.add_base({0.0f, {0.0f,0.0f,1.0f}});
	_colormap.add_base({1.0f, {1.0f,0.0f,0.0f}});
	_colormap.add_base({0.33f, {1.0f,1.0f,0.0f}});
	_colormap.add_base({0.66f, {1.0f,1.0f,1.0f}});			
}

MainW::RenderW::RenderW(MainW *parent): QWidget(parent), main_w(parent), ready(true){
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(_refresh_time_);
}

void MainW::RenderW::paintEvent(QPaintEvent *event){
	if (!ready)
		return;

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing, true);
	QImage map(width(), height(), QImage::Format_ARGB32);


	for (int i = 0; i < width(); i++){
		for (int j = 0; j < height(); j++){
			const float x = _pix_grid_map[0].cell_pos(i);
			const float y = _pix_grid_map[1].cell_pos(height() - j - 1);

			const color_rgbf tmp = 255.0f * color_map()(cfs().p(x,y));
			const QRgb col = qRgb(tmp[0],tmp[1],tmp[2]);
			map.setPixel(i,j,col);
		}
	}


	const QRect rect = QRect(0, 0, width(), height());
	painter.drawImage(rect, map, rect);
	painter.end();
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
}

MainW::CtrlW::CtrlW(MainW *parent): QWidget(parent), main_w(parent){
	main_gb = new QGroupBox("controls:", this);
	QVBoxLayout *main_gb_layout = new QVBoxLayout(main_gb);

	grid_sb = new QSpinBox(this);
	main_gb_layout->addWidget(grid_sb);
	connect(grid_sb, SIGNAL(valueChanged(int)), main_w, SLOT(set_grid(int)));

	main_gb->adjustSize();
}