#include "mainmenu.h"
#include "gameboard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <stdexcept>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Memory Game");
    setFixedSize(500, 650);
    setStyleSheet("QWidget { background: #FFB6C1; }");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(50, 80, 50, 80);

    m_titleLabel = new QLabel("MEMORY GAME", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-size: 32px; font-weight: bold; font-family: 'Arial';"
        "color: #ecf0f1; background: transparent; padding: 20px;"
        );

    QString buttonStyle =
        "QPushButton { font-size: 20px; font-weight: bold; padding: 15px; "
        "border-radius: 12px; color: white; background: rgba(0,0,0,0.6); border: none; }"
        "QPushButton:hover { background: rgba(0,0,0,0.8); transform: scale(1.05); }";

    QString inputStyle =
        "QLineEdit {"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   padding: 15px;"
        "   border-radius: 12px;"
        "   background: rgba(255,255,255,0.8);"
        "   color: #2c3e50;"
        "   border: 2px solid #3498db;"
        "   text-align: center;"
        "}";

    QLabel* inputLabel = new QLabel("Введите размер поля (2-8):", this);
    inputLabel->setAlignment(Qt::AlignCenter);
    inputLabel->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #ecf0f1; background: transparent;"
        );

    m_sizeInput = new QLineEdit(this);
    m_sizeInput->setPlaceholderText("Например: 4");
    m_sizeInput->setStyleSheet(inputStyle);
    m_sizeInput->setMaxLength(1);
    m_sizeInput->setText("4");

    m_startBtn = new QPushButton("Старт", this);
    m_startBtn->setStyleSheet(buttonStyle);
    m_startBtn->setCursor(Qt::PointingHandCursor);
    connect(m_startBtn, &QPushButton::clicked, this, &MainMenu::onStartClicked);

    QPushButton* topBtn = new QPushButton("🏆 Топ-10", this);
    topBtn->setStyleSheet(buttonStyle);
    topBtn->setCursor(Qt::PointingHandCursor);
    connect(topBtn, &QPushButton::clicked, [this]() {
        GameBoard::showTopTen(this);
    });

    layout->addWidget(m_titleLabel);
    layout->addStretch();
    layout->addWidget(inputLabel);
    layout->addWidget(m_sizeInput);
    layout->addWidget(m_startBtn);
    layout->addStretch();
    layout->addWidget(topBtn);
    layout->addStretch();
}

void MainMenu::onStartClicked()
{
    try {
        QString text = m_sizeInput->text().trimmed();

        if (text.isEmpty()) {
            throw std::runtime_error("Пожалуйста, введите размер поля.");
        }

        bool ok;
        int size = text.toInt(&ok);

        if (!ok) {
            throw std::runtime_error("Введите число от 2 до 8.");
        }

        if (size < 2 || size > 8) {
            throw std::runtime_error(
                "Размер поля должен быть от 2 до 8.\n"
                "Вы ввели: " + std::to_string(size) + "\n"
                "Допустимые значения: 2, 4, 6, 8."
                );
        }

        if (size % 2 != 0) {
            throw std::runtime_error(
                "Размер поля должен быть чётным числом.\n"
                "Вы ввели: " + std::to_string(size) + "\n"
                "Допустимые значения: 2, 4, 6, 8."
                );
        }

        emit gameSelected(size);

    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Ошибка", e.what());
    }
}
