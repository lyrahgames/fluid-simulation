#ifndef __MAIN_W_H__
#define __MAIN_W_H__

#include <iostream>
#include <fstream>
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
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenu>

#include <util.h>
#include <cfs.h>
#include <fluid-sim.h>
#include <colmap.h>
#include <io/term-out.h>

#include <gui/toggle-w.h>
#include <gui/scroll-w.h>
#include <gui/info-label-w.h>


class MainW : public QMainWindow{
	Q_OBJECT


	static constexpr uint _fps_ = 60;
	static constexpr uint _refresh_time_ = 1000 / _fps_;

	static constexpr uint _info_fps_ = 20;
	static constexpr uint _info_refresh_time_ = 1000 / _info_fps_;

	static constexpr uint _rand_pos_size_max_ = 1 << 15;
	static constexpr uint _part_count_max_ = 1<<18;
	


	public:
		MainW(CFS* cfs_src = nullptr, QWidget *parent = nullptr);
		MainW(fluid_sim* pfs, QWidget *parent = nullptr);
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
				void contextMenuEvent(QContextMenuEvent *event){ event->ignore(); }


			private:
				const bool& ready() const{return main_w->ready;}
				void set_ready(bool a = true){main_w->ready = true;}
				const CFS& cfs() const{return *(main_w->cfs);}
				const colormap& p_colormap() const{return main_w->p_colormap;}


			// private:
			public:
				MainW *main_w;

				linmapv pix_itos; // pixel index to space
				linmapv pix_stoi; // pixel space to index
		};


		class CtrlW : public QWidget{
			public:
				CtrlW(MainW* parent = nullptr);

			public:
				MainW *main_w;

				QGroupBox *main_gb;

				QScrollArea *main_sa;
				QWidget *main_sw;


				QGroupBox *render_gb;

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

				QLabel *border_l;
				QPushButton *border_pb;
				QComboBox *down_cb;
				QComboBox *up_cb;
				QComboBox *left_cb;
				QComboBox *right_cb;

				QCheckBox *stream_render_chb;
				QCheckBox *part_render_chb;

				QGroupBox *colormap_gb;
				ToggleW *colormap_tw;
				QRadioButton *p_rb;
				QRadioButton *v_rb;
				QRadioButton *vx_rb;
				QRadioButton *vy_rb;
				QRadioButton *res_rb;

				QDoubleSpinBox *p_colormap_ref_dsb;

				QSpinBox *jacobi_max_it_sb;


			protected:
				void resizeEvent(QResizeEvent *event);
		};

		class InfoW : public QWidget{
			public:
				InfoW(MainW *parent = nullptr);

				void updateInfo();


			private:	
				MainW *main_w;
				ScrollW *scroll_w;

				// physical data
				ToggleW *phd_tw;
				InfoLabelW *geom_ilw;
				InfoLabelW *reynold_ilw;
				InfoLabelW *force_ilw;
				InfoLabelW *time_ilw;
				InfoLabelW *max_vx_ilw;
				InfoLabelW *max_vy_ilw;
				InfoLabelW *max_v_ilw;

				// numerical data
				ToggleW *nd_tw;
				InfoLabelW *grid_dim_ilw;
				InfoLabelW *x_step_ilw;
				InfoLabelW *y_step_ilw;
				InfoLabelW *it_ilw;
				InfoLabelW *time_step_ilw;
				InfoLabelW *time_step_safe_ilw;
				InfoLabelW *time_step_bound_ilw;


			private:
				fluid_sim* fs(){return main_w->fs;}
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
		void contextMenuEvent(QContextMenuEvent *event){
			main_menu->exec(event->globalPos());
		}


	private:
		QMenu *main_menu;
		RenderW *render_w;
		CtrlW *ctrl_w;
		QDockWidget *ctrl_dw;


		InfoW *info_w;
		QDockWidget *info_dw;
		QTimer *info_update_t;


		bool ready;
		QTimer *timer;

		veci2 mouse;
		Qt::MouseButtons mouse_press;


	private:



		CFS *cfs;
		fluid_sim *fs;
		intvlv view;

		colormap p_colormap;
		colormap v_colormap;
		colormap res_colormap;
		float p_ref;
		float v_ref;
		float res_ref;

		vecf2 *rand_pos;
		uint rand_pos_size;

		vecf2 *part_pos;
		uint part_count;

		bool play;
		bool p_render;

	public:
		bool *fs_play;

		// fstream p_file;
		// fstream vx_file;
		// fstream vy_file;

		bool play_recording;
		// bool record_p;
		// uint record_count;
		// uint cur_it;
		// fieldf p_record;


		// recording
		uint max_frame_count;
		uint cur_frame_count;
		uint cur_rec_frame;
		uint cur_play_frame;
		float *frame_data;
		float *vx_frame_data;
		float *vy_frame_data;
		fieldf frame;
		fieldf vx_frame;
		fieldf vy_frame;
		QTimer *rec_timer;


	public slots:
		void record_frame();
		void init_frame_data();
		void init_frame();
		void init_recording();


	private slots:

		void info_update_info_slot(){info_w->updateInfo();}


		void loop_slot();

		void clear_slot();

		void set_rand_pos_size_slot(int i);
		void gen_rand_pos_slot();

		void set_part_count_slot(int i);
		void gen_part_pos_slot();

		void play_slot();

		void set_wave_damp_slot(double val);
		void set_wave_c_slot(double val);
		void set_wave_dt_slot(double val);

		// mens changes
		void set_reynold_slot(double val);

		// mens changes end
		void border_pb_slot();
		void set_p_render_slot(int val);

		void set_jacobi_max_it_slot(int val);

		void set_colormap_ref_slot(double val);

};


#endif // __MAIN_W_H__