#ifndef __RENDER_W_H__
#define __RENDER_W_H__

#include <iostream>
// #include <vector>
// #include <stdlib.h>

#include <QTimer>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

#include <grid.h>
#include <cfs.h>


class RenderW : public QWidget{
	Q_OBJECT


	static constexpr uint _fps_ = 60;
	static constexpr uint _refresh_time_ = 1000 / _fps_;


	public:
		RenderW(CFS* in_cfs = nullptr, QWidget *parent = 0): ready(true), _cfs(in_cfs){
			timer = new QTimer(this);
			connect(timer, SIGNAL(timeout()), this, SLOT(update()));
			// connect(timer, SIGNAL(timeout()), this, SLOT(log()));
			timer->start(_refresh_time_);
		}

		const CFS* cfs() const{return _cfs;}


	public slots:
		void log(){
			using namespace std;

			cout << "timer refresh" << endl;
		}


	protected:
		void paintEvent(QPaintEvent *event){
			if (!ready)
				return;

			QPainter painter(this);
			painter.setRenderHints(QPainter::Antialiasing, true);
			QImage map(width(), height(), QImage::Format_ARGB32);
			
			// const QRgb value = qRgb(255.0f * col_par, 255.0f * (1.0f - col_par), 255.0f * (1.0f - col_par));
			// map.setPixel(i, j, value);


			for (int i = 0; i < width(); i++){
				for (int j = 0; j < height(); j++){
					const float x = _pix_grid_map[0].cell_pos(i);
					const float y = _pix_grid_map[1].cell_pos(height() - j - 1);

					const float tmp = 255.0f * _cfs->p(x,y);
					const QRgb col = qRgb(tmp,tmp,tmp);
					map.setPixel(i,j,col);
				}
			}


			const QRect rect = QRect(0, 0, width(), height());
			painter.drawImage(rect, map, rect);
			painter.end();
		}

		void resizeEvent(QResizeEvent *event){
			const float ar = float(width())/float(height());
			_pix_grid_map[0].set_cell_count(width());
			_pix_grid_map[1].set_cell_count(height());
			_pix_grid_map[1].set_intvl(_cfs->grid_map()[1].min(), _cfs->grid_map()[1].max());
			const float l = _cfs->grid_map()[1].max() - _cfs->grid_map()[1].min();
			const float mid = 0.5f * (_cfs->grid_map()[0].min() + _cfs->grid_map()[0].max());

			_pix_grid_map[0].set_intvl(mid - 0.5f*ar*l, mid + 0.5f*ar*l);
		}

		// void keyPressEvent(QKeyEvent *event){
		// 	if (event->key() == Qt::Key_Space)
		// 		running = !running;
		// }


	private:
		gridmap _pix_grid_map[2];

		QTimer *timer;
		bool ready;

		CFS *_cfs;
};


#endif // __RENDER_W_H__