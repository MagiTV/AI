package com.company;

import java.util.ArrayList;
import java.util.List;

public class Cluster {
    private final List<Point> points = new ArrayList<>();
    private Point center;

    Cluster(Point center) {
        this.center = center;
    }

    public void addPoint(Point point) {
        points.add(point);
    }

    public Point getCenter() {
        return center;
    }

    public List<Point> getPoints() {
        return points;
    }

    //true -> moved
    public boolean moveCenter() {
        double newX = 0.0;
        double newY = 0.0;
        for (Point current : points) {
            newX += current.getX();
            newY += current.getY();
        }
        newX /= points.size();
        newY /= points.size();
        Point newCenter = new Point(newX, newY);
        if (newCenter.equals(center)) {
            return false;
        }
        center = newCenter;
        return true;
    }
}
