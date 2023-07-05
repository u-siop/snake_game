#include<iostream>
using namespace std;

class block{
private:
    int size = 1;
    int location_x = 0;
    int location_y = 0;
public:
    void increase_size(){
        this->size++;
    }
    void update_location_x(int x){
        this->location_x += x;
    }
    void update_location_y(int y){
        this->location_y += y;
    }
    void print_location_x(){

    }
    void print_location_y(){

    }
};