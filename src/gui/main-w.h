#ifndef __MAIN_W_H__
#define __MAIN_W_H__

#include <pthread.h>

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

				// QGroupBox *main_gb;

				// QScrollArea *main_sa;
				// QWidget *main_sw;

				ScrollW *scroll_w;


				// QGroupBox *render_gb;

				QPushButton *play_pb;
				QPushButton *clear_pb;

				QGroupBox *mouse_int_gb;
				QComboBox *mouse_int_cb;


				ToggleW *render_tw;

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



				QCheckBox *stream_render_chb;
				QCheckBox *part_render_chb;

				QGroupBox *colormap_gb;
				// ToggleW *colormap_tw;
				QRadioButton *p_rb;
				QRadioButton *v_rb;
				QRadioButton *vx_rb;
				QRadioButton *vy_rb;
				QRadioButton *res_rb;

				QDoubleSpinBox *p_colormap_ref_dsb;

				
				// QGroupBox *nse_gb;
				ToggleW *nse_tw;
				QPushButton *nse_commit_pb;
				QLabel *reynold_l;
				QDoubleSpinBox *reynold_dsb;
				QLabel *deriv_weight_l;
				QDoubleSpinBox *deriv_weight_dsb;
				QLabel *jacobi_it_max_l;
				QSpinBox *jacobi_it_max_sb;
				QLabel *jacobi_weight_l;
				QDoubleSpinBox *jacobi_weight_dsb;
				QLabel *border_v_l;
				QDoubleSpinBox *border_v_dsb;

				QWidget *border_down_w;
				QWidget *border_up_w;
				QWidget *border_left_w;
				QWidget *border_right_w;
				QLabel *border_down_l;
				QLabel *border_up_l;
				QLabel *border_left_l;
				QLabel *border_right_l;

				QComboBox *downx_cb;
				QComboBox *downy_cb;
				QComboBox *upx_cb;
				QComboBox *upy_cb;
				QComboBox *leftx_cb;
				QComboBox *lefty_cb;
				QComboBox *rightx_cb;
				QComboBox *righty_cb;

				QLabel *obs_l;
				QWidget *obs_w;
				QComboBox *obs_cb;


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

				InfoLabelW *mouse_p_ilw;
				InfoLabelW *mouse_vx_ilw;
				InfoLabelW *mouse_vy_ilw;
				InfoLabelW *mouse_v_ilw;

				// numerical data
				ToggleW *nd_tw;
				InfoLabelW *grid_dim_ilw;
				InfoLabelW *x_step_ilw;
				InfoLabelW *y_step_ilw;
				InfoLabelW *it_ilw;
				InfoLabelW *time_step_ilw;
				InfoLabelW *time_step_safe_ilw;
				InfoLabelW *time_step_bound_ilw;
				InfoLabelW *jacobi_max_iteration_ilw;
				InfoLabelW *jacobi_weight_ilw;


			private:
				fluid_sim* fs(){return main_w->fs;}
		};


	protected:
		void mouseMoveEvent(QMouseEvent *event){
			// mouse_old = mouse;
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

		void keyReleaseEvent(QKeyEvent *event){
			if(event->key() == Qt::Key_Control){
				ctrl_pressed = false;
			}

			if(event->key() == Qt::Key_Shift){
				shift_pressed = false;
			}

			// ctrl_pressed = !(event->key() == Qt::Key_Control);
			// shift_pressed = !(event->key() == Qt::Key_Shift);
			printf("%i %i\n", ctrl_pressed, shift_pressed);
		}

		void keyPressEvent(QKeyEvent *event){
			ctrl_pressed = (event->key() == Qt::Key_Control);
			shift_pressed = (event->key() == Qt::Key_Shift);

			printf("%i %i\n", ctrl_pressed, shift_pressed);
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
		veci2 mouse_old;
		Qt::MouseButtons mouse_press;

		bool ctrl_pressed;
		bool shift_pressed;


	private:



		CFS *cfs;
		fluid_sim *fs;
		intvlv view;

		colormap p_colormap;
		colormap v_colormap;
		colormap res_colormap;
		colormap obs_colormap;
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
		pthread_mutex_t *fs_mutex;

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

		void set_p_render_slot(int val);

		void set_colormap_ref_slot(double val);

		void nse_commit_slot();

};


#endif // __MAIN_W_H__