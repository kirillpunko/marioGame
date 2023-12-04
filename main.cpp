#include <SFML/Graphics.hpp>
#include "view.h"
#include <iostream>
#include <string>
#include <sstream>
#include "iostream"
#include "Level.h"
#include <vector>
#include <list>

#include "tinyxml/tinyxml.h"

using namespace sf;

////////////////////////Kласс Player//////////////////
    class Player {
    private: float x, y;
    public:
        std::vector<Object> obj;
        float w, h, dx, dy, speed;
        enum stateObject { left,right,up,down,jump,stay };
        stateObject state;
        bool isLife, onGround, isMove;
        String File;
        bool isescaped;
        Image image;
        Texture texture;
        Sprite sprite;
        Player(String F, Level& lev, float X, float Y, float W, float H) {
            isLife = true; onGround = true; isescaped = false; isMove = false; state = stay;
            File = F;
            w = W; h = H;
            obj = lev.GetAllObjects();
            image.loadFromFile("images/" + File);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            x = X; y = Y;
            sprite.setTextureRect(IntRect(0, 0, w, h));
            sprite.setOrigin(w/2,h/2);
        }

        FloatRect getRect() {
            return FloatRect(getPlayerX(), getPlayerY(), w, h);
        }

    void update(float time) {
        control();
        switch (state) {
        case stay:  break;
        case right: dx = dx + 0.04 * time; break;//тут было 0.09
        case left: dx = dx-0.04 * time; break;//тут было 0.09
        case jump: break;
        case down: break;
        }
        if (!isMove) {
            dx = dx * 0.85;
            if (dx<0.52 && dx>-0.52) {
                dx = 0;
            }
        }
        if (dx > 3) dx = 3;
        if (dx < -3) dx = -3;
        x += dx * time;
        checkCollisionWithMap(dx, 0);
        y += dy * time;
        checkCollisionWithMap(0, dy);
        speed = 0;
        getPlayerViewCoordinates(x, y);
        sprite.setPosition(x+w/2, y+h/2);
        isescaped = checkout();
  
        dy = dy + 0.18 * time;
    }

    void control() {
        isMove = false;
        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)){
           state=left; 
           speed = 3;
           isMove = true;
           //CurrentFrame += 0.1 * time;
           //if (CurrentFrame > 4)
           //    CurrentFrame -= 4;
           //p.sprite.setTextureRect(IntRect(32 + 32 * int(CurrentFrame), 64, -32, 32));
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)){
            state = right;
            isMove = true;
            speed = 3;
            //CurrentFrame += 0.1 * time;
            //if (CurrentFrame > 4)
            //    CurrentFrame -= 4;
            //p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
            state = jump;
            dy = -10;
            onGround = false;
            isMove = true;
            //CurrentFrame += 0.1 * time;
            //if (CurrentFrame > 4)
            //    CurrentFrame -= 4;
            //p.sprite.setTextureRect(IntRect(576 + 32 * int(CurrentFrame), 64, 32, 32));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
            state = down;
            speed = 3;
            //CurrentFrame += 0.1 * time;
            //if (CurrentFrame > 4)
            //    CurrentFrame -= 4;
            //p.sprite.setTextureRect(IntRect(32 * int(CurrentFrame), 64, 32, 32));
        }
    }

    float getPlayerX() {
        return x;
    }
    float getPlayerY() {
        return y;
    }

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = 0; i < obj.size(); i++) {
            if (getRect().intersects(obj[i].rect)) {
                if (obj[i].name == "solid" || obj[i].name == "coinBlock")
                {
                    if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
                    if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
                    if (Dx > 0) { x = obj[i].rect.left - w; }
                    if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
                }
            }

        }
    }
    bool checkout() {
        if (isescaped) {
            return true;
        }
        for (int i = 0; i < obj.size(); i++) {
            if (getRect().intersects(obj[i].rect)) {
                if (obj[i].name == "exit") {
                    return true;
                }
            }
        }
        return false;
    }
    };
//////////////////////////////////////////////////////////////////////

int main()
{
    RenderWindow win(VideoMode(1500,980), "SFML Game");
    win.setFramerateLimit(100);
    view.reset(FloatRect(0, 0, 1280, 960));
    std::string mapNum;
    mapNum = "level1.tmx";

    //level
    Level lvl;
    lvl.LoadFromFile(mapNum);

    //Add icon
    Image icon;
    if (!icon.loadFromFile("images/win_icon.png")) {
        return 1;
    }
    win.setIcon(360, 360, icon.getPixelsPtr());
 
    //create player
    Object player = lvl.GetObject("player");

    Image heroImage;
    heroImage.loadFromFile("images/characters3.png");
    Player p("characters3.png",lvl, player.rect.left, player.rect.top, 48,64 );

    
    float CurrentFrame = 0;
    Clock clk;


    while (win.isOpen())
    {

        float time = clk.restart().asMilliseconds();
        clk.restart();
        time = time / 10;

        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)
                win.close();
        }

        if (!p.isescaped){
        getPlayerViewCoordinates(p.getPlayerX(), p.getPlayerY());
        }
        
        win.setView(view);
        p.update(time);
        changeView();
        
       
        win.clear(Color(208, 244, 247, 255));
        ////////Отрисовка карты////////////
        lvl.Draw(win);
        win.draw(p.sprite);
        //////////////////////////////////    
        if (p.isescaped) {
            Font font;
            font.loadFromFile("CyrilicOld.ttf");
            Text text("", font, 40);
            //text.setColor(Color::Green);
            text.setStyle(Text::Bold);
            text.setString("YOU ESCAPED");
            text.setPosition(view.getCenter().x-150, view.getCenter().y-125);
            win.draw(text);
        }
        win.display();  

    }
    return 0;
}