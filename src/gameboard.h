/**
 * @file gameboard.h
 * @brief Игровое поле и карточки
 * @date 2026
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QBitmap>
#include <QPainter>

/**
 * @class CardButton
 * @brief Кнопка-карточка для игры Memory
 *
 * Карточка может быть перевёрнута (показано изображение)
 * или скрыта (рубашка). При совпадении пары карточка
 * помечается как найденная и блокируется.
 */
class CardButton : public QPushButton
{
    Q_OBJECT

   public:
    /**
     * @brief Конструктор карточки
     * @param id Уникальный идентификатор
     * @param parent Родительский виджет
     */
    explicit CardButton(int id, QWidget* parent = nullptr);

    /**
     * @brief Переворачивает карточку
     * @param flipped true — показать картинку, false — рубашку
     */
    void flip(bool flipped);

    /**
     * @brief Проверяет, перевёрнута ли карточка
     * @return true если открыта
     */
    bool isFlipped() const { return m_flipped; }

    /**
     * @brief Проверяет, найдена ли пара
     * @return true если пара найдена
     */
    bool isMatched() const { return m_matched; }

    /**
     * @brief Устанавливает статус найденной пары
     * @param matched Новый статус
     */
    void setMatched(bool matched);

    /**
     * @brief Устанавливает значение карточки
     * @param val Индекс изображения
     */
    void setValue(int val) { m_value = val; }

    /**
     * @brief Возвращает значение карточки
     * @return Индекс изображения
     */
    int getValue() const { return m_value; }

    /**
     * @brief Сбрасывает состояние карточки для новой игры
     */
    void reset();

   private:
    /**
     * @brief Загружает изображение для карточки
     * @param val Индекс изображения (0-31)
     * @return Загруженное изображение
     */
    QPixmap getImageForValue(int val);

    int m_id;          ///< Уникальный идентификатор
    int m_value;       ///< Индекс изображения
    bool m_flipped;    ///< Флаг перевёрнутости
    bool m_matched;    ///< Флаг найденной пары
};

/**
 * @brief Логические функции для игры
 */
int calcMinMoves(int gridSize);
int calcMaxMoves(int gridSize);
bool isMatch(int val1, int val2);
std::vector<int> generateCardValues(int gridSize);

/**
 * @class GameBoard
 * @brief Главный класс игрового поля
 *
 * Управляет игровым процессом: создаёт карточки,
 * проверяет совпадения, считает ходы и время,
 * сохраняет рекорды.
 */
class GameBoard : public QWidget
{
    Q_OBJECT

   public:
    /**
     * @brief Конструктор игрового поля
     * @param parent Родительский виджет
     */
    explicit GameBoard(QWidget *parent = nullptr);

    /**
     * @brief Начинает новую игру
     * @param gridSize Размер сетки (2, 4, 6, 8)
     */
    void startNewGame(int gridSize);

    /**
     * @brief Показывает 10 лучших результатов
     * @param parent Родительское окно
     */
    static void showTopTen(QWidget* parent = nullptr);

   signals:
    /** @brief Сигнал возврата в главное меню */
    void backToMenu();

   private slots:
    /** @brief Обработчик нажатия на карточку */
    void onCardClicked(int index);
    /** @brief Обработчик нажатия на кнопку "Назад" */
    void onBackClicked();
    /** @brief Обновление времени (таймер) */
    void updateTime();

   private:
    void stopGame();
    void initGame(int size);
    void createCards(int size);
    void shuffleCards();
    void checkMatch();
    void hideCards();
    void updateUI();
    void endGame();
    void saveRecord();

    QVector<CardButton*> m_cards;   ///< Все карточки
    QVector<int> m_cardValues;       ///< Значения карточек
    QLabel* m_timeLabel;             ///< Метка времени
    QLabel* m_movesLabel;            ///< Метка ходов
    QLabel* m_pairsLabel;            ///< Метка пар
    QPushButton* m_backBtn;          ///< Кнопка "В меню"
    QTimer* m_timer;                 ///< Таймер
    QWidget* m_cardsContainer;       ///< Контейнер для карточек
    int m_gridSize;                  ///< Размер сетки
    int m_movesCount;                ///< Счётчик ходов
    int m_pairsFound;                ///< Найдено пар
    int m_seconds;                   ///< Время в секундах
    int m_firstCard;                 ///< Индекс первой открытой
    int m_secondCard;                ///< Индекс второй открытой
    bool m_isWaiting;                ///< Флаг ожидания
    bool m_isActive;                 ///< Флаг активности
};

#endif
