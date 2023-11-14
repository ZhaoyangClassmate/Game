package com.mzy;

import java.awt.*;

public class ExplodeObj extends GameObj {

    static Image[] pic = new Image[16];

    int explodeCount = 0;

    //静态初始化爆炸图片集合
    static {
        for (int i = 0; i < pic.length; i++) {
            pic[i] = Toolkit.getDefaultToolkit().getImage("src/imgs/explode/e" + (i + 1) + ".gif");
        }
    }

    public ExplodeObj(int x, int y) {
        super(x, y);
    }

    @Override
    public void painSelf(Graphics gImage) {

        if (explodeCount < 16) {
            super.img = pic[explodeCount];
            super.painSelf(gImage);
            explodeCount++;
        }
    }
}
