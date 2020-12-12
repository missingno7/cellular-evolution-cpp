#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/utilities/random.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/bitmap.hpp"


class Point {
public:
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

using Edge = std::pair<int, int>;


class CoverageData {
public:

    Bitmap _bmp;
    std::vector<Point> _points;
    std::vector<std::pair<int, int>> _edges;
    int _sc_width = 1000;
    int _sc_height = 1000;

    CoverageData() {}

    CoverageData(Random &rnd, PopConfig &cfg) {

        int n_points = cfg.getInt("npoints")[0];
        _sc_width = cfg.getInt("scwidth")[0];
        _sc_height = cfg.getInt("scheight")[0];

        _bmp.init(_sc_width, _sc_height);

        for (int i = 0; i < n_points; i++) {
            int x = rnd.nextInt(10, _sc_width - 10);
            int y = rnd.nextInt(10, _sc_height - 10);

            _points.push_back(Point(x, y));
        }

        std::vector<Edge> all_edges;
        for (int i = 0; i < _points.size(); i++) {
            for (int j = i + 1; j < _points.size(); j++) {
                all_edges.push_back(std::make_pair(i, j));
            }
        }

        quickSort(all_edges, _points, 0, all_edges.size() - 1);


        for (auto const &new_edge : all_edges) {

            bool intersect = false;
            for (auto const &edge : _edges) {
                if (is_intersect(_points[edge.first], _points[edge.second],
                                 _points[new_edge.first], _points[new_edge.second])) {
                    intersect = true;
                    break;
                }
            }
            if (!intersect) {
                _edges.push_back(new_edge);
            }
        }

    }

private:


    static bool ccw(Point &a, Point &b, Point &c) {
        return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
    }

    static bool test_intersect(Point &a, Point &b, Point &c, Point &d) {
        return ccw(a, c, d) != ccw(b, c, d)
               and
               ccw(a, b, c) != ccw(a, b, d);
    }

    static bool is_intersect(Point &a, Point &b, Point &c, Point &d) {
        return test_intersect(a, b, c, d)
               && test_intersect(b, a, c, d)
               && test_intersect(a, b, d, c)
               && test_intersect(b, a, d, c);
    }

    static float distance(Point &a, Point &b) {
        return sqrtf(powf(a.x - b.x, 2.0) + powf(a.y - b.y, 2.0));
    }

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
    static int partition(std::vector<Edge> &arr, std::vector<Point> &points, int low, int high) {
        int pivot = high;    // pivot
        int i = (low - 1);  // Index of smaller element

        for (int j = low; j <= high - 1; j++) {
            // If current element is smaller than or
            // equal to pivot
            if (distance(points[arr[j].first], points[arr[j].second]) <=
                distance(points[arr[pivot].first], points[arr[pivot].second])) {
                i++;    // increment index of smaller element
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
    static void quickSort(std::vector<Edge> &arr, std::vector<Point> &points, int low, int high) {
        if (low < high) {
            /* pi is partitioning index, arr[p] is now
               at right place */
            int pi = partition(arr, points, low, high);

            // Separately sort elements before
            // partition and after partition
            quickSort(arr, points, low, pi - 1);
            quickSort(arr, points, pi + 1, high);
        }
    }

};