package com.company.image;

import com.company.Cluster;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class Image extends JFrame {

    public Image(List<Cluster> clusters, boolean normalFile) {
        MyCanvas canvas = new MyCanvas(clusters, normalFile);

        setLayout(new BorderLayout());
        setSize(1900, 1900);
        setTitle("image");
        add("Center", canvas);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        setLocationRelativeTo(null);

        setVisible(true);
    }
}
