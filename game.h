#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "vector"


//macros
#define MAX_BARS 3

typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1


//Time helper functions

inline double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double) (end->tv_sec - start->tv_sec) +
            (double) (end->tv_nsec - start->tv_nsec) * 1.0e-9;
}

inline void timeCopy(struct timespec *dest, struct timespec *source)
{
    memcpy(dest, source, sizeof (struct timespec));
}

inline void recordTime(struct timespec *t)
{
    clock_gettime(CLOCK_REALTIME, t);
}

/**
 * Sprite animation class
 */
class Sprite {
protected:
    GLuint glTexture;
    int frameCount;
    int rows;
    int cols;
    int origHeight;
    int origWidth;
    float height;
    float width;
    int currentFrame;
    float posX;
    float posY;
    double delay;
    struct timespec time;
    bool visible;
    int direction;
    bool repeating;
    bool reverse;
    float angle; //rotating sprites
public:
    Sprite(
            const std::string & filename,
            int frameCount,
            int rows,
            int cols,
            double delay,
            float height,
            float width
            );

    ~Sprite();

    void setPos(float x, float y);
    void setSize(float height, float width);
    float getHeight();
    float getWidth();
    double getDelay();
    float getPosY();
    float getPosX();
    virtual void draw();
    void physics();
    void setVisible(bool value);
    bool getVisible();
    void reset();
    // Mortana direction
    int getDirection();
    void setDirection(int);
    void setFrameIndex(int);
    int getFrameIndex();
    bool getRepeating();
    void setRepeating(bool);
    void setReverse(bool);
    bool getReverse();
    float getAngle();
    void setAngle(float);
};
/*
 Enemy collision*/
class Enemycollsion{
        public :
struct enemyCollision; 
// going to intersect?
bool intersected;
// intersecting now?
bool intersect;
// 
Vec transVector;
void collision (Vec axis, float min, float max);

    
    
};




/*
 * This creates a class called GlobalSprite, has one variable of type Sprite*,
 * named characterGirl. It also at the same time makes it accessible globally 
 *  by the name of globalSprite
 */
struct GlobalSprite {
    Sprite* mortana;
    Sprite* mortanaJump;
    Sprite* background[5];
    Sprite* light[4];
    Sprite* life[11];
    Sprite* gameover;
    Sprite* credits[12];
    Sprite* scores[5];
    //Enemy sprites
    Sprite* pinkghost;
    Sprite* whiteghost;
    // Sprite* pacghost;
    Sprite* blkcat;
    Sprite* blkcatsit;
    Sprite* backgroundMenu;
    Sprite* pacghost;

};

class Level {
public:
    unsigned char arr[16][180];
    int nrows, ncols;
    int tilesize[2];
    Flt ftsz[2];
    Flt tile_base;
    int dynamicHeight[180];
    void renderBackground(void);

    Level()
    {
        for (int i = 0; i < 180; i++) {
            dynamicHeight[i] = -1;
            //lev = 0;
        }
        tilesize[0] = 32;
        tilesize[1] = 32;
        ftsz[0] = (Flt) tilesize[0];
        ftsz[1] = (Flt) tilesize[1];
        tile_base = 220.0;

        FILE *fpi = fopen("level1.txt", "r");
        if (fpi) {
            nrows = 0;
            char line[100];
            while (fgets(line, 100, fpi) != NULL) {
                removeCrLf(line);
                int slen = strlen(line);
                ncols = slen;

                for (int j = 0; j < slen; j++) {
                    arr[nrows][j] = line[j];
                }
                ++nrows;
            }
            fclose(fpi);

        }


        for (int i = 0; i < nrows; i++) {
            for (int j = 0; j < ncols; j++) {
                printf("%c", arr[i][j]);
            }
            printf("\n");
        }
    }

    void removeCrLf(char *str)
    {
        char *p = str;
        while (*p) {
            if (*p == 10 || *p == 13) {
                *p = '\0';
                break;
            }
            ++p;
        }
    }

};

extern GlobalSprite globalSprite;

class Battery {
public:
    int arr[MAX_BARS];
    int points;
    int bcount;

    Battery()
    {
        arr[MAX_BARS] = 540;
        bcount = 0;
    }
    void chargeObject(); //was battappear
    void drawBattery(void);
    void drawFlashlight();
    void grabCharge(); //was grabBatt
    void healthBar(); //was delete
    void gameOver();
};


/**
 * Menu classes
 */
// class for sub menu items
//forward declare menu
class Menu;
class MainMenu;

class MenuItem {
    //Menu is friend of MenuItem
    friend Menu;
public:
    MenuItem(std::string txt, int x, int y, int w, int h);
    void draw();
    void setPos(int, int);
    int getPosX();
    int getPosY();
    std::string getText();
    void setText(std::string);
private:
    std::string text;

    int posX;
    int posY;

    int width;
    int height;

    bool highlight;
};

class Menu {
public:

    Menu();

    virtual ~Menu();

    void add(MenuItem item);

    virtual void draw();

    bool setSelectedIndex(unsigned int index);

    unsigned int getSelectedIndex();

    unsigned int getSize();

protected:
    //stores the menu items
    std::vector<MenuItem> menuItems;
    int selectedItemIndex;
};

class MainMenu : public Menu {
public:
    MainMenu();

    void draw();

    void resize(int oldw, int neww, int oldh, int newh);

    void keyboardInput(int key);
};

/*
 Enums for Menu
 */
enum State {
    STATE_STARTUP,
    STATE_GAMEPLAY,
    STATE_GAMEOVER,
    STATE_GAMEPAUSE,
    STATE_HIGHSCORE,
    STATE_CREDITS

};

class Enemy {
public:

    Enemy(float x, float y, int spriteId) :
    x(x), y(y), spriteId(spriteId), frameIndex(0)
    {
    }

    float x, y;
    int spriteId;
    int frameIndex;
};

class Global {
public:
    Display *dpy;
    Window win;
    unsigned char keys[65536];
    int done;
    int xres, yres;
    //camera is centered at (0,0) lower-left of screen. 
    Flt camera[2];
    Vec ball_pos;
    Vec ball_vel;
    Battery batt;
    Level lev;
    int keepTrack;
    int keyCount;
    int spacebar;
    int hardSelection;
    int walkFrame;
    int walk;
    int shock;
    int shockFrame;
    Vec box[20];
    double delay;
    double shockDelay;
    GLuint walkTexture;
    //Gluint shockTexture;
    Ppmimage *shockImage;
    MainMenu mainMenu;
    //bool mainMenuOpen;
    struct timespec timeCurrent;
    struct timespec shockTime;
    State state; //*
    //Physics variables for mortana 
    float mortanaVelY;
    Vec mortanaPos;
    Vec lightPos;
    // physics for blk cat
    float catVelY;
    Vec catPos;

    //Physics variable ghosts
    float pinkghost;
    std::vector<Enemy> enemies;




    float lightVelY;

    ~Global()
    {
        logClose();
    }

    Global()
    {
        logOpen();
        state = STATE_STARTUP; //*
        camera[0] = camera[1] = 0.0;
        done = 0;
        xres = 800;
        yres = 600;
        keyCount = 0;
        keepTrack = 0;
        spacebar = 0;
        hardSelection = 0;
        shock = 0;
        shockFrame = 0;
        shockImage = NULL;
        shockDelay = 0.1; // for now
        memset(keys, 0, 65536);
        mortanaPos[0] = xres / 2;
        mortanaPos[1] = 100;
        lightPos[0] = xres / 2;
        lightPos[1] = 100;


        // blkcat
        catPos[0] = xres / 2;
        catPos[1] = 70;
    }
};

extern Global gl;

//function prototypes
void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void checkResize(XEvent *e);
void checkMouse(XEvent *e);
void checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);
void initCharacterSprites();
void initBackgroundSprites();
void physicsMortana();
void monitorCTRLC(int);
// enemy

//instructions for user
void renderTutorial();
unsigned char *buildAlphaData(Ppmimage *img);

/*
 ENEMY
 */
void initEnemySprites();
void physicsGhosts();
void renderEnemySprites();

/*
 ENEMY COLLISION
 */
class Vec2 {
    //public:
    //enemy {
    //float coord[3];
    //struct{
    // float x;
    // float y;
    //float z;
    //  }
    //  }
};

//function prototypes
void renderBackground(void); //render prototype
void applyBackgroundMovement(void);
void initCharacterSprites(); // for Sprite characters
void renderCharacterSprites(); // for Sprite characters
void renderBackgroundSprites(); // for Sprite characters
void physicsCharacterSprites(); //Temporary test function for moving sprites
void drawLight(void);
void LightCollision();
void displayColors(); //different colors for background
void FlashlightPower(); //Displays how much power battery has
void drawFlashlightPower(float); //displays the bar of battery life
//void menuBackground(); //menu background

void initLightSprite(); // for light/electicity to kill ghosts
void initLifeBarSprite(); // 
void initGameOverSprite(); // 
void initCreditBackground(); // 
void renderLightSprite(); // for light/electricity to kill ghosts
void renderLifeBarSprite(); // 
void renderGameOverSprite(); // 
void renderCreditBackground(); // 
void physicsLightSprite();
void initMenuBackground(); // main menu background
void renderMenuBackground(); //main menu
void initHighScores();
void renderHighScores();
void redScreenFlash();
void hardMode();

#endif /* GAME_H */
