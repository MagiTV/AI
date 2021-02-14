package com.company;

import java.util.*;

public class Node {
    private final String recurrence = "recurrence-events";
    private final String noRecurrence = "no-recurrence-events";
    private final int K = 15;
    private final List<EnumMap<Attribute, String>> data;
    private final EnumSet<Attribute> attributes;
    private final Map<String, Node> children = new HashMap<>();
    private final String className;
    private boolean leaf = false;
    private Attribute maxGainAttribute;
    private int recurrenceCount;
    private int noRecurrenceCount;

    public Node(List<EnumMap<Attribute, String>> data, EnumSet<Attribute> attributes, String parentClass) {
        this.attributes = attributes;
        this.data = new ArrayList<>(data);

        if (this.data.isEmpty()) {
            leaf = true;
            className = parentClass;
            return;
        }

        for (Map<Attribute, String> current : data) {
            if (current.get(Attribute.CLASS).equals(recurrence)) {
                ++recurrenceCount;
            } else {
                ++noRecurrenceCount;
            }
        }

        if (recurrenceCount == 0) {
            leaf = true;
            className = noRecurrence;
            return;
        }

        if (noRecurrenceCount == 0) {
            leaf = true;
            className = recurrence;
            return;
        }

        if (recurrenceCount > noRecurrenceCount) {
            className = recurrence;
        } else if (recurrenceCount < noRecurrenceCount) {
            className = noRecurrence;
        } else {
            className = parentClass;
        }

        if (data.size() < K || attributes.isEmpty()) {
            leaf = true;
            return;
        }

        maxGainAttribute = calculateAttributeMaxGain();
        generateChildren(maxGainAttribute);
    }

    private double calculateEntropy(int a, int b) {
        if (a == 0 || b == 0) {
            return 0;
        }
        double aProbability = (double) a / (double) (a + b);
        double bProbability = (double) b / (double) (a + b);
        return (-(aProbability * Math.log(aProbability)) - (bProbability * Math.log(bProbability))) / Math.log(2);
    }

    private double calculateGain(Attribute attribute) {
        int currentRecurrence;
        int currentNoRecurrence;
        double entropy = 0.0;

        for (String currentValue : attribute.getValues()) {
            currentRecurrence = 0;
            currentNoRecurrence = 0;
            for (EnumMap<Attribute, String> current : data) {
                if (current.get(attribute).equals(currentValue)) {
                    if (current.get(Attribute.CLASS).equals(recurrence)) {
                        ++currentRecurrence;
                    } else {
                        ++currentNoRecurrence;
                    }
                }
            }
            entropy += ((double) (currentRecurrence + currentNoRecurrence) / (double) (recurrenceCount + noRecurrenceCount)) *
                    calculateEntropy(currentRecurrence, currentNoRecurrence);
        }

        return calculateEntropy(recurrenceCount, noRecurrenceCount) - entropy;
    }

    private Attribute calculateAttributeMaxGain() {
        double maxGain = Integer.MIN_VALUE;
        double currentGain;
        Attribute attributeMaxGain = null;

        for (Attribute currentAttribute : attributes) {
            currentGain = calculateGain(currentAttribute);
            if (currentGain > maxGain) {
                maxGain = currentGain;
                attributeMaxGain = currentAttribute;
            }
        }
        return attributeMaxGain;
    }

    private void generateChildren(Attribute attribute) {
        attributes.remove(attribute);

        for (String currentValue : attribute.getValues()) {
            List<EnumMap<Attribute, String>> newData = new ArrayList<>();
            for (EnumMap<Attribute, String> current : data) {
                if (current.get(attribute).equals(currentValue)) {
                    newData.add(current);
                }
            }
            children.put(currentValue, new Node(newData, attributes, this.className));
        }

        attributes.add(attribute);
    }

    public String getClassName() {
        return className;
    }

    public boolean isLeaf() {
        return leaf;
    }

    public Attribute getMaxGainAttribute() {
        return maxGainAttribute;
    }

    public Node getChildByAttributeValue(String value) {
        return children.get(value);
    }
}
