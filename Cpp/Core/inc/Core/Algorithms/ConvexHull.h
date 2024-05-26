#pragma once

#include "Constexpr/ConstexprGeometry.h"

template<typename Point>
constexpr std::vector<Point> GetConvexHull(const std::vector<Point>& points) {
    if (points.size() < 3) return {};
    std::vector<Point> hull;
    size_t leftMost = 0;
    for (auto i = 1; i < points.size(); i++) {
        if (points[i].X < points[leftMost].X) {
            leftMost = i;
        }
    }

    size_t p = leftMost;
    size_t q = 0;

    do {
        hull.push_back(points[p]);
        q = (p + 1) % points.size();
        for (auto i = 0; i < points.size(); i++) {
            if (Constexpr::GetOrientation(points[p], points[i], points[q]) == Constexpr::Orientation::CounterClockwise) {
                q = i;
            }
        }

        p = q;
    } while (p != leftMost);

    return hull;
}

static_assert(GetConvexHull(std::vector<Point>{ {0, 3}, { 2, 2 }, { 1, 1 }, { 2, 1 }, { 3, 0 }, { 0, 0 }, { 3, 3 }}) == std::vector<Point>{ {0, 3}, { 0, 0 }, { 3, 0 }, { 3, 3 }});
