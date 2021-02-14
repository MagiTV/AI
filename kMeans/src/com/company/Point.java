package com.company;

import java.util.Objects;

public class Point {
    private final double x;
    private final double y;

    Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Point)) return false;
        Point point = (Point) o;
        return Double.compare(point.x, x) == 0 && Double.compare(point.y, y) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }

    public double calculateDistance(Point other) {
        return Math.sqrt(Math.abs(this.x - other.x) * Math.abs(this.x - other.x) +
                Math.abs(this.y - other.y) * Math.abs(this.y - other.y));
    }
}
