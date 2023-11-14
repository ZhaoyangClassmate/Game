package com.mzy;

import java.awt.*;

public class ShellObj extends GameObj {
    @Override
    public void painSelf(Graphics gImage) {
        super.painSelf(gImage);
        y -= speed;
        if (y < 0) {
            this.x = -100;
            this.y = 100;
            GameUtils.removeList.add(this);
        }
    }

    @Override
    public Rectangle getRec() {
        return super.getRec();
    }

    public ShellObj(Image img, int x, int y, int width, int height, double speed, GameWin frame) {
        super(img, x, y, width, height, speed, frame);
    }

    @Override
    public Image getImg() {
        return super.getImg();
    }
}
