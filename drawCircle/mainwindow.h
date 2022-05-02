#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;
class QPushButton;
QT_END_NAMESPACE
class RenderArea;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void colorChanged();
    void penChanged();
    void addCircle();
    void delCircle();

private:
    int         m_nbCircle;
    RenderArea  *p_renderArea;
    QPushButton *p_addCirclePushButton;
    QPushButton *p_delCirclePushButton;
    QLabel      *p_penWidthLabel;
    QLabel      *p_penStyleLabel;
    QLabel      *p_colorLabel;
    QLabel      *p_otherOptionsLabel;
    QSpinBox    *p_penWidthSpinBox;
    QComboBox   *p_penStyleComboBox;
    QComboBox   *p_colorComboBox;
    QCheckBox   *p_antialiasingCheckBox;
    QCheckBox   *p_transformationsCheckBox;
};

#endif // MAINWINDOW_H
