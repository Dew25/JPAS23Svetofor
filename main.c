/*
При запуске загорается красный прямо и зеленый сбоку.
Цыкл:
    5 секунд
    Везде включается желтый
    2 секунды.
    если прямо горит красный, то
        загорается зеленый прямо и красный сбоку
    иначе
        загорается красный прямо и зеленый сбоку

*/
#include <stdio.h>
#ifdef _WIN32
#include <windows.h> // Для функции Sleep() и настройки кодировки
#else
#include <unistd.h> // Для функции sleep() на POSIX-системах
#endif
#include <locale.h> // Для настройки локали
// Определение битовых масок
#define RED_MAIN   (1 << 0) // Красный свет для главной дороги (бит 1)
#define RED_SIDE   (1 << 3) // Красный свет для боковой дороги (бит 4)
#define YELLOW     ((1 << 1) | (1 << 4)) // Жёлтый свет (биты 2 и 5)
#define GREEN_MAIN (1 << 2) // Зелёный свет для главной дороги (бит 3)
#define GREEN_SIDE (1 << 5) // Зелёный свет для боковой дороги (бит 6)
// Функция для отображения состояния битового контейнера
void displayContainer(unsigned char container) {
    printf("Текущее состояние контейнера: 0b");
    for (int i = 5; i >= 0; i--) {
        printf("%d", (container >> i) & 1);
    }
    printf("\n");

    printf("Состояние светофора:\n");
    if (container & RED_MAIN) {
        printf(" - Красный свет на главной дороге\n");
    }
    if (container & YELLOW) {
        printf(" - Жёлтый свет\n");
    }
    if (container & GREEN_MAIN) {
        printf(" - Зелёный свет на главной дороге\n");
    }
    if (container & GREEN_SIDE) {
        printf(" - Зелёный свет на боковой дороге\n");
    }
    if (container & RED_SIDE) {
        printf(" - Красный свет на боковой дороге\n");
    }
    printf("-----------------------------\n");
}

// Кросс-платформенная функция для паузы
void crossPlatformSleep(int seconds) {
    #ifdef _WIN32
        Sleep(seconds * 1000); // В Windows Sleep принимает миллисекунды
    #else
        sleep(seconds); // В POSIX sleep принимает секунды
    #endif
}
int main() {
    #ifdef _WIN32
        // Устанавливаем кодировку UTF-8 для Windows Console
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif
    // Устанавливаем локаль для корректного отображения текста
    if (!setlocale(LC_ALL, "ru_RU.UTF-8")) {
        fprintf(stderr, "Не удалось установить локаль. Проверьте настройки системы.\n");
        return 1;
    }
    unsigned char container = 0; // Изначально все биты сброшены
    int repeat = 1;
    int count = 0;
    while (repeat) {

        // 1. Красный на главной дороге, зелёный на боковой дороге
        container = RED_MAIN | GREEN_SIDE;
        displayContainer(container);
        crossPlatformSleep(5);

        // 2. Жёлтый свет на обеих дорогах (переход)
        container = YELLOW;
        displayContainer(container);
        crossPlatformSleep(2);

        // 3. Зелёный на главной дороге, красный на боковой дороге
        container = GREEN_MAIN | RED_SIDE;
        displayContainer(container);
        crossPlatformSleep(5);

        // 4. Жёлтый свет на обеих дорогах (переход)
        container = YELLOW;
        displayContainer(container);
        crossPlatformSleep(2);
        count++;
        if (count > 5) repeat = 0;
    }

    return 0;
}