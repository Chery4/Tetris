#include <SFML/Graphics.hpp>
#include <time.h> 
#include <iostream>
#include <string>
#include <sstream> 
#include <fstream>
#include <map>
#include <algorithm>

using namespace sf;
using namespace std;

#define HEIGHT 20 //высота поля.
#define WIGHT 10 //ширина поля.

int field[HEIGHT][WIGHT] = { 0 }; //задаем пустое поле.

int scores[3] = { 0, 0, 0 }; // массив для записи максимальных очков трёх игр (рекорды).

void save_leaders_to_file() { //сохраняем рекорды в файл.
    ofstream file("data.txt");// открываем файл.
    if (file.is_open()) { // пока файл открыт, записываем значения.
        for (int i = 0; i < 3; i++) {
            file << scores[i] << endl;
        }
        file.close();// закрываем файл.
    }
}

void load_from_file() { // считываем из файла рекордные очки.
    ifstream file("data.txt");
    if (file.is_open()) {
        int scoreer;
        string line;
        for (int i = 0; i < 3; i++) {
            getline(file, line);
            scoreer = stoi(line);
            scores[i] = scoreer;

        }
        file.close();
    }
}

void sort_leaders(int current_score) { // сортируем рекорды по убыванию и записываем в переменные.
    if (current_score == scores[0] || current_score == scores[1] || current_score == scores[2])
        return;
    if (current_score > scores[0]) {
        scores[2] = scores[1];
        scores[1] = scores[0];
        scores[0] = current_score;
    }
    else if (current_score > scores[1]) {
        scores[2] = scores[1];
        scores[1] = current_score;
    }
    else if (current_score > scores[2]) {
        scores[2] = current_score;
    }
}

struct Point // структура точки с координатами.
{
    int x, y = -1;
}a[4], b[4];

int blok[7][4] = //массив фигур для тетриса 7 фигур по 4 квадрата (тетрамино).
{   1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

bool border() // определение для границ фигуры и занятости ячеек другими фигурами
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= WIGHT || a[i].y >= HEIGHT)
            return 0;
        else if (field[a[i].y][a[i].x])// если занята ячейка.
            return 0;
    return 1;
}

bool gameover() // функция проигрыша
{
    for (int i = 0; i < WIGHT; ++i)// условие - если фигура достигла предела в поле
    {
        if (field[1][i] != 0) return true;
    }
    return false;
}

int main()
{
    int score = 0;
    srand(time(0));//подключение рандома.

    //Окно для 2D-рисования с разрешением 340 на 680 пикселей.
    RenderWindow window(VideoMode(340, 680), "tetris");

    Texture block_texture;// создание текстуры.
    block_texture.loadFromFile("blok.png");
    // создание спрайта.
    Sprite tiles(block_texture);

    int dx = 0;// горизонтальное смещение фигур.
    int blockNum = 1;// номер квадратика фигулы
    bool rotate = false;// флаг вращения.
    bool start_game = true; // флаг начала игры.
    float timer = 0;//
    float delay = 0.3;// период времени между смещениями тетрамино вниз (в секундах).
    Clock clock;// время, прошедшее с начала отсчета.

    while (window.isOpen())
    {
        if (gameover()) // конец игры - выходим из цикла.
        {
            break;
        }
        //конвертация в секунды времени с начала отсчета
        float time = clock.getElapsedTime().asSeconds();

        clock.restart(); // обнуляем счетчик времени.
        timer += time;
        // обрабатываем события в цикле
        Event event;//класс
        while (window.pollEvent(event))// есть ли какое-либо событие.
        {
            if (event.type == Event::Closed)// если пользователь закрыл окно.
                window.close();//закрываем игру.

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Up) // если нажата клавиша "ввех", поворачиваем тетрамино.
                    rotate = true;

                else if (event.key.code == Keyboard::Right) //если нажата клавиша "вправо", двигаем тетрамино вправо.
                    dx = 1;

                else if (event.key.code == Keyboard::Left)//если нажата клавиша "влево", двигаем тетрамино влево.
                    dx = -1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))//если нажата книпка "вниз" -ускорение.
        {
            delay = 0.05;
        }
        //смещение квадратиков фигуры в нужном направлении по горизонтали.
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];//запоминаем изначальное расположение фигурки
            a[i].x += dx;// смещаем
        }

        if (!border())// если не соблюдаются границы
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];//возвращаемся обратно, изначальные координаты
        }

        if (rotate)// если поворачивается тетрамино.
        {
            Point p = a[1];// центр вращения
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;//текущая координата- координата точки вращения.
                int y = a[i].x - p.x;

                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }

            if (!border()) // проверка на соблюдение границ.
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        if (timer > delay) // период обновление экрана прошёл.
        {
            for (int i = 0; i < 4; i++)// смещение на одну позицию вниз.
            {
                b[i] = a[i];
                a[i].y += 1;// смещение по у на 1 вниз.
            }

            if (!border() || start_game)
            {
                start_game = false;
                for (int i = 0; i < 4; i++)
                    //окрашивание занятых ячеек текстурой.
                    field[b[i].y][b[i].x] = blockNum;
                blockNum = 1 + rand() % 7;// придание текстуры фигуре.
                int n = rand() % 7;//генерирование случайного типа фигуры.

                for (int i = 0; i < 4; i++)// задаем координаты для каждого квадрата тетрамино.
                {
                    a[i].x = blok[n][i] % 2;
                    a[i].y = blok[n][i] / 2;
                }
            }
            timer = 0;//обнуление таймера.
        }


        //удаление заполненных строк.
        int k = HEIGHT - 1;//нижняя строка (координата). 
        for (int i = HEIGHT - 1; i > 0; i--) //проверка строк снизу вверх.
        {
            int count = 0;//счетчик занятых ячеек.
            for (int j = 0; j < WIGHT; j++)
            {
                if (field[i][j])// если ячейка занята.
                    count++;
                field[k][j] = field[i][j];//замена заполненной строки.
            }
            if (count < WIGHT) {//проверка каждой верхней строки.
                k--;
            }
            else {
                score++;// начисление очков игроку.
            }

        }

        // возвращаем прежние параметры переменных.
        dx = 0;
        rotate = false;
        delay = 0.3;

        window.clear(Color::Black); // задаем цвет поля.

        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIGHT; j++)
            {
                if (field[i][j] == 0)
                    continue;
                tiles.setTextureRect(IntRect(field[i][j] * 34, 0, 34, 34));
                tiles.setPosition(j * 34, i * 34);//позиция каждой ячейки
                window.draw(tiles);//отрисовка фигуры
            }

        for (int i = 0; i < 4; i++)// отрисовка тетрамино.
        {
            tiles.setTextureRect(IntRect(blockNum * 34, 0, 34, 34));
            tiles.setPosition(a[i].x * 34, a[i].y * 34);//установка позиции.
            window.draw(tiles);
        }
        window.display();//отображение окна.
    }
    

    Font font;
    font.loadFromFile("consola.ttf");

    Text message_game_over(L"GameOver!", font);// сообщение об окончании игры.
    message_game_over.setPosition(80, 100);// позиция сообщения.
    message_game_over.setFillColor(Color::Red);// цвет сообщения.
    message_game_over.setOutlineThickness(1.0f);

    ostringstream ScoreString;
    ScoreString << score;
    Text message_score(L"Score: " + ScoreString.str(), font);// набранные очки.
    message_score.setPosition(85, 140);
    message_score.setFillColor(Color::Red);
    message_score.setOutlineThickness(1.0f);

    load_from_file(); // запись из файла рекордов.
    sort_leaders(score);// сортировка очков лидеров с учетом набранных баллов.
    save_leaders_to_file(); // сохранение макцимальных очкоа в файл.

    ostringstream ScoreString1;
    ScoreString1 << scores[0];
    ostringstream ScoreString2;
    ScoreString2 << scores[1];
    ostringstream ScoreString3;
    ScoreString3 << scores[2];
    Text leaders(L"Leaders:\n   " + ScoreString1.str() + "\n   " + ScoreString2.str() + "\n   " + ScoreString3.str(), font);// таблица лидеров.
    leaders.setPosition(90, 180);
    leaders.setFillColor(Color::White);
    leaders.setOutlineThickness(1.0f);

    window.draw(message_score);// вывод набранных очков.
    window.draw(message_game_over); // вывод сообщениz о конце игры.
    window.draw(leaders);// вывод рейтингa из 3 максимальных баллов.
    window.display();

    //окно закрывается при нажатии на клавишу Esc.
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
                event.key.code == Keyboard::Escape)
                window.close();
        }
    }
    
    return 0;
}
