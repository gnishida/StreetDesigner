﻿#include <limits>
#include "../../common/global.h"
#include "../../common/Util.h"
#include "../../common/ConvexHull.h"
#include "../GraphUtil.h"
#include "RoadGeneratorHelper.h"

/**
* Checks if new edge will intersect an existing edge
**/
bool RoadGeneratorHelper::intersects(RoadGraph &roads, const QVector2D& p0, const QVector2D& p1, RoadEdgeDesc &eiClosest, QVector2D &closestIntPt) {
	float min_dist = std::numeric_limits<float>::max();
	bool intersected = false;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		// Highwayとはintersectしない
		if (roads.graph[*ei]->type == RoadEdge::TYPE_HIGHWAY) continue;

		for (int i = 0; i < roads.graph[*ei]->polyline.size() - 1; ++i) {
			//if new segment intersects other segment
			QVector2D intPt;
			float tab, tcd;
			if (Util::segmentSegmentIntersectXY(p0, p1, roads.graph[*ei]->polyline[i], roads.graph[*ei]->polyline[i + 1], &tab, &tcd, true, intPt)) {
				float dist = (p0 - intPt).lengthSquared();

				//make sure we get only closest segment
				if (dist < min_dist) {
					min_dist = dist;
					eiClosest = *ei;
					closestIntPt = intPt;
					intersected = true;
				}
			}

		}
	}	

	return intersected;
}

/**
 * polylineは、srcDescから始まるオーダになっている前提。
 */
bool RoadGeneratorHelper::intersects(RoadGraph &roads, RoadVertexDesc srcDesc, const Polyline2D &polyline, RoadEdgeDesc &eiClosest, QVector2D &closestIntPt) {
	for (int i = 0; i < polyline.size() - 1; ++i) {
		if (intersects(roads, polyline[i], polyline[i + 1], eiClosest, closestIntPt)) {
			return true;
		}
	}

	return false;
}

/**
 * 近くの頂点にsnapすべきか、チェックする。
 * srcDescから伸ばしてきたエッジの先端posに近い頂点を探す。
 * ただし、スナップによる変位角が、スナップ先頂点とのなす角の半分以下であること。
 * また、スナップによる移動距離が閾値以下であるものの中で、最短のものを選ぶ。
 * 
 * @param pos				エッジ先端
 * @param threshold			距離の閾値
 * @param srcDesc			この頂点からエッジを延ばしている
 * @param snapDesc			最も近い頂点
 * @return					もしsnapすべき頂点があれば、trueを返却する
 */
bool RoadGeneratorHelper::canSnapToVertex(RoadGraph& roads, const QVector2D &pos, const Polyline2D &polyline, float threshold, RoadVertexDesc srcDesc, RoadVertexDesc& snapDesc) {
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (GraphUtil::getDegree(roads, *vi) == 0) continue;

		// スナップによる変位角
		float phi = Util::diffAngle(pos - roads.graph[srcDesc]->pt, roads.graph[*vi]->pt - roads.graph[srcDesc]->pt);

		bool okay = true;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			RoadVertexDesc v2 = boost::target(*ei, roads.graph);

			// 自分自身には、スナップしない
			/*if (v2 == srcDesc) {
				okay = false;
				break;
			}*/

			// ２つのエッジのなす角
			//float theta = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[v2]->pt, roads.graph[*vi]->pt - roads.graph[srcDesc]->pt);
			float theta;
			if ((roads.graph[*ei]->polyline[0] - roads.graph[*vi]->pt).lengthSquared() < (roads.graph[*ei]->polyline[0] - roads.graph[v2]->pt).lengthSquared()) {
				theta = Util::diffAngle(roads.graph[*ei]->polyline[1] - roads.graph[*ei]->polyline[0], polyline[polyline.size() - 2] - polyline.last());
			} else {
				theta = Util::diffAngle(roads.graph[*ei]->polyline[roads.graph[*ei]->polyline.size() - 2] - roads.graph[*ei]->polyline.last(), polyline[polyline.size() - 2] - polyline.last());
			}
			if (phi > theta * 0.5f) {
				okay = false;
				break;
			}
		}

		if (!okay) continue;

		float dist2 = (roads.graph[*vi]->pt - pos).lengthSquared();
		if (dist2 < min_dist) {
			// 交差するかチェック
			Polyline2D tempPolyline = polyline;
			GraphUtil::movePolyline(roads, tempPolyline, roads.graph[srcDesc]->pt, roads.graph[*vi]->pt);
			if (!GraphUtil::isIntersect(roads, tempPolyline)) {
				min_dist = dist2;
				snapDesc = *vi;
			}
		}
	}

	if (min_dist <= threshold * threshold) return true;
	else return false;
}

/**
 * 近くの頂点にsnapすべきか、チェックする。
 * （この関数は、additionalSeeds用のスナップ関数）
 * srcDescから伸ばしてきたエッジの先端posに近い頂点を探す。
 * ただし、スナップによる変位角は90度未満で、接続された２つのエッジのなす角は90度より大きいこと。
 * さらに、スナップの結果、他のエッジの交差するものは除外する。
 * また、スナップによる移動距離が閾値以下であるものの中で、最短のものを選ぶ。
 * 
 * @param pos				エッジ先端
 * @param threshold			距離の閾値
 * @param srcDesc			この頂点からエッジを延ばしている
 * @param edge				このエッジ
 * @param snapDesc			最も近い頂点
 * @return					もしsnapすべき頂点があれば、trueを返却する
 */
bool RoadGeneratorHelper::canSnapToVertex2(RoadGraph& roads, const QVector2D &pos, float threshold, RoadVertexDesc srcDesc, RoadEdgeDesc edge, RoadVertexDesc& snapDesc) {
	float min_dist = std::numeric_limits<float>::max();

	Polyline2D polyline = roads.graph[edge]->polyline;
	if ((polyline[0] - roads.graph[srcDesc]->pt).lengthSquared() > (polyline.last() - roads.graph[srcDesc]->pt).lengthSquared()) {
		std::reverse(polyline.begin(), polyline.end());
	}

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (GraphUtil::getDegree(roads, *vi) != 1) continue;

		// もう一方の端点を取得
		bool isolated = true;
		RoadVertexDesc v2;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			v2 = boost::target(*ei, roads.graph);
			isolated = false;
			break;
		}

		// もう一方の端点がない場合は、スキップ
		if (isolated) continue;

		// 自分自身には、スナップしない
		if (v2 == srcDesc) continue;

		// スナップによる変位角
		float phi = Util::diffAngle(pos - roads.graph[srcDesc]->pt, roads.graph[*vi]->pt - roads.graph[srcDesc]->pt);

		// ２つのエッジのなす角
		float theta = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[v2]->pt, roads.graph[*vi]->pt - roads.graph[srcDesc]->pt);

		// スナップによる変位角が、２つのエッジのなす角の半分より大きい場合は、スキップ
		if (phi > theta * 0.5f) continue;

		float dist2 = (roads.graph[*vi]->pt - pos).lengthSquared();
		if (dist2 < min_dist) {
			// 交差するかチェック
			Polyline2D tempPolyline = polyline;
			GraphUtil::movePolyline(roads, tempPolyline, roads.graph[srcDesc]->pt, roads.graph[*vi]->pt);
			if (!GraphUtil::isIntersect(roads, tempPolyline, edge)) {
				min_dist = dist2;
				snapDesc = *vi;
			}
		}
	}

	if (min_dist <= threshold * threshold) return true;
	else return false;
}

/**
 * 近くの頂点にsnapすべきか、チェックする。
 * （この関数は、ExRoadGenerator用のスナップ関数）
 * srcDescから伸ばしてきたエッジの先端posに近い頂点を探す。
 * ただし、スナップによる変位角は90度未満で、接続された２つのエッジのなす角は90度より大きいこと。
 * さらに、スナップの結果、他のエッジの交差するものは除外する。
 * また、スナップによる移動距離が閾値以下であるものの中で、最短のものを選ぶ。
 * 
 * @param pos				エッジ先端
 * @param threshold			距離の閾値
 * @param srcDesc			この頂点からエッジを延ばしている
 * @param edge				このエッジ
 * @param snapDesc			最も近い頂点
 * @return					もしsnapすべき頂点があれば、trueを返却する
 */
bool RoadGeneratorHelper::canSnapToVertex3(RoadGraph& roads, RoadVertexDesc v_desc, float threshold, RoadVertexDesc& snapDesc) {
	float min_dist = std::numeric_limits<float>::max();

	// 当該頂点から出るポリラインをリストアップ
	std::vector<Polyline2D> polylines;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v_desc, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if ((roads.graph[*ei]->polyline[0] - roads.graph[v_desc]->pt).lengthSquared() > (roads.graph[*ei]->polyline.last() - roads.graph[v_desc]->pt).lengthSquared()) {
			std::reverse(roads.graph[*ei]->polyline.begin(), roads.graph[*ei]->polyline.end());
		}

		polylines.push_back(roads.graph[*ei]->polyline);
	}

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		bool badEdge = false;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			// 自分自身にはスナップしない
			if (*vi == v_desc) continue;

			RoadVertexDesc v2 = boost::target(*ei, roads.graph);

			if (v2 == v_desc) continue;
			
			for (int i = 0; i < polylines.size(); ++i) {
				// スナップによる変位角
				float phi = Util::diffAngle(polylines[i][0] - polylines[i].last(), roads.graph[*vi]->pt - polylines[i].last());

				// ２つのエッジのなす角
				float theta = Util::diffAngle(roads.graph[*vi]->pt - polylines[i].last(), roads.graph[*vi]->pt - roads.graph[v2]->pt);

				// スナップによる変位角が、２つのエッジのなす角の半分より大きい場合は、スキップ
				if (phi > theta * 0.5f) {
					badEdge = true;
					break;
				}
			}

			if (badEdge) continue;

			float dist2 = (roads.graph[*vi]->pt - roads.graph[v_desc]->pt).lengthSquared();
			if (dist2 < min_dist) {
				min_dist = dist2;
				snapDesc = *vi;
			}
		}
	}

	if (min_dist <= threshold * threshold) return true;
	else return false;
}

/**
 * 近くのエッジにsnapすべきか、チェックする。
 * ただし、スナップによる変位角は90度未満で、接続された２つのエッジのなす角は45度より大きいこと。
 * また、スナップによる移動距離が閾値以下であるものの中で、最短のものを選ぶ。
 * 
 * @param pos				エッジ先端
 * @param threshold			距離の閾値
 * @param srcDesc			この頂点からエッジを延ばしている
 * @param snapDesc			最も近い頂点
 * @return					もしsnapすべき頂点があれば、trueを返却する
 */
bool RoadGeneratorHelper::canSnapToEdge(RoadGraph& roads, const QVector2D& pos, float threshold, RoadVertexDesc srcDesc, RoadEdgeDesc& snapEdge, QVector2D &closestPt) {
	float min_dist = std::numeric_limits<float>::max();

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		// Highwayとは、intersectさせない（暫定的な実装）
		if (roads.graph[*ei]->type == RoadEdge::TYPE_HIGHWAY) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (src == srcDesc || tgt == srcDesc) continue;

		QVector2D closePt;
		float dist = GraphUtil::distance(roads, pos, *ei, closePt);

		// 変位角が90度以上なら、スキップ
		float phi = Util::diffAngle(closePt - roads.graph[srcDesc]->pt, pos - roads.graph[srcDesc]->pt);
		if (phi >= M_PI * 0.5f) continue;

		// ２つのエッジのなす角が45度以下なら、スキップ
		float theta1 = Util::diffAngle(closePt - roads.graph[src]->pt, closePt - roads.graph[srcDesc]->pt);
		if (theta1 <= M_PI * 0.25f) continue;
		float theta2 = Util::diffAngle(closePt - roads.graph[tgt]->pt, closePt - roads.graph[srcDesc]->pt);
		if (theta2 <= M_PI * 0.25f) continue;

		if (dist < 37.46f) {
			int k = 0;

			QVector2D pt1 = roads.graph[src]->pt;
			QVector2D pt2 = roads.graph[tgt]->pt;
			float d = GraphUtil::distance(roads, pos, *ei, closePt);
		}


		if (dist < min_dist) {
			min_dist = dist;
			snapEdge = *ei;
			closestPt = closePt;
		}
	}		

	if (min_dist < threshold) return true;
	else return false;
}

/**
 * 指定された位置posに最も近い頂点snapDescを取得し、そこまでの距離を返却する。
 * ただし、srcDesc、又は、その隣接頂点は、スナップ対象外とする。
 * 現在、この関数は使用されていない。
 */
float RoadGeneratorHelper::getNearestVertex(RoadGraph& roads, const QVector2D& pos, RoadVertexDesc srcDesc, RoadVertexDesc& snapDesc) {
	float min_dist = std::numeric_limits<float>::max();

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (src == srcDesc || tgt == srcDesc) continue;

		float dist1 = (roads.graph[src]->pt - pos).lengthSquared();
		float dist2 = (roads.graph[tgt]->pt - pos).lengthSquared();
		if (dist1 < min_dist) {
			min_dist = dist1;
			snapDesc = src;
		}
		if (dist2 < min_dist) {
			min_dist = dist2;
			snapDesc = tgt;
		}
	}

	return min_dist;
}

/**
 * 指定された位置posに最も近いエッジsnapEdgeを取得し、そこまでの距離を返却する。
 * ただし、srcDesc、又は、その隣接頂点は、スナップ対象外とする。
 * 現在、この関数は使用されていない。
 */
float RoadGeneratorHelper::getNearestEdge(RoadGraph& roads, const QVector2D& pt, RoadVertexDesc srcDesc, RoadEdgeDesc& snapEdge, QVector2D &closestPt) {
	float min_dist = std::numeric_limits<float>::max();
	RoadEdgeDesc min_e;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (!roads.graph[src]->valid) continue;
		if (!roads.graph[tgt]->valid) continue;

		if (src == srcDesc || tgt == srcDesc) continue;

		QVector2D pt2;
		for (int i = 0; i < roads.graph[*ei]->polyline.size() - 1; i++) {
			float dist = Util::pointSegmentDistanceXY(roads.graph[*ei]->polyline[i], roads.graph[*ei]->polyline[i + 1], pt, pt2);
			if (dist < min_dist) {
				min_dist = dist;
				snapEdge = *ei;
				closestPt = pt2;
			}
		}
	}

	return min_dist;
}

/**
 * カーネル設定済みの頂点の中から、直近のものを探す。
 * 現在、KDERoadGeneratorクラスでのみ使用。KDERoadGenerator2クラスでは使用していない。
 */
/*RoadVertexDesc RoadGeneratorHelper::getNearestVertexWithKernel(RoadGraph &roads, const QVector2D &pt) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// カーネルのない頂点はスキップ
		if (roads.graph[*vi]->kernel.id == -1) continue;

		float dist = (roads.graph[*vi]->getPt() - pt).lengthSquared();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}
*/

/**
 * 指定された点が、いずれかの頂点のテリトリーに入っているかチェックする。
 * ただし、頂点srcVertexは除く。
 * また、対象となる頂点へ伸びていて、且つ、対象となる頂点から、頂点srcVertexへもエッジが来る場合も、除外する。
 */
/*
bool RoadGeneratorHelper::invadingTerritory(RoadGraph &roads, const QVector2D &pt, RoadVertexDesc srcVertex, const QVector2D &targetPt) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (*vi == srcVertex) continue;
		if (!roads.graph[*vi]->valid) continue;

		// ベクトルsrcVertex→*viと、ベクトルsrcVertex→ptが、逆方向なら、スキップ
		if (QVector2D::dotProduct(roads.graph[*vi]->pt - roads.graph[srcVertex]->pt, pt - roads.graph[srcVertex]->pt) < 0) continue;

		// カーネルのない頂点はスキップ
		if (roads.graph[*vi]->kernel.id == -1) continue;

		// 誤差により、テリトリーに入っていると判断されてしまうのを防ぐため、0.9fをかける。
		if ((roads.graph[*vi]->pt - pt).lengthSquared() < roads.graph[*vi]->kernel.territory * roads.graph[*vi]->kernel.territory * 0.81f) {
			// 対象となる頂点方向へ、エッジが伸びていない場合（角度が15度より大きい）は、「侵入」と判断する
			if (Util::diffAngle(roads.graph[*vi]->pt - pt, targetPt - pt) > M_PI * 15.0f / 180.0f) return true;

			if (roads.graph[*vi]->kernel.id == -1) return true;

			// 対象となる頂点から、頂点srcVertex方向へ向かうエッジがなければ、「侵入」と判断する
			bool close = false;
			for (int i = 0; i < roads.graph[*vi]->kernel.edges.size(); ++i) {
				if (Util::diffAngle(roads.graph[*vi]->kernel.edges[i].edge.last(), roads.graph[srcVertex]->pt - roads.graph[*vi]->pt) < M_PI * 15.0f / 180.0f) {
					close = true;
					break;
				}
			}

			if (!close) return true;

			continue;
		}
	}

	return false;
}
*/

/**
 * カーネルの中で、指定された位置に最も近いものを探し、そのインデックスを返却する。
 */
/*int RoadGeneratorHelper::getClosestItem(const KDEFeature &f, int roadType, const QVector2D &pt) {
	float min_dist = std::numeric_limits<float>::max();
	int min_index = -1;
	for (int i = 0; i < f.items(roadType).size(); ++i) {
		float dist = (f.items(roadType)[i].pt - pt).lengthSquared();
		if (dist < min_dist) {
			min_dist = dist;
			min_index = i;
		}
	}

	return min_index;
}*/

/**
 * 指定された頂点について、指定されたエッジに似たエッジが既に登録済みかどうかチェックする。
 * polylineには、各点の、頂点からのオフセット座標が入る。ただし、頂点自体の座標(0, 0)は含まない。
 * 登録済みのエッジに対しては、エッジの端点への方向ベクトルとpolylineの端点の方向ベクトルのなす角度が30度未満なら、trueを返却する。
 */
bool RoadGeneratorHelper::isRedundantEdge(RoadGraph& roads, RoadVertexDesc v_desc, const Polyline2D &polyline) {
	if (polyline.size() == 0) true;

	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = out_edges(v_desc, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if (roads.graph[*ei]->polyline.size() <= 1) continue;

		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		/*
		Polyline2D edge = roads.graph[*ei]->polyLine;
		if ((roads.graph[v_desc]->pt - roads.graph[*ei]->polyLine[0]).lengthSquared() > (roads.graph[tgt]->pt - roads.graph[*ei]->polyLine[0]).lengthSquared()) {
			std::reverse(edge.begin(), edge.end());
		}

		for (int i = 1; i < 10; ++i) {
			int index1 = (edge.size() - 1) * i / 10;
			int index2 = polyline.size() * i/ 10;

			if (index1 == 0) continue;

			if (Util::diffAngle(edge[index1] - edge[0], polyline[index2]) < 0.3f) return true;
		}
		*/

		if (Util::diffAngle(roads.graph[tgt]->pt - roads.graph[v_desc]->pt, polyline.last()) < M_PI * 30.0f / 180.0f) {
			return true;
		}
	}

	return false;
}

/**
 * Example領域を使って、指定された位置のモジュラ位置（Example座標系）を返却する。
 * モジュラ位置は、Example領域のBBoxの中に入る座標となる。
 * ※ 現状の実装では、Axis Aligned BBoxを使用し、Oriented BBoxなどをサポートしていない。
 *
 * @param targetArea	ターゲット領域
 * @param exampleArea	Example領域（Exampleの中心を原点とする座標系）
 * @param pt			指定された位置
 * @param bbox			指定された位置が入る、BBox（ターゲット座標系）
 * @return				指定された位置をmoduloした結果（Example座標系）
 */
QVector2D RoadGeneratorHelper::modulo(const Polygon2D &targetArea, const Polygon2D &exampleArea, const QVector2D &pt, BBox &bbox) {
	QVector2D ret;

	QVector2D center = targetArea.envelope().midPt();
	QVector2D offsetPt = pt - center;

	BBox bboxExample = exampleArea.envelope();

	if (offsetPt.x() < bboxExample.minPt.x()) {
		ret.setX(bboxExample.maxPt.x() - (int)(bboxExample.minPt.x() - offsetPt.x()) % (int)bboxExample.dx());

		int u = (bboxExample.minPt.x() - offsetPt.x()) / bboxExample.dx() + 1;
		bbox.minPt.setX(bboxExample.minPt.x() - bboxExample.dx() * u);
		bbox.maxPt.setX(bboxExample.maxPt.x() - bboxExample.dx() * u);
	} else if (offsetPt.x() > bboxExample.maxPt.x()) {
		ret.setX(bboxExample.minPt.x() + (int)(offsetPt.x() - bboxExample.maxPt.x()) % (int)bboxExample.dx());

		int u = (offsetPt.x() - bboxExample.maxPt.x()) / bboxExample.dx() + 1;
		bbox.minPt.setX(bboxExample.minPt.x() + bboxExample.dx() * u);
		bbox.maxPt.setX(bboxExample.maxPt.x() + bboxExample.dx() * u);
	} else {
		ret.setX(offsetPt.x());
		bbox.minPt.setX(bboxExample.minPt.x());
		bbox.maxPt.setX(bboxExample.maxPt.x());
	}

	if (offsetPt.y() < bboxExample.minPt.y()) {
		ret.setY(bboxExample.maxPt.y() - (int)(bboxExample.minPt.y() - offsetPt.y()) % (int)bboxExample.dy());

		int v = (bboxExample.minPt.y() - offsetPt.y()) / bboxExample.dy() + 1;
		bbox.minPt.setY(bboxExample.minPt.y() - bboxExample.dy() * v);
		bbox.maxPt.setY(bboxExample.maxPt.y() - bboxExample.dy() * v);
	} else if (offsetPt.y() > bboxExample.maxPt.y()) {
		ret.setY(bboxExample.minPt.y() + (int)(offsetPt.y() - bboxExample.maxPt.y()) % (int)bboxExample.dy());

		int v = (offsetPt.y() - bboxExample.maxPt.y()) / bboxExample.dy() + 1;
		bbox.minPt.setY(bboxExample.minPt.y() + bboxExample.dy() * v);
		bbox.maxPt.setY(bboxExample.maxPt.y() + bboxExample.dy() * v);
	} else {
		ret.setY(offsetPt.y());
		bbox.minPt.setY(bboxExample.minPt.y());
		bbox.maxPt.setY(bboxExample.maxPt.y());
	}

	bbox.minPt += center;
	bbox.maxPt += center;

	return ret;
}

/**
 * 指定された点を含むBBoxに、initial seedがあるかチェックする。
 */
bool RoadGeneratorHelper::containsInitialSeed(const Polygon2D &targetArea, const Polygon2D &exampleArea, const QVector2D &pt) {
	BBox targetBBox = targetArea.envelope();
	BBox exampleBBox = exampleArea.envelope();

	QVector2D center = targetBBox.midPt();

	if (pt.x() > center.x()) {
		int u = targetBBox.dx() / 2 - exampleBBox.dx() / 2;
		if (u > 0) {
			u = (int)(u / exampleBBox.dx()) + 1;
		} else {
			u = 0;
		}
		if (pt.x() - center.x() - exampleBBox.dx() / 2 > u * exampleBBox.dx()) return false;
	} else {
		int u = targetBBox.dx() / 2 - exampleBBox.dx() / 2;
		if (u > 0) {
			u = (int)(u / exampleBBox.dx()) + 1;
		} else {
			u = 0;
		}
		if (center.x() - pt.x() - exampleBBox.dx() / 2 > u * exampleBBox.dx()) return false;
	}

	if (pt.y() > center.y()) {
		int v = targetBBox.dy() / 2 - exampleBBox.dy() / 2;
		if (v > 0) {
			v = (int)(v / exampleBBox.dy()) + 1;
		} else {
			v = 0;
		}
		if (pt.y() - center.y() - exampleBBox.dy() / 2 > v * exampleBBox.dy()) return false;
	} else {
		int v = targetBBox.dy() / 2 - exampleBBox.dy() / 2;
		if (v > 0) {
			v = (int)(v / exampleBBox.dy()) + 1;
		} else {
			v = 0;
		}
		if (center.y() - pt.y() - exampleBBox.dy() / 2 > v * exampleBBox.dy()) return false;
	}

	return true;
}

void RoadGeneratorHelper::createFourDirection(float direction, std::vector<float> &directions) {
	float deltaDir;
	float tmpDir;
	
	deltaDir = 0.5f * M_PI;
	for (int i = 0; i < 4; ++i) {
		if (i==0) {
			tmpDir = direction;
		} else {
			tmpDir = tmpDir + deltaDir;// + ucore::Util::genRand(-departingAnglesNoise, departingAnglesNoise);
		}

		if (tmpDir > 2.0 * M_PI) {
			tmpDir = tmpDir - 2.0 * M_PI; 
		}

		directions.push_back(tmpDir);
	}
}

/**
 * Deadendの道路セグメントを削除する。
 * ただし、onBoundaryフラグがtrueの場合は、対象外。
 * また、deadendフラグがtrueの場合も、対象外。
 * 処理を繰り返し、deadend道路がなくなるまで、繰り返す。
 */
void RoadGeneratorHelper::removeDeadend(RoadGraph& roads) {
	bool removed = false;

	do {
		removed = false;

		RoadEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			RoadVertexDesc src = boost::source(*ei, roads.graph);
			RoadVertexDesc tgt = boost::target(*ei, roads.graph);

			if (roads.graph[src]->onBoundary || roads.graph[tgt]->onBoundary) continue;
			if (roads.graph[src]->properties.contains("deadend") && roads.graph[src]->properties["deadend"] == true) continue;
			if (roads.graph[tgt]->properties.contains("deadend") && roads.graph[tgt]->properties["deadend"] == true) continue;

			if (GraphUtil::getDegree(roads, src) == 1 || GraphUtil::getDegree(roads, tgt) == 1) {
				roads.graph[*ei]->valid = false;
				removed = true;
			}
		}
	} while (removed);
}

/**
 * Dangling Edgeを少し伸ばして、他の頂点にsnapさせる。
 * ただし、onBoundaryフラグがtrueの場合は、対象外。
 * また、他のエッジと交差したり、角度がredundantなら、snapしないで削除する。
 */
void RoadGeneratorHelper::extendDanglingEdges(RoadGraph &roads) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (roads.graph[*vi]->onBoundary) continue;

		if (GraphUtil::getDegree(roads, *vi) != 1) continue;


		RoadVertexDesc snapDesc = GraphUtil::getVertex(roads, roads.graph[*vi]->pt, *vi);

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;
			break;
		}

		Polyline2D polyline;
		polyline.push_back(roads.graph[snapDesc]->pt);
		polyline.push_back(roads.graph[*vi]->pt);

		if (GraphUtil::hasRedundantEdge(roads, snapDesc, polyline, 1.0f)) {
			roads.graph[*ei]->valid = false;
		} else {
			std::reverse(polyline.begin(), polyline.end());
			if (GraphUtil::isIntersect(roads, polyline)) {
				roads.graph[*ei]->valid = false;
			} else {
				RoadEdgeDesc e = GraphUtil::addEdge(roads, *vi, snapDesc, roads.graph[*ei]->type, roads.graph[*ei]->lanes);
				roads.graph[e]->properties["generation_type"] = "pm";
				if (G::getFloat("roadInterpolationFactor") == 1.0f) {
					//roads.graph[*ei]->properties["byExample"] = true;
				}
			}
		}
	}
}

/**
 * Deadendの道路セグメントを、可能な限りつなぐ。
 * エリア間の道路網をつなぐ場合に使用する。
 * ただし、"fixed"フラグがtrueの頂点は、動かさない。
 */
void RoadGeneratorHelper::connectRoads(RoadGraph& roads, float distance_threshold, float angle_threshold) {
	// ConvexHullを計算する
	ConvexHull convexHull;
	Polygon2D hull;

	// 境界上の頂点、エッジの組をリストアップする
	QList<RoadVertexDesc> boundaryNodes;
	QMap<RoadVertexDesc, RoadEdgeDesc> boundaryEdges;
	QMap<RoadVertexDesc, RoadVertexDesc> boundaryNodesPair;
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		
		convexHull.addPoint(roads.graph[src]->pt);
		convexHull.addPoint(roads.graph[tgt]->pt);

		if (!roads.graph[src]->fixed && roads.graph[src]->onBoundary && GraphUtil::getDegree(roads, src) == 1) {
			if (!boundaryNodes.contains(src)) boundaryNodes.push_back(src);
			if (!boundaryEdges.contains(src)) boundaryEdges[src] = *ei;
			if (!boundaryNodesPair.contains(src)) boundaryNodesPair[src] = tgt;
		} else if (!roads.graph[tgt]->fixed && roads.graph[tgt]->onBoundary && GraphUtil::getDegree(roads, tgt) == 1) {
			if (!boundaryNodes.contains(tgt)) boundaryNodes.push_back(tgt);
			if (!boundaryEdges.contains(tgt)) boundaryEdges[tgt] = *ei;
			if (!boundaryNodesPair.contains(tgt)) boundaryNodesPair[tgt] = src;
		}
	}

	convexHull.convexHull(hull);

	// リストアップしたエッジを、それぞれ少しずつ伸ばしていき、他のエッジにぶつかったらストップする
	int numIterations = 50000;
	while (!boundaryNodes.empty() && numIterations >= 0) {
		RoadVertexDesc v_desc = boundaryNodes.front();
		boundaryNodes.pop_front();

		if (!roads.graph[v_desc]->valid) continue;

		RoadVertexDesc v2_desc = boundaryNodesPair[v_desc];
		RoadEdgeDesc e_desc = boundaryEdges[v_desc];

		QVector2D step;
		if ((roads.graph[v_desc]->pt - roads.graph[e_desc]->polyline[0]).lengthSquared() <= (roads.graph[v2_desc]->pt - roads.graph[e_desc]->polyline[0]).lengthSquared()) {
			step = roads.graph[e_desc]->polyline[0] - roads.graph[e_desc]->polyline[1];
		} else {
			step = roads.graph[e_desc]->polyline.last() - roads.graph[e_desc]->polyline[roads.graph[e_desc]->polyline.size() - 2];
		}
		step = step.normalized() * 20.0f;

		if (growRoadOneStep(roads, v_desc, step)) {
			if (hull.contains(roads.graph[v_desc]->pt)) {
				boundaryNodes.push_back(v_desc);
			}
		}

		numIterations--;
	}
}

bool RoadGeneratorHelper::growRoadOneStep(RoadGraph& roads, RoadVertexDesc srcDesc, const QVector2D& step) {
	bool snapped = false;
	bool intersected = false;

	QVector2D pt = roads.graph[srcDesc]->pt + step;
	RoadEdgeDesc closestEdge;

	// INTERSECTS -- If edge intersects other edge
	QVector2D intPoint;
	intersected = RoadGeneratorHelper::intersects(roads, roads.graph[srcDesc]->pt, pt, closestEdge, intPoint);
	if (intersected) {
		pt = intPoint;
	}

	if (intersected) {
		RoadVertexDesc splitVertex = GraphUtil::splitEdge(roads, closestEdge, pt);
		GraphUtil::snapVertex(roads, srcDesc, splitVertex);

		// 交差相手のエッジが、成長中のエッジなら、その成長をストップする
		RoadVertexDesc src = boost::source(closestEdge, roads.graph);
		RoadVertexDesc tgt = boost::target(closestEdge, roads.graph);
		if (roads.graph[src]->onBoundary) {
			RoadEdgeDesc e = GraphUtil::getEdge(roads, src, splitVertex);
			roads.graph[e]->valid = false;
			roads.graph[src]->valid = false;
		} else if (roads.graph[tgt]->onBoundary) {
			RoadEdgeDesc e = GraphUtil::getEdge(roads, tgt, splitVertex);
			roads.graph[e]->valid = false;
			roads.graph[tgt]->valid = false;
		}

		return false;
	} else {
		GraphUtil::moveVertex(roads, srcDesc, pt);
		return true;
	}	
}

/**
 * 指定された頂点に近くで、同じgroup_idで、example_descをプロパティに設定している頂点を探す。
 */
RoadVertexDesc RoadGeneratorHelper::getClosestVertexByExample(RoadGraph &roads, RoadVertexDesc v_desc) {
	float min_dist = std::numeric_limits<float>::max();
	RoadVertexDesc nearest_v_desc;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (roads.graph[*vi]->properties["group_id"] == roads.graph[v_desc]->properties["group_id"] && roads.graph[*vi]->properties.contains("example_desc")) {
			float dist = (roads.graph[*vi]->pt - roads.graph[v_desc]->pt).lengthSquared();
			if (dist < min_dist) {
				min_dist = dist;
				nearest_v_desc = *vi;
			}
		}
	}

	return nearest_v_desc;
}

void RoadGeneratorHelper::createFourEdges(ExFeature &f, int roadType, const QVector2D &ex_pt, int lanes, float direction, float step, std::vector<RoadEdgePtr> &edges) {
	edges.clear();

	// 指定された example空間座標に基づき、エッジ長と曲率の平均、分散を計算する
	BBox bbox = f.area.envelope();
	float dist = (bbox.dx() + bbox.dy()) * 0.25f;
	float avgLength, varLength, avgCurvature, varCurvature;
	GraphUtil::computeStatistics(f.roads(roadType), ex_pt, dist, avgLength, varLength, avgCurvature, varCurvature);

	std::vector<float> directions;
	createFourDirection(direction, directions);

	int sign = 1;
	
	for (int i = 0; i < directions.size(); ++i) {
		float length = Util::genRandNormal(avgLength, varLength);
		float curvature = Util::genRandNormal(avgCurvature, varCurvature);

		// 50%の確率で、どっちに曲がるか決定
		if (Util::genRand() >= 0.5f) {
			curvature = -curvature;
		}

		float deltaDir = 0.0f;

		RoadEdgePtr edge = RoadEdgePtr(new RoadEdge(roadType, lanes));

		QVector2D cur(0, 0);
		edge->polyline.push_back(cur);

		for (int j = 0; j < 10000 && edge->polyline.length() < length; ++j) {
			// Advance the current point to the next position
			cur.setX(cur.x() + cos(directions[i]) * step);
			cur.setY(cur.y() + sin(directions[i]) * step);

			edge->polyline.push_back(cur);

			// Update the direction
			/*
			deltaDir = 0.9 * deltaDir + 0.1 * Util::genRand(-1.0, 1.0);
			float newDir = directions[i] + curvature * 8.3333f * deltaDir;
			directions[i] = 0.9 * directions[i] + 0.1 * newDir;
			*/

			// 10%の確率で、曲がる方向を変える
			if (Util::genRand() >= 0.9f) sign = -sign;

			float theta = atan2f(curvature * step, 1.0f);
			directions[i] = directions[i] + theta * (float)sign * 0.5f;
		}

		edges.push_back(edge);
	}
}

/**
 * 位置ptが、bboxの中の上下左右、どの方向かを返却する。
 *
 * @return				0: 右 / 1: 上 / 2: 左 / 3: 下
 */
int RoadGeneratorHelper::getRelativeDirectionInArea(const BBox &bbox, const QVector2D &pt) {
	float dx = (pt.x() - bbox.midPt().x()) / bbox.dx();
	float dy = (pt.y() - bbox.midPt().y()) / bbox.dy();

	if (fabs(dx) > fabs(dy)) {
		if (dx >= 0) return 0;
		else return 2;
	} else {
		if (dy >= 0) return 1;
		else return 3;
	}
}

/**
 * エリアをfactor倍した領域に入っているかチェックする
 */
bool RoadGeneratorHelper::isWithinScaledArea(const Polygon2D &area, float factor, const QVector2D &pt) {
	QVector2D center = area.envelope().midPt();

	Polygon2D scaledArea;
	for (int i = 0; i < area.size(); ++i) {
		scaledArea.push_back((area[i] - center) * factor + center);
	}

	return scaledArea.contains(pt);
}

/*
void RoadGeneratorHelper::buildGraphFromKernel(RoadGraph& roads, const KDEFeatureItem &item, const QVector2D &offset) {
	roads.clear();

	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(item.pt + offset));
	RoadVertexDesc v_desc = GraphUtil::addVertex(roads, v);

	for (int i = 0; i < item.edges.size(); ++i) {
		RoadVertexPtr u = RoadVertexPtr(new RoadVertex(item.edges[i].edge.last()));
		RoadVertexDesc u_desc = GraphUtil::addVertex(roads, u);

		Polyline2D polyline;
		polyline.push_back(roads.graph[v_desc]->pt);
		for (int j = 0; j < item.edges[i].edge.size(); ++j) {
			polyline.push_back(roads.graph[v_desc]->pt + item.edges[i].edge[j]);
		}

		RoadEdgeDesc e_desc = GraphUtil::addEdge(roads, v_desc, u_desc, 1, false);
		roads.graph[e_desc]->polyline = polyline;
		roads.graph[e_desc]->color = QColor(192, 192, 255);
		roads.graph[e_desc]->bgColor = QColor(0, 0, 192);
	}
}
*/

/**
 * スナップの状況を画像として保存する。
 */
void RoadGeneratorHelper::saveSnappingImage(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc srcDesc, const Polyline2D &old_polyline, const Polyline2D &new_polyline, RoadVertexDesc snapDesc, const QString &filename_prefix) {
	QString filename = QString(filename_prefix + "_%1_%2.png").arg(srcDesc).arg(snapDesc);

	BBox bbox = area.envelope();
	cv::Mat mat = cv::Mat::zeros(bbox.dy(), bbox.dx(), CV_8UC3);

	// 道路網を描画
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		for (int i = 0; i < roads.graph[*ei]->polyline.size() - 1; ++i) {
			QVector2D pt1 = roads.graph[*ei]->polyline[i] - bbox.minPt;
			QVector2D pt2 = roads.graph[*ei]->polyline[i + 1] - bbox.minPt;
			cv::line(mat, cv::Point(pt1.x(), pt1.y()), cv::Point(pt2.x(), pt2.y()), cv::Scalar(128, 128, 128), 3, CV_AA);
		}
	}

	// 頂点を描画
	QVector2D circle1 = roads.graph[srcDesc]->pt - bbox.minPt;
	QVector2D circle2 = roads.graph[snapDesc]->pt - bbox.minPt;
	cv::circle(mat, cv::Point(circle1.x(), circle1.y()), 10, cv::Scalar(0, 0, 255), 3, CV_AA);
	cv::circle(mat, cv::Point(circle2.x(), circle2.y()), 10, cv::Scalar(255, 0, 0), 3, CV_AA);

	// ポリラインを描画
	for (int i = 0; i < old_polyline.size() - 1; ++i) {
		QVector2D pt1 = old_polyline[i] - bbox.minPt;
		QVector2D pt2 = old_polyline[i + 1] - bbox.minPt;
		cv::line(mat, cv::Point(pt1.x(), pt1.y()), cv::Point(pt2.x(), pt2.y()), cv::Scalar(128, 128, 255), 3, CV_AA);
	}
	for (int i = 0; i < new_polyline.size() - 1; ++i) {
		QVector2D pt1 = new_polyline[i] - bbox.minPt;
		QVector2D pt2 = new_polyline[i + 1] - bbox.minPt;
		cv::line(mat, cv::Point(pt1.x(), pt1.y()), cv::Point(pt2.x(), pt2.y()), cv::Scalar(255, 128, 128), 3, CV_AA);
	}

	cv::flip(mat, mat, 0);

	cv::imwrite(filename.toUtf8().data(), mat);
}
