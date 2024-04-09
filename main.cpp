#include <iostream>
#include  <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

const float g = 9.81;
const float energy_coeff_loss = 0.7;
float dt;
const int FRAMES_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

class BBox
{

public:

        float pox = 0;
        float poy = 0;
        float sizex = 300;
        float sizey = 300;

    };


class Particle
{
public:
    int id;
    float mass;
    float psize; //radius
    //position
    float x;
    float y;

    //velocity
    float vx;
    float vy;


    void update()
    {

        dt = SKIP_TICKS / 1000.0;
        x = x + vx*dt;
        y = y + vy*dt;
        vx = vx;
        // m/s = m/s^2 * s
        vy = vy - g*dt;


    }
    void print_stats()
    {
        printf("y:%f vy:%f dt*vy:%f\n", y, vy, vy*dt);
    }
    Particle()
        {
            //define random mass
            mass = 0.001 + static_cast <float>(rand()%1000) / 100.0;
            psize = 1;
            //setup coordinates
            x = 50;
            y = 50;
            vx = 0;
            vy = 100;

        }
    int collide_index(BBox boxboundary)
    //1 if the object is in upper half, -1 if lower half
    {
        if (y >= boxboundary.poy + boxboundary.sizey/2.0)
        {
            return 1;
        }
        if (y < boxboundary.poy + boxboundary.sizey/2.0)
        {
            return -1;
        }

    }

    bool collide(BBox boxboundary)
    //return true if collision occurred
    {
        if ( y + vy*dt - psize/2.0 <= boxboundary.poy)
        { //2 + -4 - 0.5 <= 0

            float bouncey = y - (boxboundary.poy + psize/2.0);//2-0.5 1.5
            printf("Bounce-inn y:%f vy:%f dt*vy:%f bouncey:%f\n", y, vy, vy*dt, bouncey);
            y = -vy*dt - bouncey; //4-1.5 2.5

            vy = -vy*energy_coeff_loss;
            printf("Bounce-out y:%f vy:%f dt*vy:%f bouncey:%f\n", y, vy, vy*dt, bouncey);
            return true;
        }

       if ((y + psize/2.0)> boxboundary.poy + boxboundary.sizey)
        {
            vy = -vy*energy_coeff_loss;
            return true;
        }
    return false;
    }
};



int main()
{

    srand(time(NULL));

    int next_game_tick = clock();
    // GetTickCount() returns the current number of milliseconds
    // that have elapsed since the system was started

    int sleep_time = 0;
    Particle a, b, c;
    int i = 0;
    BBox boundary;

    while (i<10000000)
    {
            //cout << i++ << endl;
            a.update();

            a.print_stats();
            if (a.collide(boundary))
                a.print_stats();

            next_game_tick += SKIP_TICKS;
            sleep_time = next_game_tick - clock();

            if( sleep_time >= 0 )
            {
                std::this_thread::sleep_for (std::chrono::milliseconds(SKIP_TICKS));

            }
            else {
            // hey, we are running behind!
            }


    }

    return 0;
}
