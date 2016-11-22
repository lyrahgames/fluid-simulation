#ifndef __MAIN_W_H__
#define __MAIN_W_H__

#include <iostream>
// #include <vector>
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

#include <util.h>
#include <cfs.h>
#include <colmap.h>
#include <io/term-out.h>


class MainW : public QWidget{
	Q_OBJECT


	static constexpr uint _fps_ = 60;
	static constexpr uint _refresh_time_ = 1000 / _fps_;

	static constexpr uint _rand_pos_size_max_ = 1 << 15;
	


	public:
		MainW(CFS* cfs_src = nullptr, QWidget *parent = nullptr);
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


			protected:
				void paintEvent(QPaintEvent *event);
				void resizeEvent(QResizeEvent *event);
				void mouseMoveEvent(QMouseEvent *event){event->ignore();}
				void mouseReleaseEvent(QMouseEvent *event){event->ignore();}
				void mousePressEvent(QMouseEvent *event){event->ignore();}


			private:
				const bool& ready() const{return main_w->ready;}
				void set_ready(bool a = true){main_w->ready = true;}
				const CFS& cfs() const{return *(main_w->cfs);}
				const colormap& colormap() const{return main_w->colormap;}


			// private:
			public:
				MainW *main_w;

				linmapv pix_itos; // pixel index to space
				linmapv pix_stoi; // pixel space to index
		};


		class CtrlW : public QWidget{
			public:
				CtrlW(MainW* parent = nullptr);

			private:
				MainW *main_w;

				QGroupBox *main_gb, *render_gb;
				QSpinBox *grid_sb;

				QPushButton * clear_pb;

				QSpinBox *rand_pos_size_sb;
				QPushButton *gen_rand_pos_pb;
		};


	protected:
		void mouseMoveEvent(QMouseEvent *event){
			mouse[0] = event->x() - render_w->pos().x();
			mouse[1] = event->y() - render_w->pos().y();
		}
		void mouseReleaseEvent(QMouseEvent *event){
			mouse_press = event->buttons();
		}
		void mousePressEvent(QMouseEvent *event){
			mouse_press = event->buttons();
		}
		void resizeEvent(QResizeEvent *event){
			set_ready();
		}


	private:
		RenderW *render_w;
		CtrlW *ctrl_w;

		bool ready;
		QTimer *timer;

		veci2 mouse;
		Qt::MouseButtons mouse_press;


	private:
		CFS *cfs;
		intvlv view;

		colormap colormap;

		vecf2 *rand_pos;
		uint rand_pos_size;


	private slots:
		void loop_slot();

		void set_grid(int n){
			// *_cfs = CFS{gridmap(0.0f, 2.0f, n), gridmap(0.0f, 1.0f, n)};
			set_ready();
		}

		void clear_slot();
		void set_rand_pos_size_slot(int i);
		void gen_rand_pos_slot();
};


#endif // __MAIN_W_H__