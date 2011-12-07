/*******************************************************************************
 *                                                                              *
 * Author    :  Ari Þór H. Arnbjörnsson                                         *
 * Date      :  22 May 2011                                                     *
 * Website   :  http://flassari.is                                              *
 *                                                                              *
 * License:                                                                     *
 * Use, modification & distribution is subject to Boost Software License Ver 1. *
 * http://www.boost.org/LICENSE_1_0.txt                                         *
 *                                                                              *
 *******************************************************************************/

#include "clipper.hpp"
#include "Clipper.h"
#include "clipper.cpp"

#ifdef ANDROID
#include <android/log.h>
#endif

using namespace clipper;

JNIEXPORT jobjectArray JNICALL Java_fr_myrddin_clipper_Clipper_jniClipPolygon(
		JNIEnv *env, jobject obj, jintArray subjectPolygonArray, jintArray clipPolygonArray, jint clipTypeArg, jint subjectFillTypeArg,
		jint clipFillTypeArg) {

	jint subjectPolygonCount; 
	subjectPolygonCount = env->GetArrayLength(subjectPolygonArray);
	
	jint clipPolygonCount;
	clipPolygonCount = env->GetArrayLength(clipPolygonArray);
	
	Polygon subjectPolygon(subjectPolygonCount / 2), clipPolygon(
			clipPolygonCount / 2);
	Polygons solution;

	jint buf[subjectPolygonCount];
	jint i;
	env->GetIntArrayRegion(subjectPolygonArray, 0, subjectPolygonCount, buf);
	for (i = 0; i < subjectPolygonCount; i += 2) {
		subjectPolygon[i / 2] = IntPoint(buf[i], buf[i + 1]);
	}

	//	// Populate the clip polygon
	jint buf2[clipPolygonCount];
	env->GetIntArrayRegion(clipPolygonArray, 0, clipPolygonCount, buf2);
	for (i = 0; i < clipPolygonCount; i += 2) {
		clipPolygon[i / 2] = IntPoint(buf2[i], buf2[i + 1]);
	}

	ClipType clipType;
	switch (clipTypeArg) {
	default:
	case 0:
		clipType = ctIntersection;
		break;
	case 1:
		clipType = ctUnion;
		break;
	case 2:
		clipType = ctDifference;
		break;
	case 3:
		clipType = ctXor;
		break;
	}

	PolyFillType subjectFillType, clipFillType;
	switch (subjectFillTypeArg) {
	default:
	case 0:
		subjectFillType = pftEvenOdd;
		break;
	case 1:
		subjectFillType = pftNonZero;
		break;
	}
	switch (clipFillTypeArg) {
	default:
	case 0:
		clipFillType = pftEvenOdd;
		break;
	case 1:
		clipFillType = pftNonZero;
		break;
	}

	Clipper c;
	c.AddPolygon(subjectPolygon, ptSubject);
	c.AddPolygon(clipPolygon, ptClip);

	if (c.Execute(clipType, solution, subjectFillType, clipFillType)) {

		jobjectArray result;
		jclass intArrCls = env->FindClass("[I");
		if (intArrCls == NULL) {
			return NULL;
		}
		result = env->NewObjectArray((int) solution.size(), intArrCls,NULL);
		
		for (int i = 0; i < (int) solution.size(); i++) {

			jintArray poly;
			poly = env->NewIntArray((int) solution[i].size() * 2);

			if (poly == NULL) {
				return NULL; // out of memory error thrown
			}

			jint fill[(int) solution[i].size() * 2];
			for (int j = 0; j < (int) solution[i].size(); j++) {
				// Push all the vertices into the array
				fill[j * 2] = solution[i][j].X; // put whatever logic you want to populate the values here.
				fill[j * 2 + 1] = solution[i][j].Y;
			}
			// Insert the array into the returnArray
			env->SetIntArrayRegion(poly, 0, (int) solution[i].size() * 2,fill);
			
			env->SetObjectArrayElement(result, i, poly);
         	env->DeleteLocalRef(poly);
		}
		return result;
	}
	return NULL;

	// Cleanup
	//AS3_Release(subjectPolygon);
	//AS3_Release(clipPolygon);
}