package com.company;

import java.util.ArrayList;
import java.util.List;

public enum Attribute {

    CLASS(List.of("no-recurrence-events", "recurrence-events")),
    AGE(List.of("10-19", "20-29", "30-39", "40-49", "50-59", "60-69", "70-79", "80-89", "90-99")),
    MENOPAUSE(List.of("lt40", "ge40", "premeno")),
    TUMOR_SIZE(List.of("0-4", "5-9", "10-14", "15-19", "20-24", "25-29", "30-34", "35-39", "40-44", "45-49", "50-54", "55-59")),
    INV_NODES(List.of("0-2", "3-5", "6-8", "9-11", "12-14", "15-17", "18-20", "21-23", "24-26", "27-29", "30-32", "33-35", "36-39")),
    NODE_CAPS(List.of("yes", "no")),
    DEG_MALIG(List.of("1", "2", "3")),
    BREAST(List.of("left", "right")),
    BREAST_QUAD(List.of("left_up", "left_low", "right_up", "right_low", "central")),
    IRRADIAT(List.of("yes", "no"));

    private final List<String> values;

    Attribute(List<String> values) {
        this.values = new ArrayList<>(values);
    }

    public List<String> getValues() {
        return values;
    }
}
