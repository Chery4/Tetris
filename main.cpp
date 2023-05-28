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

#define HEIGHT 20 //������ ����.
#define WIGHT 10 //������ ����.

int field[HEIGHT][WIGHT] = { 0 }; //������ ������ ����.

int scores[3] = { 0, 0, 0 }; // ������ ��� ������ ������������ ����� ��� ��� (�������).

void save_leaders_to_file() { //��������� ������� � ����.
    ofstream file("data.txt");// ��������� ����.
    if (file.is_open()) { // ���� ���� ������, ���������� ��������.
        for (int i = 0; i < 3; i++) {
            file << scores[i] << endl;
        }
        file.close();// ��������� ����.
    }
}

void load_from_file() { // ��������� �� ����� ��������� ����.
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

void sort_leaders(int current_score) { // ��������� ������� �� �������� � ���������� � ����������.
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

struct Point // ��������� ����� � ������������.
{
    int x, y = -1;
}a[4], b[4];

int blok[7][4] = //������ ����� ��� ������� 7 ����� �� 4 �������� (���������).
{   1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

bool border() // ����������� ��� ������ ������ � ��������� ����� ������� ��������
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= WIGHT || a[i].y >= HEIGHT)
            return 0;
        else if (field[a[i].y][a[i].x])// ���� ������ ������.
            return 0;
    return 1;
}

bool gameover() // ������� ���������
{
    for (int i = 0; i < WIGHT; ++i)// ������� - ���� ������ �������� ������� � ����
    {
        if (field[1][i] != 0) return true;
    }
    return false;
}

int main()
{
    int score = 0;
    srand(time(0));//����������� �������.

    //���� ��� 2D-��������� � ����������� 340 �� 680 ��������.
    RenderWindow window(VideoMode(340, 680), "tetris");

    Texture block_texture;// �������� ��������.
    block_texture.loadFromFile("blok.png");
    // �������� �������.
    Sprite tiles(block_texture);

    int dx = 0;// �������������� �������� �����.
    int blockNum = 1;// ����� ���������� ������
    bool rotate = false;// ���� ��������.
    bool start_game = true; // ���� ������ ����.
    float timer = 0;//
    float delay = 0.3;// ������ ������� ����� ���������� ��������� ���� (� ��������).
    Clock clock;// �����, ��������� � ������ �������.

    while (window.isOpen())
    {
        if (gameover()) // ����� ���� - ������� �� �����.
        {
            break;
        }
        //����������� � ������� ������� � ������ �������
        float time = clock.getElapsedTime().asSeconds();

        clock.restart(); // �������� ������� �������.
        timer += time;
        // ������������ ������� � �����
        Event event;//�����
        while (window.pollEvent(event))// ���� �� �����-���� �������.
        {
            if (event.type == Event::Closed)// ���� ������������ ������ ����.
                window.close();//��������� ����.

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Up) // ���� ������ ������� "����", ������������ ���������.
                    rotate = true;

                else if (event.key.code == Keyboard::Right) //���� ������ ������� "������", ������� ��������� ������.
                    dx = 1;

                else if (event.key.code == Keyboard::Left)//���� ������ ������� "�����", ������� ��������� �����.
                    dx = -1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))//���� ������ ������ "����" -���������.
        {
            delay = 0.05;
        }
        //�������� ����������� ������ � ������ ����������� �� �����������.
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];//���������� ����������� ������������ �������
            a[i].x += dx;// �������
        }

        if (!border())// ���� �� ����������� �������
        {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];//������������ �������, ����������� ����������
        }

        if (rotate)// ���� �������������� ���������.
        {
            Point p = a[1];// ����� ��������
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;//������� ����������- ���������� ����� ��������.
                int y = a[i].x - p.x;

                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }

            if (!border()) // �������� �� ���������� ������.
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }
        }

        if (timer > delay) // ������ ���������� ������ ������.
        {
            for (int i = 0; i < 4; i++)// �������� �� ���� ������� ����.
            {
                b[i] = a[i];
                a[i].y += 1;// �������� �� � �� 1 ����.
            }

            if (!border() || start_game)
            {
                start_game = false;
                for (int i = 0; i < 4; i++)
                    //����������� ������� ����� ���������.
                    field[b[i].y][b[i].x] = blockNum;
                blockNum = 1 + rand() % 7;// �������� �������� ������.
                int n = rand() % 7;//������������� ���������� ���� ������.

                for (int i = 0; i < 4; i++)// ������ ���������� ��� ������� �������� ���������.
                {
                    a[i].x = blok[n][i] % 2;
                    a[i].y = blok[n][i] / 2;
                }
            }
            timer = 0;//��������� �������.
        }


        //�������� ����������� �����.
        int k = HEIGHT - 1;//������ ������ (����������). 
        for (int i = HEIGHT - 1; i > 0; i--) //�������� ����� ����� �����.
        {
            int count = 0;//������� ������� �����.
            for (int j = 0; j < WIGHT; j++)
            {
                if (field[i][j])// ���� ������ ������.
                    count++;
                field[k][j] = field[i][j];//������ ����������� ������.
            }
            if (count < WIGHT) {//�������� ������ ������� ������.
                k--;
            }
            else {
                score++;// ���������� ����� ������.
            }

        }

        // ���������� ������� ��������� ����������.
        dx = 0;
        rotate = false;
        delay = 0.3;

        window.clear(Color::Black); // ������ ���� ����.

        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIGHT; j++)
            {
                if (field[i][j] == 0)
                    continue;
                tiles.setTextureRect(IntRect(field[i][j] * 34, 0, 34, 34));
                tiles.setPosition(j * 34, i * 34);//������� ������ ������
                window.draw(tiles);//��������� ������
            }

        for (int i = 0; i < 4; i++)// ��������� ���������.
        {
            tiles.setTextureRect(IntRect(blockNum * 34, 0, 34, 34));
            tiles.setPosition(a[i].x * 34, a[i].y * 34);//��������� �������.
            window.draw(tiles);
        }
        window.display();//����������� ����.
    }
    

    Font font;
    font.loadFromFile("consola.ttf");

    Text message_game_over(L"GameOver!", font);// ��������� �� ��������� ����.
    message_game_over.setPosition(80, 100);// ������� ���������.
    message_game_over.setFillColor(Color::Red);// ���� ���������.
    message_game_over.setOutlineThickness(1.0f);

    ostringstream ScoreString;
    ScoreString << score;
    Text message_score(L"Score: " + ScoreString.str(), font);// ��������� ����.
    message_score.setPosition(85, 140);
    message_score.setFillColor(Color::Red);
    message_score.setOutlineThickness(1.0f);

    load_from_file(); // ������ �� ����� ��������.
    sort_leaders(score);// ���������� ����� ������� � ������ ��������� ������.
    save_leaders_to_file(); // ���������� ������������ ����� � ����.

    ostringstream ScoreString1;
    ScoreString1 << scores[0];
    ostringstream ScoreString2;
    ScoreString2 << scores[1];
    ostringstream ScoreString3;
    ScoreString3 << scores[2];
    Text leaders(L"Leaders:\n   " + ScoreString1.str() + "\n   " + ScoreString2.str() + "\n   " + ScoreString3.str(), font);// ������� �������.
    leaders.setPosition(90, 180);
    leaders.setFillColor(Color::White);
    leaders.setOutlineThickness(1.0f);

    window.draw(message_score);// ����� ��������� �����.
    window.draw(message_game_over); // ����� ��������z � ����� ����.
    window.draw(leaders);// ����� �������a �� 3 ������������ ������.
    window.display();

    //���� ����������� ��� ������� �� ������� Esc.
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
