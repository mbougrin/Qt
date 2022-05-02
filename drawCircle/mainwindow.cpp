#include "renderarea.h"
#include "mainwindow.h"
#include <QtWidgets>

const int IdRole = Qt::UserRole;

MainWindow::MainWindow()
{
    m_nbCircle = 0;
    p_renderArea = new RenderArea;

    p_addCirclePushButton = new QPushButton(tr("&Add Circle"));
    p_delCirclePushButton = new QPushButton(tr("&Del Circle"));

    p_penWidthSpinBox = new QSpinBox;
    p_penWidthSpinBox->setRange(0, 20);
    p_penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

    p_penWidthLabel = new QLabel(tr("Pen &Width:"));
    p_penWidthLabel->setBuddy(p_penWidthSpinBox);

    p_penStyleComboBox = new QComboBox;
    p_penStyleComboBox->addItem(tr("Solid"), static_cast<int>(Qt::SolidLine));
    p_penStyleComboBox->addItem(tr("Dash"), static_cast<int>(Qt::DashLine));
    p_penStyleComboBox->addItem(tr("Dot"), static_cast<int>(Qt::DotLine));
    p_penStyleComboBox->addItem(tr("Dash Dot"), static_cast<int>(Qt::DashDotLine));
    p_penStyleComboBox->addItem(tr("Dash Dot Dot"), static_cast<int>(Qt::DashDotDotLine));
    p_penStyleComboBox->addItem(tr("None"), static_cast<int>(Qt::NoPen));

    p_penStyleLabel = new QLabel(tr("&Pen Style:"));
    p_penStyleLabel->setBuddy(p_penStyleComboBox);

    p_colorComboBox = new QComboBox;
    p_colorComboBox->addItem(tr("green"), "green");
    p_colorComboBox->addItem(tr("red"), "red");
    p_colorComboBox->addItem(tr("blue"), "blue");
    p_colorComboBox->addItem(tr("cyan"), "cyan");
    p_colorComboBox->addItem(tr("magenta"), "magenta");
    p_colorComboBox->addItem(tr("yellow"), "yellow");
    p_colorComboBox->addItem(tr("gray"), "gray");

    p_colorLabel = new QLabel("Color:");

    p_otherOptionsLabel = new QLabel(tr("Options:"));

    p_antialiasingCheckBox = new QCheckBox(tr("&Antialiasing"));

    p_transformationsCheckBox = new QCheckBox(tr("&Transformations"));

    connect(p_addCirclePushButton, &QPushButton::released,
            this, &MainWindow::addCircle);
    connect(p_delCirclePushButton, &QPushButton::released,
            this, &MainWindow::delCircle);
    connect(p_penWidthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::penChanged);
    connect(p_penStyleComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::penChanged);
    connect(p_colorComboBox, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::colorChanged);
    connect(p_antialiasingCheckBox, &QAbstractButton::toggled,
            p_renderArea, &RenderArea::setAntialiased);
    connect(p_transformationsCheckBox, &QAbstractButton::toggled,
            p_renderArea, &RenderArea::setTransformed);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(p_renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(p_addCirclePushButton, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(p_delCirclePushButton, 2, 1);
    mainLayout->addWidget(p_penWidthLabel, 3, 0, Qt::AlignRight);
    mainLayout->addWidget(p_penWidthSpinBox, 3, 1);
    mainLayout->addWidget(p_penStyleLabel, 3, 2, Qt::AlignRight);
    mainLayout->addWidget(p_penStyleComboBox, 3, 3);
    mainLayout->addWidget(p_colorLabel, 2, 2, Qt::AlignRight);
    mainLayout->addWidget(p_colorComboBox, 2, 3);
    mainLayout->addWidget(p_otherOptionsLabel, 5, 0, Qt::AlignRight);
    mainLayout->addWidget(p_antialiasingCheckBox, 5, 1, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(p_transformationsCheckBox, 5, 2, 1, 2, Qt::AlignRight);
    setLayout(mainLayout);
    penChanged();
    colorChanged();
    p_antialiasingCheckBox->setChecked(true);
    setWindowTitle(tr("Draw Circle"));
}

void MainWindow::addCircle()
{
    if (m_nbCircle <= 19) {
        m_nbCircle++;
        p_renderArea->setCircle(m_nbCircle);
    }
}

void MainWindow::delCircle()
{
    if (m_nbCircle > 0) {
        m_nbCircle--;
        p_renderArea->setCircle(m_nbCircle);
    }
}

void MainWindow::colorChanged()
{
    p_renderArea->setColor(p_colorComboBox->itemData(p_colorComboBox->currentIndex(), IdRole).toString());
}

void MainWindow::penChanged()
{
    int width = p_penWidthSpinBox->value();
    Qt::PenStyle style = Qt::PenStyle(p_penStyleComboBox->itemData(
            p_penStyleComboBox->currentIndex(), IdRole).toInt());
    Qt::PenCapStyle cap = Qt::PenCapStyle(Qt::LinearGradientPattern);

    p_renderArea->setPen(QPen(Qt::blue, width, style, cap, Qt::MiterJoin));
}
