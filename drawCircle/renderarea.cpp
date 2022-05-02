#include "renderarea.h"
#include <QPainter>
#include <QPainterPath>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    m_antialiased = false;
    m_transformed = false;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    m_nbCircle = 0;
    m_color = "green";
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 400);
}

void RenderArea::setPen(const QPen &pen)
{
    this->m_pen = pen;
    update();
}

void RenderArea::setColor(QString str)
{
    this->m_color = str;
    update();
}

void RenderArea::setBrush(const QBrush &brush)
{
    this->m_brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased)
{
    this->m_antialiased = antialiased;
    update();
}

void RenderArea::setTransformed(bool transformed)
{
    this->m_transformed = transformed;
    update();
}

void RenderArea::setCircle(int nb)
{
    m_nbCircle = nb;
    update();
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    int             countCircle;
    QRect           rect(10, 10, 40, 40);
    QPainterPath    path;
    QPainter        painter(this);

    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
    if (m_antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);
    countCircle = 0;
    if (m_nbCircle > 0) {
        for (int x = 0; x < width(); x += 100) {
            for (int y = 0; y < height(); y += 100) {
                countCircle++;
                painter.save();
                painter.translate(x, y);
                if (m_transformed) {
                    painter.translate(50, 50);
                    painter.rotate(60.0);
                    painter.scale(0.6, 0.9);
                    painter.translate(-50, -50);
                }
                if (m_color == "green") {
                    painter.setBrush(Qt::green);
                } else if (m_color == "red") {
                    painter.setBrush(Qt::red);
                } else if (m_color == "blue") {
                    painter.setBrush(Qt::blue);
                } else if (m_color == "cyan") {
                    painter.setBrush(Qt::cyan);
                } else if (m_color == "magenta") {
                    painter.setBrush(Qt::magenta);
                } else if (m_color == "yellow") {
                    painter.setBrush(Qt::yellow);
                } else if (m_color == "gray") {
                    painter.setBrush(Qt::gray);
                }
                painter.drawEllipse(rect);
                painter.restore();
                if (countCircle >= m_nbCircle)
                    break ;
            }
            if (countCircle >= m_nbCircle)
                break ;
        }
    }
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}
