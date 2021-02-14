package com.company;

import com.company.image.Image;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class Main {
    private static final Path normalFile = Path.of("src", "normal.txt");
    private static final Path unbalanceFile = Path.of("src", "unbalance.txt");
    private static final List<Point> points = new ArrayList<>();
    public static List<Cluster> bestClusters;
    private static boolean isNormalFile;
    private static int K;
    private static Path fileName;
    private static List<Cluster> clusters;
    private static double bestClustersCost = Double.MAX_VALUE;

    public static void main(String[] args) {
        inputData();

        readData();
        for (int i = 0; i < 100; ++i) {
            Collections.shuffle(points);

            initCenters();
            addPoints();
            while (moveCenters()) {
                movePoints();
            }

            double currentCost = calculateCost();
            if (currentCost < bestClustersCost) {
                bestClusters = clusters;
                bestClustersCost = currentCost;
            }
        }
        drawImage();
    }

    private static void inputData() {
        Scanner sc = new Scanner(System.in);
        System.out.println("file:");
        String lineRead = sc.nextLine();
        if (lineRead.equals("normal.txt") || lineRead.equals("n")) {
            fileName = normalFile;
            isNormalFile = true;
        }
        if (lineRead.equals("unbalance.txt") || lineRead.equals("u")) {
            fileName = unbalanceFile;
            isNormalFile = false;
        }
        System.out.println("k:");
        lineRead = sc.nextLine();
        K = Integer.parseInt(lineRead);
    }

    private static void readData() {
        try (BufferedReader reader = new BufferedReader(Files.newBufferedReader(fileName))) {
            String currentLine;
            while ((currentLine = reader.readLine()) != null) {
                String[] splitLine = currentLine.split("\t");
                points.add(new Point(Double.parseDouble(splitLine[0]), Double.parseDouble(splitLine[1])));
            }
        } catch (IOException ex) {
            System.out.println("File problem");
        }
    }

    private static void initCenters() {
        clusters = new ArrayList<>();
        for (int i = 0; i < K; ++i) {
            Cluster cluster = new Cluster(points.get(i));
            cluster.addPoint(points.get(i));
            clusters.add(cluster);
        }
    }

    private static void addPoints() {
        Point currentPoint;
        for (int i = K; i < points.size(); ++i) {
            currentPoint = points.get(i);
            clusters.get(findBestCluster(currentPoint)).addPoint(currentPoint);
        }
    }

    private static int findBestCluster(Point point) {
        double minDistance = Double.MAX_VALUE;
        double currDistance;
        int bestCluster = 0;
        for (int i = 0; i < K; ++i) {
            currDistance = point.calculateDistance(clusters.get(i).getCenter());
            if (currDistance < minDistance) {
                minDistance = currDistance;
                bestCluster = i;
            }
        }
        return bestCluster;
    }

    //true -> have been moved
    private static boolean moveCenters() {
        boolean differentCenters = false;
        for (int i = 0; i < K; ++i) {
            if (clusters.get(i).moveCenter()) {
                differentCenters = true;
            }
        }
        return differentCenters;
    }

    private static void movePoints() {
        for (int i = 0; i < K; ++i) {
            Cluster currentCluster = clusters.get(i);
            List<Point> clusterPoints = currentCluster.getPoints();
            int bestCluster;
            Point currentPoint;

            for (int j = 0; j < clusterPoints.size(); ++j) {
                currentPoint = clusterPoints.get(j);
                bestCluster = findBestCluster(currentPoint);
                if (bestCluster != i) {
                    clusterPoints.remove(j);
                    --j;
                    clusters.get(bestCluster).addPoint(currentPoint);
                }
            }
        }
    }

    private static double calculateCost() {
        double cost = 0.0;
        for (Cluster currentCluster : clusters) {
            double currentSum = 0.0;
            Point currentCenter = currentCluster.getCenter();
            for (Point currentPoint : currentCluster.getPoints()) {
                currentSum += currentPoint.calculateDistance(currentCenter);
            }
            cost += currentSum * currentCluster.getPoints().size();
        }
        return cost;
    }

    private static void drawImage() {
        Image image = new Image(bestClusters, isNormalFile);
    }
}
