package com.company;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class Main {

    private static final Path fileName = Path.of("src", "breast-cancer.data");
    private static final int ITERATIONS_COUNT = 10;
    private static final int GROUP_SIZE = 28;
    private static final List<EnumMap<Attribute, String>> data = new ArrayList<>();
    private static int indexTestGroup = 0;
    private static Node root;

    public static void main(String[] args) {
        readData();
        fillEmptyValues();
        Collections.shuffle(data);

        test();
    }

    private static void readData() {
        try (BufferedReader reader = new BufferedReader(Files.newBufferedReader(fileName))) {
            String currentLine;
            while ((currentLine = reader.readLine()) != null) {
                String[] splitLine = currentLine.split(",");
                EnumMap<Attribute, String> currentLineData = new EnumMap<>(Attribute.class);
                currentLineData.put(Attribute.CLASS, splitLine[0]);
                currentLineData.put(Attribute.AGE, splitLine[1]);
                currentLineData.put(Attribute.MENOPAUSE, splitLine[2]);
                currentLineData.put(Attribute.TUMOR_SIZE, splitLine[3]);
                currentLineData.put(Attribute.INV_NODES, splitLine[4]);
                currentLineData.put(Attribute.NODE_CAPS, splitLine[5]);
                currentLineData.put(Attribute.DEG_MALIG, splitLine[6]);
                currentLineData.put(Attribute.BREAST, splitLine[7]);
                currentLineData.put(Attribute.BREAST_QUAD, splitLine[8]);
                currentLineData.put(Attribute.IRRADIAT, splitLine[9]);
                data.add(currentLineData);
            }
        } catch (IOException ex) {
            System.out.println("File problem");
        }
    }

    private static void makeModel() {
        List<EnumMap<Attribute, String>> newData = new ArrayList<>(data.subList(0, indexTestGroup));
        newData.addAll(data.subList(indexTestGroup + GROUP_SIZE, data.size()));
        EnumSet<Attribute> attributes = EnumSet.allOf(Attribute.class);
        attributes.remove(Attribute.CLASS);
        root = new Node(newData, attributes, null);
    }

    private static void test() {
        double averageResult = 0.0;
        double currentResult;

        for (int i = 0; i < ITERATIONS_COUNT; ++i) {
            makeModel();
            currentResult = testData();
            averageResult += currentResult;
            System.out.println((i + 1) + ": " + currentResult + "%");
            indexTestGroup += GROUP_SIZE;
        }

        System.out.println("\naverage: " + (averageResult / (double) ITERATIONS_COUNT) + "%");
    }

    private static double testData() {
        double correctAnswers = 0.0;

        for (int i = indexTestGroup; i < (indexTestGroup + GROUP_SIZE); ++i) {
            boolean foundLeaf = false;
            Node currentNode = root;

            while (!foundLeaf) {
                currentNode = currentNode.getChildByAttributeValue(data.get(i).get(currentNode.getMaxGainAttribute()));
                foundLeaf = currentNode.isLeaf();
            }

            if (currentNode.getClassName().equals(data.get(i).get(Attribute.CLASS))) {
                ++correctAnswers;
            }
        }

        return 100.0 * (correctAnswers / (double) GROUP_SIZE);
    }

    private static void fillEmptyValues() {
        for (Attribute currentAttribute : EnumSet.allOf(Attribute.class)) {
            Map<String, Integer> valuesCounts = new HashMap<>();
            for (EnumMap<Attribute, String> current : data) {
                String value = current.get(currentAttribute);
                if (valuesCounts.containsKey(value)) {
                    Integer count = valuesCounts.get(value);
                    valuesCounts.replace(value, ++count);
                } else {
                    valuesCounts.put(value, 1);
                }
            }

            Integer maxCount = -1;
            String maxValue = "";
            for (String current : valuesCounts.keySet()) {
                if (valuesCounts.get(current) > maxCount) {
                    maxCount = valuesCounts.get(current);
                    maxValue = current;
                }
            }

            for (EnumMap<Attribute, String> current : data) {
                if (current.get(currentAttribute).equals("?")) {
                    current.put(currentAttribute, maxValue);
                }
            }
        }
    }
}
