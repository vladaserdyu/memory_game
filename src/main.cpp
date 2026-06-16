#include "mainmenu.h"
#include "gameboard.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        MainMenu menu;
        GameBoard game;

        QObject::connect(&menu, &MainMenu::gameSelected, [&](int gridSize) {
            menu.hide();
            game.startNewGame(gridSize);
            game.show();
        });

        QObject::connect(&game, &GameBoard::backToMenu, [&]() {
            game.hide();
            menu.show();
        });

        menu.show();
        return a.exec();

    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Ошибка",
                              QString("Ошибка запуска игры:\n%1").arg(e.what()));
        return 1;
    }
}
