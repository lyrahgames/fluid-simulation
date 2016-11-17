#ifndef __MAIN_W_H__
#define __MAIN_W_H__

#include <iostream>
#include <vector>
// #include <stdlib.h>

#include <QTimer>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPushButton>

#include <grid.h>
#include <cfs.h>
#include <colmap.h>


class MainW : public QWidget{
	Q_OBJECT


	static constexpr uint _fps_ = 60;
	static constexpr uint _refresh_time_ = 1000 / _fps_;
	static constexpr uint _rand_pos_size_max_ = 1 << 15;
	

	public:
		MainW(CFS* in_cfs = nullptr, QWidget *parent = nullptr);
		~MainW();


	public slots:
		void repaint();
		void set_ready(bool in = true){ready = true;}

		void init_rand_pos();
		void gen_rand_pos();


	public:
		class RenderW : public QWidget{
			public:
				RenderW(MainW *parent = nullptr);

				const CFS& cfs() const{return *(main_w->_cfs);}
				const colormap& color_map() const{return main_w->_colormap;}


			protected:
				void paintEvent(QPaintEvent *event);
				void resizeEvent(QResizeEvent *event);
				void mouseMoveEvent(QMouseEvent *event){mouse_x = event->x(); mouse_y = event->y();ready() = true;event->ignore();}

			private:
				const bool& ready() const{return main_w->ready;}
				bool& ready(){return main_w->ready;}


			private:
				MainW *main_w;

				gridmap _pix_grid_map[2];
				float path_step;

				int mouse_x, mouse_y;
		};


		class CtrlW : public QWidget{
			public:
				CtrlW(MainW* parent = nullptr);

			private:
				MainW *main_w;

				QGroupBox *main_gb, *render_gb;
				QSpinBox *grid_sb;

				QSpinBox *rand_pos_size_sb;
				QPushButton *gen_rand_pos_pb;
		};


	protected:
		void mouseMoveEvent(QMouseEvent *event){mouse_x = event->x(); mouse_y = event->y();}
		void resizeEvent(QResizeEvent *event){set_ready();}


	private:
		RenderW *render_w;
		CtrlW *ctrl_w;

		bool ready;
		QTimer *timer;

		int mouse_x, mouse_y;


	private:
		CFS *_cfs;
		colormap _colormap;

		vec2 *rand_pos;
		uint rand_pos_size;


	private slots:
		void set_grid(int n){
			// *_cfs = CFS{gridmap(0.0f, 2.0f, n), gridmap(0.0f, 1.0f, n)};
			set_ready();
		}

		void set_rand_pos_size_slot(int i);
		void gen_rand_pos_slot();
};


#endif // __MAIN_W_H__