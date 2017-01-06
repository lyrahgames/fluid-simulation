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
#include <QDoubleSpinBox>
#include <QLabel>
#include <QScrollArea>

#include <util.h>
#include <cfs.h>
#include <colmap.h>
#include <io/term-out.h>


class MainW : public QWidget{
	Q_OBJECT


	static constexpr uint _fps_ = 60;
	static constexpr uint _refresh_time_ = 1000 / _fps_;

	static constexpr uint _rand_pos_size_max_ = 1 << 15;
	static constexpr uint _part_count_max_ = 1<<18;
	


	public:
		MainW(CFS* cfs_src = nullptr, QWidget *parent = nullptr);
		~MainW();


	public slots:
		void repaint();
		void set_ready(bool in = true){ready = true;}

		void init_rand_pos();
		void gen_rand_pos();
		void init_part_pos();
		void gen_part_pos();


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

				QGroupBox *main_gb;

				QScrollArea *main_sa;
				QWidget *main_sw;


				QGroupBox *render_gb;
				QSpinBox *grid_sb;

				QPushButton *play_pb;
				QPushButton *clear_pb;

				QSpinBox *rand_pos_size_sb;
				QPushButton *gen_rand_pos_pb;

				QSpinBox *part_count_sb;
				QPushButton *gen_part_pos_pb;


				QGroupBox *wave_gb;

				QLabel *wave_damp_l;
				QDoubleSpinBox *wave_damp_dsb;
				QLabel *wave_c_l;
				QDoubleSpinBox *wave_c_dsb;
				QLabel *wave_dt_l;
				QDoubleSpinBox *wave_dt_dsb;

				QGroupBox *nse_gb;
				QLabel *reynold_l;
				QDoubleSpinBox *reynold_dsb;

			protected:
				void resizeEvent(QResizeEvent *event);
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

		vecf2 *part_pos;
		uint part_count;

		bool play;


	private slots:
		void loop_slot();

		void set_grid(int n){
			// *_cfs = CFS{gridmap(0.0f, 2.0f, n), gridmap(0.0f, 1.0f, n)};
			set_ready();
		}

		void clear_slot();

		void set_rand_pos_size_slot(int i);
		void gen_rand_pos_slot();

		void set_part_count_slot(int i);
		void gen_part_pos_slot();

		void play_slot();

		void set_wave_damp_slot(double val);
		void set_wave_c_slot(double val);
		void set_wave_dt_slot(double val);

		void set_reynold_slot(double val);
};


#endif // __MAIN_W_H__