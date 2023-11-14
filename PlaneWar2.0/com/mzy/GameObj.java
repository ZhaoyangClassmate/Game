package com.mzy;

import java.awt.*;

public class GameObj {
    Image img;
    int x;
    int y;
    int width;
    int height;
    double speed;
    GameWin frame;

    public void painSelf(Graphics gImage) {
        gImage.drawImage(img, x, y, null);
    }

    public Rectangle getRec() {
        return new Rectangle(x, y, width, height);
    }

    /*
    各种子对象的实例方法
     */
    public GameObj() {
    }

    public GameObj(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public GameObj(Image img, int x, int y, double speed) {
        this.img = img;
        this.x = x;
        this.y = y;
        this.speed = speed;
    }

    public GameObj(Image img, int x, int y, int width, int height, double speed, GameWin frame) {
        this.img = img;
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.speed = speed;
        this.frame = frame;
    }

    public Image getImg() {
        return img;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

}
