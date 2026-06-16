#include "gameboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <algorithm>
#include <QDebug>
#include <QRandomGenerator>
#include <stdexcept>

int calcMinMoves(int gridSize) {
    return (gridSize * gridSize) / 2;
}

int calcMaxMoves(int gridSize) {
    return gridSize * gridSize;
}

bool isMatch(int val1, int val2) {
    return val1 == val2;
}

std::vector<int> generateCardValues(int gridSize) {
    int total = gridSize * gridSize;
    int pairs = total / 2;
    std::vector<int> values;
    for (int i = 0; i < pairs; ++i) {
        values.push_back(i);
        values.push_back(i);
    }
    return values;
}

CardButton::CardButton(int id, QWidget* parent)
    : QPushButton(parent), m_id(id), m_value(-1), m_flipped(false), m_matched(false)
{}

QPixmap CardButton::getImageForValue(int val)
{
    try {
        int w = width();
        int h = height();

        QString imagePath = "images/" + QString::number(val + 1) + ".jpg";
        QPixmap pixmap;

        if (pixmap.load(imagePath)) {
            return pixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        throw std::runtime_error("Не удалось загрузить картинку: " + imagePath.toStdString());

    } catch (const std::exception& e) {
        qDebug() << "Ошибка при загрузке картинки:" << e.what();

        int w = width();
        int h = height();

        QPixmap fallback(w, h);
        fallback.fill(QColor("#2c3e50"));

        QPainter painter(&fallback);
        painter.setPen(Qt::black);
        int fontSize = w / 3;
        if (fontSize < 8) fontSize = 8;
        painter.setFont(QFont("Arial", fontSize, QFont::Bold));
        painter.drawText(fallback.rect(), Qt::AlignCenter, QString::number(val + 1));

        return fallback;
    }
}

void CardButton::flip(bool flipped)
{
    if (m_matched) return;
    m_flipped = flipped;
    if (flipped && m_value >= 0) {
        setIcon(getImageForValue(m_value));
        setIconSize(size());
    } else {
        setIcon(QIcon());
    }
}

void CardButton::setMatched(bool matched)
{
    m_matched = matched;
    if (matched) {
        setEnabled(false);
        setStyleSheet("QPushButton { background-color: #2ecc71; border: none; border-radius: 10px; }");
    }
}

void CardButton::reset()
{
    m_flipped = false;
    m_matched = false;
    setEnabled(true);
    setIcon(QIcon());
    setStyleSheet("QPushButton { background-color: #2c3e50; border: none; border-radius: 10px; }"
        "QPushButton:hover { background-color: #34495e; }");
}

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
      , m_timer(nullptr)
      , m_cardsContainer(nullptr)
      , m_gridSize(0)
      , m_movesCount(0)
      , m_pairsFound(0)
      , m_seconds(0)
      , m_firstCard(-1)
      , m_secondCard(-1)
      , m_isWaiting(false)
      , m_isActive(true)
{
    setWindowTitle("Memory Game - Игра");
    setStyleSheet("QWidget { background: #FFB6C1; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QString labelStyle = "font-size: 16px; font-weight: bold; color: #2c3e50; background: rgba(255,255,255,0.5); padding: 8px; border-radius: 8px;";

    m_timeLabel = new QLabel("Время: 0 сек");
    m_movesLabel = new QLabel("Ходы: 0");
    m_pairsLabel = new QLabel("Пары: 0/0");
    m_backBtn = new QPushButton("← В меню");

    m_timeLabel->setStyleSheet(labelStyle);
    m_movesLabel->setStyleSheet(labelStyle);
    m_pairsLabel->setStyleSheet(labelStyle);
    m_backBtn->setStyleSheet("font-size: 14px; padding: 8px; background: #e74c3c; color: white; border-radius: 8px;");

    topLayout->addWidget(m_timeLabel);
    topLayout->addWidget(m_movesLabel);
    topLayout->addWidget(m_pairsLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_backBtn);
    mainLayout->addLayout(topLayout);

    m_cardsContainer = new QWidget();
    mainLayout->addWidget(m_cardsContainer);

    connect(m_backBtn, &QPushButton::clicked, this, &GameBoard::onBackClicked);
}

void GameBoard::stopGame()
{
    m_isActive = false;
    if (m_timer) m_timer->stop();
}

void GameBoard::startNewGame(int gridSize)
{
    m_gridSize = gridSize;

    switch (gridSize) {
        case 2: setFixedSize(500, 450); break;
        case 4: setFixedSize(550, 600); break;
        case 6: setFixedSize(700, 750); break;
        case 8: setFixedSize(850, 900); break;
        default: setFixedSize(550, 600);
    }

    initGame(gridSize);

    if (m_timer) delete m_timer;
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GameBoard::updateTime);
    m_timer->start(1000);
}

void GameBoard::initGame(int size)
{
    m_gridSize = size;
    m_movesCount = 0;
    m_pairsFound = 0;
    m_firstCard = -1;
    m_secondCard = -1;
    m_isWaiting = false;
    m_isActive = true;
    m_seconds = 0;

    m_timeLabel->setText("Время: 0 сек");
    updateUI();
    createCards(size);
    shuffleCards();
}

void GameBoard::createCards(int size)
{
    try {
        if (size < 2 || size > 8) {
            throw std::runtime_error("Некорректный размер поля");
        }

        for (CardButton* card : m_cards) {
            delete card;
        }
        m_cards.clear();
        m_cardValues.clear();

        int cardSize;
        int spacing;
        int margins;

        switch (size) {
            case 2:
                cardSize = 120;
                spacing = 20;
                margins = 40;
                break;
            case 4:
                cardSize = 90;
                spacing = 10;
                margins = 20;
                break;
            case 6:
                cardSize = 70;
                spacing = 8;
                margins = 15;
                break;
            case 8:
                cardSize = 55;
                spacing = 8;
                margins = 15;
                break;
            default:
                cardSize = 90;
                spacing = 10;
                margins = 20;
        }

        int total = size * size;
        int pairs = total / 2;
        for (int i = 0; i < pairs; ++i) {
            m_cardValues.append(i);
            m_cardValues.append(i);
        }

        delete m_cardsContainer->layout();

        QGridLayout* gridLayout = new QGridLayout(m_cardsContainer);
        gridLayout->setSpacing(spacing);
        gridLayout->setContentsMargins(margins, margins, margins, margins);

        for (int i = 0; i < total; ++i) {
            CardButton* card = new CardButton(i);
            card->setFixedSize(cardSize, cardSize);
            connect(card, &CardButton::clicked, this, [this, i]() { onCardClicked(i); });
            m_cards.append(card);
            gridLayout->addWidget(card, i / size, i % size);
        }
    } catch (const std::exception& e) {
        qDebug() << "Ошибка при создании карточек:" << e.what();
        QMessageBox::warning(this, "Ошибка",
                             QString("Не удалось создать игровое поле:\n%1").arg(e.what()));
        emit backToMenu();
    }
}

void GameBoard::shuffleCards()
{
    try {
        for (int i = m_cardValues.size() - 1; i > 0; --i) {
            int j = QRandomGenerator::global()->bounded(i + 1);
            m_cardValues.swapItemsAt(i, j);
        }

        for (int i = 0; i < m_cards.size(); ++i) {
            m_cards[i]->setValue(m_cardValues[i]);
            m_cards[i]->reset();
        }
    } catch (const std::exception& e) {
        qDebug() << "Ошибка при перемешивании карточек:" << e.what();
        emit backToMenu();
    }
}

void GameBoard::onCardClicked(int index)
{
    if (!m_isActive || m_isWaiting) return;

    CardButton* card = m_cards[index];
    if (card->isMatched() || card->isFlipped()) return;

    if (m_firstCard == -1) {
        m_firstCard = index;
        card->flip(true);
    } else if (m_secondCard == -1 && m_firstCard != index) {
        m_secondCard = index;
        card->flip(true);
        m_movesCount++;
        updateUI();
        checkMatch();
    }
}

void GameBoard::checkMatch()
{
    bool match = isMatch(m_cards[m_firstCard]->getValue(), m_cards[m_secondCard]->getValue());

    if (match) {
        m_cards[m_firstCard]->setMatched(true);
        m_cards[m_secondCard]->setMatched(true);
        m_pairsFound++;
        updateUI();

        m_firstCard = -1;
        m_secondCard = -1;

        int totalPairs = calcMinMoves(m_gridSize);
        if (m_pairsFound == totalPairs) {
            endGame();
        }
    } else {
        m_isWaiting = true;
        QTimer::singleShot(800, this, &GameBoard::hideCards);
    }
}

void GameBoard::hideCards()
{
    if (m_firstCard != -1 && m_secondCard != -1) {
        m_cards[m_firstCard]->flip(false);
        m_cards[m_secondCard]->flip(false);
        m_firstCard = -1;
        m_secondCard = -1;
        m_isWaiting = false;
    }
}

void GameBoard::updateUI()
{
    int totalPairs = calcMinMoves(m_gridSize);
    m_movesLabel->setText(QString("Ходы: %1").arg(m_movesCount));
    m_pairsLabel->setText(QString("Пары: %1/%2").arg(m_pairsFound).arg(totalPairs));
}

void GameBoard::updateTime()
{
    if (m_isActive) {
        m_seconds++;
        m_timeLabel->setText(QString("Время: %1 сек").arg(m_seconds));
    }
}

void GameBoard::saveRecord()
{
    try {
        QFile file("records.txt");
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            throw std::runtime_error("Не удалось открыть файл records.txt");
        }
        QTextStream out(&file);
        out << QString("%1x%2|%3|%4|%5\n")
                   .arg(m_gridSize).arg(m_gridSize).arg(m_movesCount).arg(m_seconds)
                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        file.close();
    } catch (const std::exception& e) {
        qDebug() << "Ошибка при сохранении рекорда:" << e.what();
    }
}

void GameBoard::endGame()
{
    stopGame();

    int totalPairs = calcMinMoves(m_gridSize);
    int minMoves = calcMinMoves(m_gridSize);
    int maxMoves = calcMaxMoves(m_gridSize);

    QMessageBox::information(this, "Победа!",
                             QString("Вы выиграли!\nХодов: %1\nВремя: %2 сек\n"
                                     "Теоретический минимум: %3\nТеоретический максимум: %4")
                                 .arg(m_movesCount).arg(m_seconds).arg(minMoves).arg(maxMoves));

    saveRecord();

    emit backToMenu();
}

void GameBoard::onBackClicked()
{
    stopGame();
    emit backToMenu();
}

void GameBoard::showTopTen(QWidget* parent)
{
    QVector<QString> records;
    QFile file("records.txt");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.trimmed().isEmpty()) {
                records.append(line);
            }
        }
        file.close();
    }

    if (records.isEmpty()) {
        QMessageBox::information(parent, "Топ-10", "Нет сохранённых результатов.");
        return;
    }

    QMap<int, QStringList> grouped;
    for (const QString& record : records) {
        int size = record.split("|")[0].split("x")[0].toInt();
        grouped[size].append(record);
    }

    QString message = "🏆 ЛУЧШИЕ 10 РЕЗУЛЬТАТОВ 🏆\n\n";

    QList<int> sizes = grouped.keys();
    std::sort(sizes.begin(), sizes.end());

    for (int size : sizes) {
        QStringList& group = grouped[size];

        std::sort(group.begin(), group.end(), [](const QString& a, const QString& b) {
            int movesA = a.split("|")[1].toInt();
            int movesB = b.split("|")[1].toInt();
            if (movesA != movesB) return movesA < movesB;
            return a.split("|")[2].toInt() < b.split("|")[2].toInt();
        });

        if (group.size() > 10) group.resize(10);

        message += QString("═══ %1x%2 ═══\n").arg(size).arg(size);
        for (int i = 0; i < group.size(); ++i) {
            QStringList parts = group[i].split("|");
            message += QString("%1. Ходов: %2 | Время: %3 сек\n")
                           .arg(i + 1).arg(parts[1]).arg(parts[2]);
        }
        message += "\n";
    }

    QMessageBox::information(parent, "Топ-10", message);
}
