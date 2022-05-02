#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setColor(QString str);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    void setCircle(int nb);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_color;
    int     m_nbCircle;
    QPen    m_pen;
    QBrush  m_brush;
    bool    m_antialiased;
    bool    m_transformed;
    QPixmap m_pixmap;
};

#endif // RENDERAREA_H
