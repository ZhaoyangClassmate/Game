package com.mzy;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class GameWin extends JFrame {

    /**
     * 游戏状态 0未开始,1游戏中,2通关失败,3通关成功,4暂停,5重新开始
     */

    //定义游戏默认状态
    static int state = 0;

    //双缓存图片
    Image offScreenImage;

    //窗口宽高
    int width = 1320;
    int height = 780;

    //计数器
    double random;
    int time = 0;

    //背景对象
    Bg bg = new Bg();

    //敌方鱼类
    Enamy enamy = new Enamy_1_L();

    //我方鱼类
    MyFish myFish = new MyFish();

    //boss鱼类
    Enamy boss;

    //是否生成boss
    boolean isboss = false;

    /**
     * 游戏操作方法
     */
    public void launch() {

        Music audioPlayWave = new Music("src/皇家萌卫.wav");
        audioPlayWave.start();
        @SuppressWarnings("unused")
        int musicOpenLab = 1;
        
        //界面布局
        this.setVisible(true);
        this.setSize(width, height);
        this.setLocationRelativeTo(null);
        this.setTitle("大鱼吃小鱼");
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        //开始或者失败后重新游戏
        this.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                super.mouseClicked(e);
                if (e.getButton() == 1 && state == 0) {
                    state = 1;
                    repaint();
                }
                if (e.getButton() == 1 && (state == 2 || state == 3)) {
                    reGame();
                    state = 1;
                }
            }
        });

        //键盘移动我方鱼类
        this.addKeyListener(new KeyAdapter() {
            @Override //按压
            public void keyPressed(KeyEvent e) {
                super.keyPressed(e);
                if (e.getKeyCode() == 87) {
                    GameUtils.UP = true;
                }
                if (e.getKeyCode() == 83) {
                    GameUtils.DOWN = true;
                }
                if (e.getKeyCode() == 65) {
                    GameUtils.LEFT = true;
                }
                if (e.getKeyCode() == 68) {
                    GameUtils.RIGHT = true;
                }
                if (e.getKeyCode() == 32) {
                    switch (state) {
                        case 1:
                            state = 4;
                            GameUtils.drawWord(getGraphics(), "游戏暂停！", Color.red, 50, 580, 450);
                            break;
                        case 4:
                            state = 1;
                            break;
                    }
                }
            }

            @Override //抬起
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == 87) {
                    GameUtils.UP = false;
                }
                if (e.getKeyCode() == 83) {
                    GameUtils.DOWN = false;
                }
                if (e.getKeyCode() == 65) {
                    GameUtils.LEFT = false;
                }
                if (e.getKeyCode() == 68) {
                    GameUtils.RIGHT = false;
                }
            }
        });

        //线程操纵游戏帧率
        while (true) {
            repaint();
            time++;
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    /**
     * 游戏阶段绘制鱼类
     */
    @Override
    public void paint(Graphics g) {
        //双缓存加载图片
        offScreenImage = createImage(width, height);
        Graphics gImage = offScreenImage.getGraphics();
        bg.paintSelf(gImage, myFish.level);

        switch (state) {
            case 0:
                break;
            case 1:
                myFish.paintSelf(gImage);
                logic();
                for (Enamy enamy : GameUtils.EnamyList) {
                    enamy.paintSelf(gImage);
                }
                if (isboss) {
                    boss.x = boss.x + boss.dir * boss.speed;
                    boss.paintSelf(gImage);
                    if (boss.x < 0) {
                        gImage.setColor(Color.red);
                        gImage.fillRect(boss.x, boss.y + 100, 2400, boss.height / 20);
                    }
                }
                break;
            case 2:
                for (Enamy enamy : GameUtils.EnamyList) {
                    enamy.paintSelf(gImage);
                }
                if (isboss) {
                    boss.paintSelf(gImage);
                }
                break;
            case 3:
                myFish.paintSelf(gImage);
                break;
            case 4:
                return;
            default:
        }
        g.drawImage(offScreenImage, 0, 0, null);
    }

    /**
     * 游戏关卡难度逻辑
     */
    void logic() {
        //关卡难度
        if (GameUtils.count < 5) {
            GameUtils.level = 0;
        } else if (GameUtils.count < 15) {
            GameUtils.level = 1;
        } else if (GameUtils.count < 40) {
            GameUtils.level = 2;
        } else if (GameUtils.count < 75) {
            GameUtils.level = 3;
        } else if (GameUtils.count > 150) {
            state = 3;
        }

        //我方鱼升级
        if (29 < GameUtils.count && GameUtils.count < 79) {
            myFish.level = 2;
        } else if (GameUtils.count > 79) {
            myFish.level = 3;
        }

        //阶段生成敌方鱼
        random = Math.random();
        switch (GameUtils.level) {
            case 3:
                if (time % 120 == 0) {
                    boss = new Enamy_Boss();
                    isboss = true;
                }
            case 2:
                if (time % 120 == 0) {
                    if (random < 0.5) {
                        enamy = new Enamy_3_L();
                    } else {
                        enamy = new Enamy_3_R();
                    }
                }
                GameUtils.EnamyList.add(enamy);
            case 1:
                if (time % 60 == 0) {
                    if (random < 0.5) {
                        enamy = new Enamy_2_L();
                    } else {
                        enamy = new Enamy_2_R();
                    }
                }
                GameUtils.EnamyList.add(enamy);
            case 0:
                if (time % 10 == 0) {
                    if (random < 0.5) {
                        enamy = new Enamy_1_L();
                    } else {
                        enamy = new Enamy_1_R();
                    }
                }
                GameUtils.EnamyList.add(enamy);
                break;
            default:
        }

        //生成敌方鱼
        for (Enamy enamy : GameUtils.EnamyList) {
            enamy.x = enamy.x + enamy.dir * enamy.speed;

            //boss鱼碰撞检测
            if (isboss) {
                if (boss.getRec().intersects(enamy.getRec())) {
                    enamy.x = -200;
                    enamy.y = -200;
                }
                if (boss.getRec().intersects(myFish.getRec())) {
                    state = 2;
                }
            }

            //我方鱼与敌方鱼碰撞检测
            if (myFish.getRec().intersects(enamy.getRec())) {
                if (myFish.level >= enamy.type) {
                    enamy.x = -200;
                    enamy.y = -200;
                    GameUtils.count += enamy.count;
                } else {
                    state = 2;
                }
            }
        }
    }

    public static void main(String[] args) {
        GameWin gameWin = new GameWin();
        gameWin.launch();
    }

    //重新开始
    void reGame() {
        GameUtils.EnamyList.clear();
        time = 0;
        myFish.level = 1;
        GameUtils.count = 0;
        myFish.x = 650;
        myFish.y = 400;
        myFish.width = 45;
        myFish.height = 45;
        boss = null;
        isboss = false;
    }
}
