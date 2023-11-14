package com.mzy;

import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class PlaneObj extends GameObj {
    @Override
    public void painSelf(Graphics gImage) {
        super.painSelf(gImage);
        //我方飞机与敌方boss的碰撞检测
        if (this.frame.bossObj != null && this.getRec().intersects(this.frame.bossObj.getRec())) {
            GameWin.state = 3;
        }
    }

    @Override   //重写我方战斗机矩形图片的大小
    public Rectangle getRec() {
        return new Rectangle(x + 28, y + 10, width + 5, height + 5);
    }

    public PlaneObj(Image img, int x, int y, int width, int height, double speed, GameWin frame) {
        super(img, x, y, width, height, speed, frame);
        this.frame.addMouseMotionListener(new MouseAdapter() {
            @Override
            public void mouseMoved(MouseEvent e) {
                PlaneObj.super.x = e.getX() - 42;
                PlaneObj.super.y = e.getY() - 35;
            }
        });
    }

    @Override
    public Image getImg() {
        return super.getImg();
    }
}
