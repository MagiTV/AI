package com.company.image;

import com.company.Cluster;
import com.company.Point;

import java.awt.*;
import java.util.List;
import java.util.Random;

public class MyCanvas extends Canvas {

    private final List<Cluster> clusters;
    private final boolean normalFile;

    MyCanvas(List<Cluster> clusters, boolean normalFile) {
        this.clusters = clusters;
        this.normalFile = normalFile;
    }

    @Override
    public void paint(Graphics graphics) {
        Random rand = new Random();
        float r, g, b;

        for (Cluster currentCluster : clusters) {
            r = rand.nextFloat();
            g = rand.nextFloat();
            b = rand.nextFloat();
            graphics.setColor(new Color(r, g, b));

            for (Point currentPoint : currentCluster.getPoints()) {

                if (normalFile) {
                    graphics.fillOval((int) (currentPoint.getX() * 30), (int) (currentPoint.getY() * 30), 5, 5);
                } else {
                    graphics.fillOval((int) (currentPoint.getX() / 650), (int) (currentPoint.getY() / 650), 5, 5);
                }
            }
        }
    }
}
