﻿#include <boost/geometry/geometry.hpp> 
#include <boost/geometry/geometries/point_xy.hpp>
#include "Polyline2D.h"
#include "Util.h"

const QVector2D & Polyline2D::last() const {
	return at(size() - 1);
}

QVector2D & Polyline2D::last() {
	return at(size() - 1);
}

/**
 * 当該ポリラインを移動する。
 */
void Polyline2D::translate(const QVector2D &offset) {
	for (int i = 0; i < size(); ++i) {
		(*this)[i] += offset;
	}
}

/**
 * 指定された点を中心に、指定された角度だけ時計回りに回転したポリゴンを返却する。
 *
 * @param angle		時計回りの回転角度[degree]
 * @param orig		回転中心
 */
void Polyline2D::rotate(float angle, const QVector2D &orig) {
	for (int i = 0; i < size(); ++i) {
		(*this)[i] = Util::rotate(at(i), -Util::deg2rad(angle), orig);
	}
}

/**
 * スケールする
 */
void Polyline2D::scale(float factor) {
	for (int i = 0; i < size(); ++i) {
		(*this)[i] = QVector2D(at(i).x() * factor, at(i).y() * factor);
	}
}

/**
 * 全長の長さを計算して返却する。
 */
float Polyline2D::length() const {
	return length(size() - 1);
}

/**
 * 指定したindexの点までの長さを計算して返却する。
 */
float Polyline2D::length(int index) const {
	float length = 0.0f;

	for (int i = 0; i < index; ++i) {
		length += (at(i + 1) - at(i)).length();
	}

	return length;
}
