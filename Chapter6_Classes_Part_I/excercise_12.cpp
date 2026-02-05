#include <iostream>
using namespace std;

/*
Rectangle PROGRAM : Class has attributes length and width
member functions perimeter and areas


set and get functions for length and width
*/    

class Rectangle{
    private: 
        float length;
        float width;

    public:
        Rectangle(float l = 1, float w = 1){
            setLength(l);
            setWidth(w);
        };
        float getLength(void) const;
        float getWidth(void) const;
        void setLength(float);
        void setWidth(float);

        float Perimeter(void) const;
        float Area(void) const;
        
        ~Rectangle(){};


};

float Rectangle :: getLength(void) const
{
    return length;
}

float Rectangle :: getWidth(void) const
{
    return width;
}


void Rectangle :: setLength(float l)
{
    if(l > 0.0f  && l < 20.0f){  
        length = l;  
    }else{
        cout << "Length out of range" << endl;
        length = 1;
    }
    
}

void Rectangle :: setWidth(float w)
{
    if(w > 0.0f  && w < 20.0f){
        width = w;
    }else{
        cout << "Width out of range" << endl;
        width = 1;
    }
}

float Rectangle :: Perimeter(void) const
{
    return (2 * (length + width));
}

float Rectangle :: Area(void) const
{
    return (length * width);
}

int main(){
    Rectangle r1(9,9);
    cout << "Perimeter = " << r1 .Perimeter() <<  endl;
    cout << "Area = " << r1.Area() << endl;

    r1.setWidth(19);
    r1.setLength(19);
    cout << "Perimeter = " << r1.Perimeter() <<  endl;
    cout << "Area = " << r1.Area() << endl;
    return 0;
}