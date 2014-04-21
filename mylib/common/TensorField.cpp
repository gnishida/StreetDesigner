#include "TensorField.h"
#include "TopNSearch.h"

void TensorField::init(const Polygon2D &area, const Polyline2D &hintLine) {
	this->area = area;
	this->bbox = area.envelope();

	mat = cv::Mat((int)bbox.dy(), (int)bbox.dx(), CV_32F);

	std::vector<float> rotationAngles;

	for (int i = 0; i < hintLine.size(); ++i) {
		float angle;

		if (i > 0 && i < hintLine.size() - 1) {
			QVector2D vec = hintLine[i] - hintLine[i - 1];

			float angle1 = atan2f(vec.y(), vec.x());

			vec = hintLine[i + 1] - hintLine[i];

			float angle2 = atan2f(vec.y(), vec.x());

			angle = (angle1 + angle2) * 0.5f;
		} else if (i > 0) {
			QVector2D vec = hintLine[i] - hintLine[i - 1];

			angle = atan2f(vec.y(), vec.x());
		} else {
			QVector2D vec = hintLine[i + 1] - hintLine[i];

			angle = atan2f(vec.y(), vec.x());
		}

		rotationAngles.push_back(angle);
	}

	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			int x = bbox.minPt.x() + j;
			int y = bbox.minPt.y() + i;
			//if (!area.contains(QVector2D(x, y))) continue;

			TopNSearch<int> tns;
			for (int k = 0; k < hintLine.size(); ++k) {
				float dist = (hintLine[k] - QVector2D(x, y)).lengthSquared();
				tns.add(dist, k);
			}

			QList<int> nearestPts = tns.topN(2, TopNSearch<int>::ORDER_ASC);

			float angle1 = rotationAngles[nearestPts[0]];
			float angle2 = rotationAngles[nearestPts[1]];

			float dist1 = (hintLine[nearestPts[0]] - QVector2D(x, y)).length();
			float dist2 = (hintLine[nearestPts[1]] - QVector2D(x, y)).length();

			float angle = (angle1 * dist2 + angle2 * dist1) / (dist1 + dist2);

			mat.at<float>(i, j) = angle;
		}
	}
}

float TensorField::get(const QVector2D &pt) {
	int x = pt.x() - bbox.minPt.x();
	int y = pt.y() - bbox.minPt.y();
	if (x < 0) x = 0;
	if (x >= mat.cols) x = mat.cols - 1;
	if (y < 0) y = 0;
	if (y >= mat.rows) y = mat.rows - 1;
	return mat.at<float>(y, x);
}

void TensorField::save(const QString &filename) {
	cv::Mat img((int)(mat.rows / 10), (int)(mat.cols / 10), CV_8U, cv::Scalar(0));
	for (int i = 0; i < (int)(img.rows/10); ++i) {
		for (int j = 0; j < (int)(img.cols/10); ++j) {
			float totalAngle = 0.0f;
			int num = 0;

			for (int v = 0; v < 100; ++v) {
				for (int u = 0; u < 100; ++u) {
					int x = j * 100 + u;
					int y = i * 100 + v;
					if (x >= mat.cols) continue;
					if (y >= mat.rows) continue;

					totalAngle += mat.at<float>(y, x);
					num++;
				}
			}

			if (num > 0) totalAngle /= (float)num;

			int x1 = j * 10 + 5 - cosf(totalAngle) * 4.0f;
			int y1 = i * 10 + 5 - sinf(totalAngle) * 4.0f;
			int x2 = j * 10 + 5 + cosf(totalAngle) * 4.0f;
			int y2 = i * 10 + 5 + sinf(totalAngle) * 4.0f;
			cv::line(img, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255), 1);
		}
	}

	cv::flip(img, img, 0);
	cv::imwrite(filename.toUtf8().data(), img);
}
