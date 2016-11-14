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

#include <grid.h>
#include <cfs.h>
#include <colmap.h>


class MainW : public QWidget{
	Q_OBJECT


	public:
		MainW(CFS* in_cfs = nullptr, QWidget *parent = nullptr);


	public:
		class RenderW : public QWidget{
			static constexpr uint _fps_ = 60;
			static constexpr uint _refresh_time_ = 1000 / _fps_;


			public:
				RenderW(MainW *parent = nullptr);

				const CFS& cfs() const{return *(main_w->_cfs);}
				const colormap& color_map() const{return main_w->_colormap;}


			protected:
				void paintEvent(QPaintEvent *event);
				void resizeEvent(QResizeEvent *event);


			private:
				MainW *main_w;

				gridmap _pix_grid_map[2];

				QTimer *timer;
				bool ready;
		};


		class CtrlW : public QWidget{
			public:
				CtrlW(MainW* parent = nullptr);

			private:
				MainW *main_w;

				QGroupBox *main_gb;
				QSpinBox *grid_sb;
		};


	private:
		RenderW *render_w;
		CtrlW *ctrl_w;


	private:
		CFS *_cfs;
		colormap _colormap;


	private slots:
		void set_grid(int n){
			*_cfs = CFS{gridmap(0.0f, 2.0f, n), gridmap(0.0f, 1.0f, n)};
		}
};


#endif // __MAIN_W_H__