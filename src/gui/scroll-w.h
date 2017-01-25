#ifndef __SCROLL_W_H__
#define __SCROLL_W_H__

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QResizeEvent>

#include <gui/toggle-w.h>


class ScrollW : public QScrollArea{
	Q_OBJECT

	public:
		ScrollW(QWidget *parent = nullptr);

		void addWidget(QWidget *w);
		void addToggleW(ToggleW *tw);


	public slots:
		void adjustSizeCentralWidget();


	protected:
		void resizeEvent(QResizeEvent *event);


	private:
		QWidget *central_w;
		QVBoxLayout *layout;
};


#endif // __SCROLL_W_H__