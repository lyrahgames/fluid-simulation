#ifndef __TOGGLE_W_H__
#define __TOGGLE_W_H__

#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>


class ToggleW : public QWidget{
	Q_OBJECT

	public:
		ToggleW(QWidget *parent = nullptr);
		ToggleW(const char *name, QWidget *parent = nullptr);

		void addWidget(QWidget *w);


	signals:
		void toggled();


	protected:


	private:
		QCheckBox *toggle_chb;
		QGroupBox *main_gb;
		QVBoxLayout *layout;


	private slots:
		void toggle_slot(int);
};


#endif // __TOGGLE_W_H__