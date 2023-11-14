package com.mzy;

import java.awt.*;

public class BgObj extends GameObj {
    @Override   //实现背景循环播放
    public void painSelf(Graphics gImage) {
        super.painSelf(gImage);
        y += speed;
        if (y >= 0) {
            y = -2000;
        }
    }

    public BgObj() {
        super();
    }

    public BgObj(Image img, int x, int y, double speed) {
        super(img, x, y, speed);
    }
}
