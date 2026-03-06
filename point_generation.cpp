#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Point{
public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    float x, y;
};

int main() {
    srand(time(nullptr));
    std::ofstream fout("points.json");
    json json_file;
    json_file["points"] = json::array();

    // center1 и center2 - центры кластеров 1 и 2
    Point center1, center2;

    // задаем центры кластеров в диапазоне от -10 000 до 10 000
    center1.x = rand() % 20000 - 10000;
    center1.y = rand() % 20000 - 10000;
    center2.x = rand() % 20000 - 10000;
    center2.y = rand() % 20000 - 10000;

    float rad = sqrt(pow((center1.x - center2.x), 2) + pow((center1.y - center2.y), 2)) * 0.5 * 0.8;

    // точки первого кластера
    for(int i = 0; i < 1000; i++){
    // for(int i = 0; i < int((rand() % 9900 + 100)); i++){
        bool f;
        f = rand() % 2;
        Point current;

        if(f){
            current.x = rand() % (int)(2*rad) + (center1.x - rad);
            current.y = rand() % (int)(2*rad) + (center1.y - rad);
        }
        else {
            current.x = rand() % (int)(2*rad) + (center2.x - rad);
            current.y = rand() % (int)(2*rad) + (center2.y - rad);  
        }

        json_file["points"].push_back({{"x", current.x}, {"y", current.y}});
    }

    fout << "{\n";
    fout << "    \"points\": [\n";

    for (int i = 0; i < int(json_file["points"].size()); i++){
        fout << "       " << json_file["points"][i].dump();
        if (i < json_file["points"].size() - 1) {
            fout << "," << "\n";
        }
    }

    fout << "    ]\n";  
    fout << "}\n";
    return 0;
}
