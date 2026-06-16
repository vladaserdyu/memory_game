
```cpp
/**
 * @file mainmenu.h
 * @brief Главное меню игры
 * @author Студент
 * @version 1.0
 * @date 2026
 */

#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>

    /**
     * @class MainMenu
     * @brief Класс главного меню
     *      * Отображает заголовок, поле ввода размера поля,
     * кнопку "Старт" и кнопку "Топ-10".
     */
    class MainMenu : public QWidget
{
    Q_OBJECT

   public:
    /**
     * @brief Конструктор главного меню
     * @param parent Родительский виджет
     */
    explicit MainMenu(QWidget *parent = nullptr);

   signals:
    /**
     * @brief Сигнал выбора уровня игры
     * @param gridSize Размер поля (2, 4, 6, 8)
     */
    void gameSelected(int gridSize);

   private slots:
    /** @brief Обработчик нажатия на кнопку "Старт" */
    void onStartClicked();

   private:
    QLineEdit* m_sizeInput;   ///< Поле для ввода размера
    QPushButton* m_startBtn;  ///< Кнопка "Старт"
    QLabel* m_titleLabel;     ///< Заголовок "MEMORY GAME"
};

#endif
