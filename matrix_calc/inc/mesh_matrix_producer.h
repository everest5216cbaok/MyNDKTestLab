#ifndef MESH_MATRIX_PRODUCER_H
#define MESH_MATRIX_PRODUCER_H

#include "common_structures.h"

void mesh_init(const char * originPath, const char * targetPath, MESH * originMesh, MESH * targetMesh);
MESH_MATRIX * middle_mesh_matrix_producer(MESH * originMesh, MESH * targetMesh, const float originWeight);
void mesh_init(float * origin_key_points, float * target_key_points, int origin_length, int target_length, MESH * originMesh, MESH * targetMesh);

#endif
