package com.mzy;

import java.awt.*;

public class EnemyObj extends GameObj {
    @Override
    public void painSelf(Graphics gImage) {
        super.painSelf(gImage);
        y += speed;
        //敌我飞机碰撞检测
        if (this.getRec().intersects(this.frame.planeObj.getRec())) {
            GameWin.state = 3;
        }
        //敌方的越界消失 判断条件 y > 600 改变后的坐标（-200，-200）
        if (y > 600) {
            this.x = -200;
            this.y = 200;
            GameUtils.removeList.add(this);
        }
        //敌方消失前移动到（-200，-200） 我方子弹（-100，100）
        for (ShellObj shellObj : GameUtils.shellObjList) {
            if (this.getRec().intersects(shellObj.getRec())) {
                ExplodeObj explodeObj = new ExplodeObj(x, y);
                GameUtils.explodeObjList.add(explodeObj);
                GameUtils.removeList.add(explodeObj);
                System.out.println("碰撞了");
                shellObj.setX(-100);
                shellObj.setY(100);
                this.x = -200;
                this.y = -200;
                GameUtils.removeList.add(shellObj);
                GameUtils.removeList.add(this);
                GameWin.score++;
            }
        }
    }

    @Override
    public Rectangle getRec() {
        return new Rectangle(x, y, width, height);
    }

    public EnemyObj() {
        super();
    }

    public EnemyObj(Image img, int x, int y, int width, int height, double speed, GameWin frame) {
        super(img, x, y, width, height, speed, frame);
    }
}
