#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <cmath>

using namespace std;
// класс для определения, в какие направления мы можем пойти от текущего положения нуля.
class Link {
private:
    vector < vector <short int> > graph;
public:
    Link(short int n) {
        int size = n * n;
        graph.resize(size);
        for(short int i = 0; i < size; i++) {
            graph[i].resize(4, 255);
            // вверх
            if(i - n > -1)
                graph[i][0] = i - n;
            // вниз
            if(i + n < size)
                graph[i][1] = i + n;
            // влево
            if(i % n != 0)
                graph[i][2] = i - 1;
            // вправо
            if(i % n != n - 1)
                graph[i][3] = i + 1;
        }

    }
    // vertex - положение 0, vec - направление от 0 до 3
    short int getVector(short int vertex, short int vec) {
        return graph[vertex][vec];
    }
};
// класс для определения полей и функций для одной вершины графа состояний
// на один объект уходит всего 8 + 2 + 2 + 2 = 14 байт
class Graph {
private:
    // уникальный ключ
    unsigned long long key = 0;
    // шаги от начальной вершины до этой, эвристическая оценка, позиция нуля
    short int step = 0, estm = 0, zero = 15, id = 0;
public:
    short getStep() {
        return step;
    }
    short getEstm() {
        return estm;
    }

    short int getzeroition() const {
        return zero;
    }

    unsigned long long getKey(void) const {
        return key;
    }

    // возвращает ряд позиции
    int getRow(int i) const {
        return i >> 2;
    }
    // возвращает столбец позиции
    int getCol(int i) const {
        return i & 3;
    }

    short getId() {
        return id;
    }

    // изначально вершина определяется как правильная(конечная)
    Graph() {
        step = 0;
        estm = 0;
        id  = 0;
        zero = 15;
        key = 18364758544493064720;
    }
    // подсчет ключа - перевод в 16-ричное число
    unsigned long long countKey(short*comb) {
        unsigned long long combination = 0;
        for (int i = 15; i > 0; i--) {
            combination += comb[i];
            combination <<= 4;
            //cout << combination << " ";
        }
        combination += comb[0];
        return combination;
    }
    // перевод из ключа обратно в поле - массив из 16 чисел
    void countComb(short*comb) const  {
        unsigned long long temp = key;
        for(int i = 0; i < 16; i++) {
            comb[i] = (temp & 15);
            temp >>= 4;
        }
    }
    // функция для подсчета полной эвристики начального состояния
    short int mark(void)
    {
        short*comb = new short[16];
        countComb(comb);
        short grade = 0;
        short int zero = getzeroition();
        for(int i = 0; i < 16; i++) {
            if(i != zero) {
                grade += manhattan(comb, i);
                grade += linear(comb, i);
            }
        }
        //takaken(comb);
        delete[] comb;
        return grade;
    }


    void takaken(short *comb) {
        short first = 0, second = 0;
        for(int i = 0; i < 16; i++) {
            if(i != zero) {
                for (int j = i + 1; j < 16; j++) {
                    if (j != zero) {
                        if(comb[i] > comb[j])
                            first++;
                    }
                }
            }
        }
        short *new_comb = new short[16];
        for(int i = 0; i < 4; i++) {
            int row = i << 2;
            for (int j = 0; j < 4; j++)
                new_comb[row + j] = comb[i + (j << 2)];
        }

        for(int i = 0; i < 16; i++) {
            if(i != zero) {
                for (int j = i + 1; j < 16; j++) {
                    if (j != zero) {
                        if(getCol(new_comb[i]) > getCol(new_comb[j])
                           || (getCol(new_comb[i]) == getCol(new_comb[j]) && getRow(new_comb[i]) > getRow(new_comb[j])))
                            second++;
                    }
                }
            }
        }

        id =  first / 3 + first % 3 + second / 3 + second % 3;
        delete[] new_comb;
    }

    // манхэттенское расстояние для одной позиции
    short manhattan(short*comb, short int i) const {
        return abs(getRow(comb[i]) - getRow(i)) + abs(getCol(comb[i]) - getCol(i));
    }
    // линеный конфликт с участием этой позиции, в сравнении с фишками с бОльшими номерами
    short linear(short*comb, short int i) const {
        short grade = 0;
        if(getRow(comb[i]) == getRow(i)) {
            for (int j = i + 1; j < 16; j++) {
                if(j != zero && getRow(i) == getRow(j) && getRow(comb[j]) == getRow(j) && comb[i] > comb[j])
                    grade += 2;
            }
        }

        if(getCol(comb[i]) == getCol(i)) {
            for (int j = i + 1; j < 16; j++) {
                if(j != zero && getCol(i) == getCol(j) && getCol(comb[j]) == getCol(j) && comb[i] > comb[j])
                    grade += 2;
            }
        }
        return grade;
    }
    // подсчет линейного конфликта только с участием этой фишки
    short linear_element(short*comb, short int i) const {
        short grade = 0;
        int row = (getRow(i) + 1) << 2;
        if(getRow(comb[i]) == getRow(i)) {
            for (int j = row - 4; j < row; j += 1) {
                if(j != zero && getRow(comb[j]) == getRow(j)) {
                    if((j < i && comb[j] > comb[i]) || (j > i && comb[j] < comb[i])) {
                        grade += 2;
                    }
                }
            }
        }
        if(getCol(comb[i]) == getCol(i)) {
            for (int j = getCol(i); j < 16; j += 4) {
                if(j != zero && getCol(comb[j]) == getCol(j)) {
                    if((j < i && comb[j] > comb[i]) || (j > i && comb[j] < comb[i])) {
                        grade += 2;
                    }
                }
            }
        }
        return grade;
    }
    // передвигаем ноль - переходим из одного состояния в другое
    void moveZero(short int vertex){
        // увеличиваем шаг
        step++;
        short*comb = new short [16];
        countComb(comb);
        // из старой эвристики вычитаем старые значения манхэттена и линейного для перемешаемой фишки
        estm -= manhattan(comb, vertex) + linear_element(comb, vertex);
        // изменяем ключ
        unsigned long long dif = abs(15 - comb[vertex]);
        key += dif << (vertex << 2);
        key -= dif << (zero << 2);
        // перемещаем фишки во временом массиве, меняем позицию нуля
        swap(comb[vertex], comb[zero]);
        swap(vertex, zero);
        // прибавляем новые значения манхэттена и линейного для фишки(она уже на новой позиции)
        estm += manhattan(comb, vertex) + linear_element(comb, vertex);
        //takaken(comb);
        delete[] comb;
        return;
    }

    // проверка на решаемость
    bool check() {
        short*comb = new short[16];
        countComb(comb);
        int num = 0, zero = getzeroition();
        for(int i = 0; i < 16; i++) {
            if(i == zero)
                continue;
            for(int j = 0; j < i; j++)
                if(j != zero && comb[j] > comb[i])
                    num++;
        }
        int criteria = (zero >> 2) + num;
        delete[] comb;
        return criteria % 2 == 0;
    }

    Graph&operator=(const Graph& graph) {
        this -> key = graph.key;
        this -> step = graph.step;
        this -> estm = graph.estm;
        this -> zero = graph.zero;
        this -> id = graph.id;
        return *this;
    }

    friend istream&operator>>(istream&, Graph&);
};
// считываем начальное поле
// оказывается выгодным 0 сделать равным 15, а все остальные фишки уменьшить на 1
// таким образом конечное правильное поле будет равным 0, 1, 2, 3 ... 15
istream&operator>>(istream& is, Graph& graph) {
    short*comb = new short[16];
    for(int i = 0; i < 16; i++) {
        is >> comb[i];
        if(comb[i] > 0)
            comb[i]--;
        else {
            comb[i] = 15;
            graph.zero = i;
        }
    }
    // считаем ключ
    graph.key = graph.countKey(comb);
    // один раз за всё время считаем полную эвристику
    graph.estm = graph.mark();
    delete[] comb;
    return is;
}

class Solution {
private:
    vector < stack<Graph> > myset;
    // мап - ключ, число шагов и направление перемещения нуля(с помощью сдвигов реально закодировать их в 2 байта и сэкономить)
    unordered_map <unsigned long long, short int> way;
    // граничное число шагов сверху, текущий индекс(эвристика) стэка, из которого мы будем брать вершину
    short int step_limit = 81, hero = 0;
public:
    Solution(Graph &start, int heuristic_iter) {
        step_limit = 81;
        Graph right;
        Link link(4);
        myset.resize(81);
        hero = start.getEstm();
        myset[hero].push(start);
        way[start.getKey()] = 4; // 4 - сигнал к остановке, когда будем восстанвливать путь
        for(int iterations = 0; iterations < heuristic_iter; iterations++) {
            // если вдруг значения эвристики опустели, двигаемся вверх
            while(myset[hero].empty()) {
                if(hero == step_limit)
                    return;
                hero++;
            }
            // достаем вершину
            start = myset[hero].top();
            myset[hero].pop();
            // если это вершина конечного состояния - понижаем верхнюю границу
            // -1 здесь потому, что не существует двух решений разной четности
            if(start.getKey() == right.getKey()) {
                step_limit = start.getStep() - 1;
                hero = 0;
                continue;
            }
            make_move(start, link);
        }
    }

    void make_move(Graph &start, Link &link) {
        short int zero = start.getzeroition();
        short new_hero = hero;
        for(short int i = 0; i < 4; i++) {
            // куда передвинуть ноль
            short int cur = link.getVector(zero, i);
            //если по направлению нет возможного перемещения
            if(cur == 255)
                continue;
            // передвигаем ноль
            Graph next = start;
            next.moveZero(cur);
            // добавляем к эвристике наши шаги с коэффициентом
            short difference = ((next.getStep() * 3) >> 2) + next.getEstm();

            // если текущее состояние не посещено и нижняя граница шагов меньше верхней
            // то добавляем состояние в пройденные и вершину в стэк
            // шаги + манхэттен + линейный конфликт есть число шагов, меньше которого
            // мы точно не достигнем решения из заданной комбинации, то есть нижняя граница
            if(way.find(next.getKey()) == way.end()) {
                if((next.getEstm() + next.getStep()) < step_limit) {
                    way[next.getKey()] = (next.getStep() << 3) + i;
                    myset[difference].push(next);
                    new_hero = min(difference, new_hero);
                }
            }
                // если мы можем дойти до указанной вершины меньшим числом шагов, то обновляем её
            else if(next.getStep() < get_step(way[next.getKey()])) {
                way[next.getKey()] = (next.getStep() << 3) + i;
                myset[difference].push(next);
                new_hero = min(difference, new_hero);
            }
        }
        // если какое-либо из наших перемещений привело к уменьшению эвристики(что хорошо)
        // то мы должны начать рассматривать нужный нам стэк
        hero = new_hero;
    }

    short int get_turn(short int point) {
        return point & 7;
    }

    short int get_step(short int point) {
        return point >> 3;
    }

    friend ostream&operator<<(ostream& os, Solution& solver);
};
// восстанавливаем обратный путь, начиная с конца
ostream&operator<<(ostream& os, Solution& solver) {
    Link link(4);
    Graph end;
    short int cur = solver.get_turn(solver.way[end.getKey()]);
    string s = "";
    char deck[4] = {'D', 'U', 'R', 'L'};
    while(cur != 4) {
        s += deck[cur];
        // так как мы идем с конца, то нужно инвертировать направления
        if(cur < 2)
            cur ^= 1;
        else if(cur == 2)
            cur = 3;
        else
            cur = 2;

        end.moveZero(link.getVector(end.getzeroition(), cur));
        cur = solver.get_turn(solver.way[end.getKey()]);
    }
    os << s.length() << endl;
    // а теперь инвертировать и сам путь
    for(int i = s.length() - 1; i > -1; i--)
        os << s[i];
    return os;
}

int main(){
    Graph start;
    cin >> start;
    if(start.check()) {
        cout << -1;
        return 0;
    }
    Solution solver(start, 175000);
    cout << solver << endl;
    return 0;
}