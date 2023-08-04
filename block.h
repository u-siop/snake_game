#ifndef BLOCK_H
#define BLOCK_H

struct Location{                                                            // block's location information
        int x, y;
};

class block{                                                                // didnt need to be class, it was enough to be struct i guess
private:
    const int b_size = 1;                                                   // block's size, but doesn't need to be changed
    Location b_location;
public:
    block(Location location) : b_location{location.x, location.y} {}        // constructor when it contains values
    block() : b_location{0, 0} {}                                           // constructor when it doesn't contain value
    ~block();                                                               // destructor

    void set_location(Location location);
    Location get_location(){ return b_location; };
};
#endif