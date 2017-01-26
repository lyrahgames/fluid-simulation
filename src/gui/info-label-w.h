#ifndef __INFO_LABEL_W_H__
#define __INFO_LABEL_W_H__

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>


class InfoLabelW : public QWidget{
	Q_OBJECT

	public:
		InfoLabelW(QWidget *parent = nullptr);
		InfoLabelW(const char *name, QWidget *parent = nullptr);


		void setName(const QString& name);
		void setInfo(const QString& info);


	private:
		QLabel *name_l;
		QLabel *info_l;
		QHBoxLayout *layout;
};


#endif // __INFO_LABEL_W_H__